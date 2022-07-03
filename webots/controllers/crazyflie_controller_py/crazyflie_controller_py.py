
  
#  ...........       ____  _ __
#  |  ,-^-,  |      / __ )(_) /_______________ _____  ___
#  | (  O  ) |     / __  / / __/ ___/ ___/ __ `/_  / / _ \
#  | / ,..´  |    / /_/ / / /_/ /__/ /  / /_/ / / /_/  __/
#     +.......   /_____/_/\__/\___/_/   \__,_/ /___/\___/
 
# MIT License

# Copyright (c) 2022 Bitcraze

# @file crazyflie_controllers_py.py
# Controls the crazyflie motors in webots in Python

"""crazyflie_controller_py controller."""


from controller import Robot
from controller import Motor
from controller import InertialUnit
from controller import GPS
from controller import Gyro
from controller import Keyboard
from controller import Camera
from controller import DistanceSensor
import torch
import torchvision.transforms as transforms
import numpy as np

import cv2 as cv
from PIL import Image, ImageOps
from math import cos, sin

import sys
sys.path.append('../../../controllers/')
from dronet_v2_nemo_dory import dronet_nemo
from  pid_controller import init_pid_attitude_fixed_height_controller, pid_velocity_fixed_height_controller
from pid_controller import MotorPower_t, ActualState_t, GainsPID_t, DesiredState_t
robot = Robot()

timestep = int(robot.getBasicTimeStep())
## Initialize Ai model
#Initialize model
model = dronet_nemo()
#load the parameters into the model
model.load_state_dict(torch.load("../../../controllers/Pulp-Dronet/dronet_v2_nemo_dory_original.pth"), "CUDA")

## Initialize motors
m1_motor = robot.getDevice("m1_motor");
m1_motor.setPosition(float('inf'))
m1_motor.setVelocity(-1)
m2_motor = robot.getDevice("m2_motor");
m2_motor.setPosition(float('inf'))
m2_motor.setVelocity(1)
m3_motor = robot.getDevice("m3_motor");
m3_motor.setPosition(float('inf'))
m3_motor.setVelocity(-1)
m4_motor = robot.getDevice("m4_motor");
m4_motor.setPosition(float('inf'))
m4_motor.setVelocity(1)

## Initialize Sensors
imu = robot.getDevice("inertial unit")
imu.enable(timestep)
gps = robot.getDevice("gps")
gps.enable(timestep)
Keyboard().enable(timestep)
gyro = robot.getDevice("gyro")
gyro.enable(timestep)
camera = robot.getDevice("camera")
camera.enable(timestep)
range_front = robot.getDevice("range_front")
range_front.enable(timestep)
range_left = robot.getDevice("range_left")
range_left.enable(timestep)
range_back = robot.getDevice("range_back")
range_back.enable(timestep)
range_right = robot.getDevice("range_right")
range_right.enable(timestep)

## Wait for two seconds
#while robot.step(timestep) != -1:
#    if robot.getTime()>2.0:
#        break
    
## Initialize variables
actualState = ActualState_t()
desiredState = DesiredState_t()
pastXGlobal = 0
pastYGlobal = 0
currentPos = [0,0]
currentVel = [0,0]
past_time = robot.getTime()
loopCounter = 0
turnBool = False
turnCounter = 0

targets = [[3,-3,2]] 
reachTarget = False
## Initialize PID gains.
gainsPID = GainsPID_t()
gainsPID.kp_att_y = 1
gainsPID.kd_att_y = 0.5
gainsPID.kp_att_rp =0.5
gainsPID.kd_att_rp = 0.1
gainsPID.kp_vel_xy = 2;
gainsPID.kd_vel_xy = 0.5;
gainsPID.kp_z = 10
gainsPID.ki_z = 50
gainsPID.kd_z = 5
init_pid_attitude_fixed_height_controller();

## Initialize struct for motor power
motorPower = MotorPower_t()

print('Take off!')


def go_to_POS(actualPOS, desiredPOS): 
    
    dX = desiredPOS[0] - actualPOS[0]
    dY = desiredPOS[1] - actualPOS[1]
    reachedX = 0
    reachedY = 0
    
    
    forwardDesired = 0.0
    sidewaysDesired = 0.0

    if dX > 0.1:
        forwardDesired += 0.5
       
    elif dX < -0.1:
        forwardDesired -= 0.5
    else:
        reachedX += 1
       
    if dY > 0.1:
        sidewaysDesired += 0.5
        
    elif dY < -0.1:
        sidewaysDesired -= 0.5
    else:
        reachedY += 1   
        
    if reachedX + reachedY == 2:    
        return forwardDesired, sidewaysDesired, True
        
    else:
        return forwardDesired, sidewaysDesired, False


