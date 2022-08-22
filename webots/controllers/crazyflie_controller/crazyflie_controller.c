/* 
 *  ...........       ____  _ __
 *  |  ,-^-,  |      / __ )(_) /_______________ _____  ___
 *  | (  O  ) |     / __  / / __/ ___/ ___/ __ `/_  / / _ \
 *  | / ,..´  |    / /_/ / / /_/ /__/ /  / /_/ / / /_/  __/
 *     +.......   /_____/_/\__/\___/_/   \__,_/ /___/\___/
 *  
 * MIT License
 * 
 * Copyright (c) 2022 Bitcraze
 * 
 * @file crazyflie_controller.c
 * Controls the crazyflie motors in webots
 */

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include <webots/robot.h>
#include <webots/motor.h>
#include <webots/gps.h>
#include <webots/gyro.h>
#include <webots/inertial_unit.h>
#include <webots/keyboard.h>
#include <webots/camera.h>
#include <webots/distance_sensor.h>

#include "../../../controllers/pid_controller.h"
#include "../../../controllers/gateFinder/imageIO.h"
#include "../../../controllers/gateFinder/GateFinder.h"
#include "../../../controllers/gateFinder/config.h"
#define PI 3.142857
#define PI_OVER_2 1.570796
#define PI_OVER_4 0.785398

int currentState = 0; //Controls what action the drone is currently doing
double oldYaw = 0;
int edges;
int rotationDir = 1; 
Point oldGate = (Point){0,0,0};

PGMImage *getImage(WbDeviceTag camera){ // Transforms webots image file into PGMImage struct
  PGMImage *img;
  img = (PGMImage *)malloc(sizeof(PGMImage));
  const unsigned char *rawImage = wb_camera_get_image(camera);
  img->x = wb_camera_get_width(camera);
  img->y = wb_camera_get_height(camera);
  img->data = (PGMPixel*)malloc((img->x)*(img->y) * sizeof(PGMPixel));
  for (int j = 0; j < img->y; j++){
    int line = j*img->x;
    for (int i = 0; i<=img->x; i++){
      int gray = wb_camera_image_get_gray(rawImage, img->x, i, j);
      img->data[i + line].gray = gray;
    }
  }
  return img;
}

double obsDetector(WbDeviceTag camera){//Calls python script for running the obstacle detection AI
  const unsigned char *uRawImage = wb_camera_get_image(camera);
  char *rawImage = malloc(sizeof(uRawImage));
  
  for (size_t i = 0; i < sizeof(uRawImage); i++) {
        rawImage[i] = uRawImage[i];
  }
  
  Py_Initialize();
  
  
  PyObject *sys = PyImport_ImportModule("sys");                                                                                                                                                                     
  PyObject *path = PyObject_GetAttrString(sys, "path");                                                                                                                                                     
  PyList_Insert(path, 0, PyUnicode_FromString("."));
  
  PyObject *pModule = PyImport_ImportModule("ObsDetector");
  
  
  if (pModule != NULL){
    PyObject *pFunc = PyObject_GetAttrString(pModule, "obsDetector");
    if(pFunc && PyCallable_Check(pFunc)){
      PyObject *pValue = PyObject_CallFunction(pFunc,"y", uRawImage);
      PyErr_Print();
      double prediction = PyFloat_AsDouble(pValue);
      printf("%lf\n", prediction);
      return prediction;
    }
  }
  return 0;
 
}
//Defines what edge the drone is at
int edgeFinder(double xGlobal, double yGlobal){
  if (xGlobal >= 2.5){
    if (yGlobal >= 2.5){
     return 0; 
    }
    else if (yGlobal <= -2.5){
      return 1;
    }
    else{
      return 5;
    }
  }
  else if( xGlobal <= -2.5){
    if (yGlobal >= 2.5){
      return 3;
    }
    else if(yGlobal <= -2.5){
      return 2;
    }
    else{
      return 7;
    }
  }
  else{
    if(yGlobal >= 2.5){
      return 4;
    }
    else if(yGlobal <= -2.5){
      return 6;
    }
  }
  return -1;
}
//Rotates to the desired angle (in radians)
float rotate(double actualYaw, double desiredYaw) {
 
  
  if (desiredYaw > PI){
    desiredYaw = (desiredYaw - 2*PI );
  }
  else if(desiredYaw <= -PI){
    desiredYaw = (desiredYaw + 2*PI);
  }
  
  actualYaw *= 10;
  desiredYaw *= 10;
  //printf("%lf, %lf \n", actualYaw, desiredYaw);
  if (round(actualYaw) != round(desiredYaw)){
    return rotationDir*0.5;
  }
  return 0;   
}