def turn90(turnBool, turnCounter):
    if turnCounter < 50:
        turnCounter += 1
        return 1, turnCounter, turnBool
    else:
        turnCounter = 0
        turnBool = False
        return 0, turnCounter, turnBool

def ObsChecker(): #Uses Pulp Dronet to calculate chance of collision
    cameraData = camera.getImage()
    im = np.frombuffer(cameraData, np.uint8).reshape((camera.getHeight(), camera.getWidth(), 4))
    im = cv.cvtColor(im, cv.COLOR_BGR2GRAY)
    #Gets camera data, transforms into cv2 image Garyscale image
    
    imP = Image.fromarray(im) #Transforms image into Pillow image
    transform = transforms.ToTensor()
    Tensor = transform(imP) #Transforms image into Tensor
    Tensor = Tensor.unsqueeze(0)
    Result = model.forward(Tensor)
    return Result[1][0].item()
    

def targetSetter(currentPos, targets):
    #In the future, this is where the AI and gate finder functions will be called         
    targets.append([6,0,2]) 
    return targets
           
# Main loop:
while robot.step(timestep) != -1:

    dt = robot.getTime() - past_time;

    ## Get measurements
    actualState.roll = imu.getRollPitchYaw()[0]
    actualState.pitch = imu.getRollPitchYaw()[1]
    actualState.yaw_rate = gyro.getValues()[2];
    actualState.altitude = gps.getValues()[2];
    xGlobal = gps.getValues()[0]
    vxGlobal = (xGlobal - pastXGlobal)/dt
    yGlobal = gps.getValues()[1]
    vyGlobal = (yGlobal - pastYGlobal)/dt
    currentPos[0] += currentVel[0]*dt
    currentPos[1] += currentVel[1]*dt
    
   
    ## Get body fixed velocities
    actualYaw = imu.getRollPitchYaw()[2];
    cosyaw = cos(actualYaw)
    sinyaw = sin(actualYaw)
    actualState.vx = vxGlobal * cosyaw + vyGlobal * sinyaw
    actualState.vy = - vxGlobal * sinyaw + vyGlobal * cosyaw
    
    
    
    ## Initialize values
    desiredState.roll = 0
    desiredState.pitch = 0
    desiredState.vx = 0
    desiredState.vy = 0
    desiredState.yaw_rate = 0
    desiredState.altitude = 2
    yawDesired = 0
    
    
    
        
    if loopCounter == 100 and len(targets) > 0:
        forwardDesired, sidewaysDesired,reachTarget =go_to_POS(currentPos, targets[0])
        desiredState.altitude = targets[0][2]        
    elif len(targets) == 0:
        if turnBool:
            yawDesired, turnCounter, turnBool = turn90(turnBool, turnCounter)
        else:
            forwardDesired, sidewaysDesired = 0,0
            targets = targetSetter(currentPos, targets)
    else:
        forwardDesired, sidewaysDesired = 0,0
        loopCounter += 1
    
    
    if reachTarget:
        targets.pop(0)
        reachTarget = False
        if len(targets) == 0:
            currentPos = [0,0]
            currentVel = [0,0]
            turnBool = True
            
    key = Keyboard().getKey()
    while key>0:
        if key == Keyboard.UP:
            forwardDesired = 0.2
        elif key == Keyboard.DOWN:
            forwardDesired = -0.2
        elif key == Keyboard.RIGHT:
            sidewaysDesired  = -0.2
        elif key == Keyboard.LEFT:
            sidewaysDesired = 0.2
        elif key == ord('Q'):
            yawDesired =  + 1
        elif key == ord('E'):
            yawDesired = - 1

    key = Keyboard().getKey()

    ## Example how to get sensor data
    ## range_front_value = range_front.getValue();
    ## cameraData = camera.getImage()


    desiredState.yaw_rate = yawDesired;

    ## PID velocity controller with fixed height
    desiredState.vy = sidewaysDesired;
    desiredState.vx = forwardDesired;
   
    pid_velocity_fixed_height_controller(actualState, desiredState,
    gainsPID, dt, motorPower);
    

    ## PID attitude controller with fixed height
    
    #desiredState.roll = sidewaysDesired;
    #desiredState.pitch = forwardDesired;
    #pid_attitude_fixed_height_controller(actualState, desiredState,
    #gainsPID, dt, motorPower);
    ObsChance = ObsChecker()
    if ObsChance >= 0.8:
        print(ObsChance)
    
    m1_motor.setVelocity(-motorPower.m1)
    m2_motor.setVelocity(motorPower.m2)
    m3_motor.setVelocity(-motorPower.m3)
    m4_motor.setVelocity(motorPower.m4)
    
    past_time = robot.getTime()
    pastXGlobal = xGlobal
    pastYGlobal = yGlobal
    currentVel = [forwardDesired, sidewaysDesired]
    
    pass