//Rotates until facing inwards
float lookInward(int edges, double actualYaw){
  switch(edges){
    case 0: //Corner of the first quadrant (+,+)
      if (actualYaw >= PI_OVER_4 && actualYaw < PI){
        rotationDir = 1;
        return 0.5;
      }
      else if(actualYaw < PI_OVER_4 && actualYaw > -PI_OVER_2){
        rotationDir = -1;
        return -0.5;
      }
      else { // No longer looking outwards
        currentState = 3;
        return 0;
      }
      break;
    case 1: //Corner of the second quadrant (+,-)
      if (actualYaw >= -PI_OVER_4 && actualYaw < PI_OVER_2){
        rotationDir = 1;
        return 0.5;
      }
      else if (actualYaw < -PI_OVER_4 && actualYaw > -PI){
        rotationDir = -1;
        return -0.5;
      }
      else {// No longer looking outwards
        currentState = 3;
        return 0;
      }
       break;
    case 2://Corner of the third Quadrant (-,-)
      if (actualYaw >= -3*PI_OVER_4 && actualYaw < 0){
        rotationDir = 1;
        return 0.5;
      }
      else if ((actualYaw < -3*PI_OVER_4 && actualYaw > -PI) || (actualYaw <= PI && actualYaw > PI_OVER_2)){
        rotationDir = -1;
        return -0.5;
      }
      else {//No longer looking outwards
        currentState = 3;
        return 0;
      }
      break;
    case 3://Corner of the fourth quadrant (-,+)
      if ((actualYaw >= 3*PI_OVER_4 && actualYaw <= PI) || (actualYaw > -PI && actualYaw < -PI_OVER_2)){
        rotationDir = 1;
        return 0.5;
      }
      else if (actualYaw < 3*PI_OVER_4 && actualYaw > 0){
        rotationDir = -1;
        return -0.5;
      }
      else{//No longer looking outwards
        currentState = 3;
        return 0;
      }
      break;
    case 4: //Reaches first edge (y reached the upper limit, but not x)
      if (actualYaw >= PI_OVER_2 && actualYaw < PI){
        rotationDir = 1;
        return 0.5;
      }
      else if (actualYaw < PI_OVER_2 && actualYaw > 0){
        rotationDir = -1;
        return -0.5;
      }
      else{
        currentState = 3;
        return 0;
      }
      break;
    case 5://Reaches second edge (x reached the upper limit, but not y)
      if (actualYaw >= 0 && actualYaw < PI_OVER_2){
        rotationDir = 1;
        return 0.5;
      }
      else if (actualYaw < 0 && actualYaw > -PI_OVER_2){
        rotationDir = -1;
        return -0.5;
      }
      else{
        currentState = 3;
        return 0;
      }
      break;
    case 6://Reaches third edge (y reached the lower limit, but not x)
      if (actualYaw >= -PI_OVER_2  && actualYaw < 0){
        rotationDir = 1;
        return 0.5;
      }
      else if(actualYaw < -PI_OVER_2 && actualYaw > -PI){
        rotationDir = -1;
        return -0.5;
      }
      else{
        currentState = 3;
        return 0;
      }
      break;
    case 7://Reaches fourth edge (x reached the lower limit, but not y)
      if (actualYaw > -PI && actualYaw < -PI_OVER_2){
        rotationDir = 1;
        return 0.5;
      }
      else if(actualYaw <= PI && actualYaw > PI_OVER_2){
        rotationDir = -1;
        return -0.5;
      }
      else{
         currentState = 3;
         return 0;
      }
      break;
  }
  return 0;
}

int main(int argc, char **argv) {
  wb_robot_init();

  int timestep = (int)wb_robot_get_basic_time_step();




  // Initialize motors
  WbDeviceTag m1_motor = wb_robot_get_device("m1_motor");
  wb_motor_set_position(m1_motor, INFINITY);
  wb_motor_set_velocity(m1_motor, -1.0);
  WbDeviceTag m2_motor = wb_robot_get_device("m2_motor");
  wb_motor_set_position(m2_motor, INFINITY);
  wb_motor_set_velocity(m2_motor, 1.0);
  WbDeviceTag m3_motor = wb_robot_get_device("m3_motor");
  wb_motor_set_position(m3_motor, INFINITY);
  wb_motor_set_velocity(m3_motor, -1.0);
  WbDeviceTag m4_motor = wb_robot_get_device("m4_motor");
  wb_motor_set_position(m4_motor, INFINITY);
  wb_motor_set_velocity(m4_motor, 1.0);

  // Initialize Sensors
  WbDeviceTag imu = wb_robot_get_device("inertial unit");
  wb_inertial_unit_enable(imu, timestep);
  WbDeviceTag gps = wb_robot_get_device("gps");
  wb_gps_enable(gps, timestep);
  wb_keyboard_enable(timestep);
  WbDeviceTag gyro = wb_robot_get_device("gyro");
  wb_gyro_enable(gyro, timestep);
  WbDeviceTag camera = wb_robot_get_device("camera");
  wb_camera_enable(camera, timestep);
  WbDeviceTag range_front = wb_robot_get_device("range_front");
  wb_distance_sensor_enable(range_front, timestep);
  WbDeviceTag range_left = wb_robot_get_device("range_left");
  wb_distance_sensor_enable(range_left, timestep);
  WbDeviceTag range_back = wb_robot_get_device("range_back");
  wb_distance_sensor_enable(range_back, timestep);
  WbDeviceTag range_right = wb_robot_get_device("range_right");
  wb_distance_sensor_enable(range_right, timestep);


  // Wait for 2 seconds
  while (wb_robot_step(timestep) != -1) {
    if (wb_robot_get_time() > 2.0)
      break;
  }

  // Initialize variables
  ActualState_t actualState = {0};
  DesiredState_t desiredState = {0};
  double pastXGlobal =0;
  double pastYGlobal=0;
  double past_time = wb_robot_get_time();

  // Initialize PID gains.
  GainsPID_t gainsPID;
  gainsPID.kp_att_y = 1;
  gainsPID.kd_att_y = 0.5;
  gainsPID.kp_att_rp =0.5;
  gainsPID.kd_att_rp = 0.1;
  gainsPID.kp_vel_xy = 2;
  gainsPID.kd_vel_xy = 0.5;
  gainsPID.kp_z = 10;
  gainsPID.ki_z = 50;
  gainsPID.kd_z = 5;
  init_pid_attitude_fixed_height_controller();

  // Initialize struct for motor power
  MotorPower_t motorPower;
  
  printf("Take off!\n");

 

  while (wb_robot_step(timestep) != -1) {

    const double dt = wb_robot_get_time() - past_time;

    // Get measurements
    actualState.roll = wb_inertial_unit_get_roll_pitch_yaw(imu)[0];
    actualState.pitch = wb_inertial_unit_get_roll_pitch_yaw(imu)[1];
    actualState.yaw_rate = wb_gyro_get_values(gyro)[2];
    actualState.altitude = wb_gps_get_values(gps)[2];
    double xGlobal= wb_gps_get_values(gps)[0];
    double vxGlobal = (xGlobal - pastXGlobal)/dt;
    double yGlobal = wb_gps_get_values(gps)[1];
    double vyGlobal = (yGlobal - pastYGlobal)/dt;
    
    
    // Get body fixed velocities
    double actualYaw = wb_inertial_unit_get_roll_pitch_yaw(imu)[2];
    double cosyaw = cos(actualYaw);
    double sinyaw = sin(actualYaw);
    actualState.vx = vxGlobal * cosyaw + vyGlobal * sinyaw;
    actualState.vy = - vxGlobal * sinyaw + vyGlobal * cosyaw;

    
    // Initialize values
    desiredState.roll = 0;
    desiredState.pitch = 0;
    desiredState.vx = 0;
    desiredState.vy = 0;
    desiredState.altitude = 2.0;
    desiredState.yaw_rate = 0;

    double forwardDesired = 0;
    double sidewaysDesired = 0;
    double yawDesired = 0;
    
    PGMImage *image = getImage(camera);
    Point gateCenter;
    
    obsDetector(camera);

    switch(currentState){
      case 0:// Going toward something
        if (xGlobal >= 3.5|| xGlobal <= -3.5 || yGlobal >= 3.5 || yGlobal <= -3.5){
          edges = edgeFinder(xGlobal, yGlobal);
          currentState = 1;
        }
          forwardDesired = 1;
          break;
                       
      case 1:// Reached the borders, backing up
        if (xGlobal >= 3.5|| xGlobal <= -3.5 || yGlobal >= 3.5 || yGlobal <= -3.5){
          forwardDesired = -1;
        }
        else{
          oldYaw = actualYaw;
          currentState = 2;
        }
        break;
      case 2: // Rotates to look inwards
        //printf("%d\n", edges);
        
        yawDesired = lookInward(edges, actualYaw);
        oldYaw = actualYaw;
        break;
      case 3: // Gate scanner
        
        if (edges <= 3){
          
          yawDesired = rotate(actualYaw, oldYaw + rotationDir*PI_OVER_2);
        }
        else{
        
          yawDesired = rotate(actualYaw, oldYaw + rotationDir*PI);
        }
        
        gateCenter = GateFinder(image);
        
        if(!(gateCenter.x == 0 && gateCenter.y == 0 && gateCenter.grayShade == 0)){
          printf("%d, %d, %d\n", gateCenter.x, gateCenter.y, gateCenter.grayShade);
          if (gateCenter.x > 70 && gateCenter.x < 130){
            oldYaw = actualYaw;
            currentState = 0;
          }
        }
        if (yawDesired == 0){
          oldYaw = actualYaw;
          currentState = 0;
        }
        
        
        break;
      case 4: // Avoiding obstacles
        break;
    }
    // Control altitude
    int key = wb_keyboard_get_key();
    while (key > 0) {
      switch (key) {
        case WB_KEYBOARD_UP:
          forwardDesired = + 1.0;
          break;
        case WB_KEYBOARD_DOWN:
          forwardDesired = - 1.0;
          break;
        case WB_KEYBOARD_RIGHT:
          sidewaysDesired = - 1.0;
          break;
        case WB_KEYBOARD_LEFT:
          sidewaysDesired = + 1.0;
          break;
        case 'Q':
          yawDesired = 0.5;
          break;
        case 'E':
          yawDesired = - 0.5;
          break;
        }
      key = wb_keyboard_get_key();
    }
    
    // Example how to get sensor data
    // range_front_value = wb_distance_sensor_get_value(range_front));
    // const unsigned char *image = wb_camera_get_image(camera);


    desiredState.yaw_rate = yawDesired;

    // PID velocity controller with fixed height
    desiredState.vy = sidewaysDesired;
    desiredState.vx = forwardDesired;
    pid_velocity_fixed_height_controller(actualState, &desiredState,
    gainsPID, dt, &motorPower);

    // PID attitude controller with fixed height
    /*desiredState.roll = sidewaysDesired;
    desiredState.pitch = forwardDesired;
     pid_attitude_fixed_height_controller(actualState, &desiredState,
    gainsPID, dt, &motorPower);*/
    
    // Setting motorspeed
    wb_motor_set_velocity(m1_motor, - motorPower.m1);
    wb_motor_set_velocity(m2_motor, motorPower.m2);
    wb_motor_set_velocity(m3_motor, - motorPower.m3);
    wb_motor_set_velocity(m4_motor, motorPower.m4);
    
    // Save past time for next time step
    past_time = wb_robot_get_time();
    pastXGlobal = xGlobal;
    pastYGlobal = yGlobal;

    
  };

  wb_robot_cleanup();

  return 0;
}
