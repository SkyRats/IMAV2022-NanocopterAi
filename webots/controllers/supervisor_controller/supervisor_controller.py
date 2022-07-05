import math
import random
from controller import Supervisor

# create the Robot instance.
robot = Supervisor()

crazyflie_node = robot.getFromDef('CRAZYFLIE')

droneRotation = crazyflie_node.getField('rotation')

root_node = robot.getRoot()
children_field = root_node.getField('children')

#referencing each object node
gate_node = robot.getFromDef("GATE")
pole1_node = robot.getFromDef("POLE_1")
pole2_node = robot.getFromDef("POLE_2")
pole3_node =  robot.getFromDef("POLE_3")
pole4_node =  robot.getFromDef("POLE_4")
panel1_node = robot.getFromDef("PANEL_1")
panel2_node = robot.getFromDef("PANEL_2")






i = 0
timestep = int(robot.getBasicTimeStep())

panel1_rot = panel1_node.getField('rotation')
panel2_rot = panel2_node.getField('rotation')
        
# Main loop:
# - perform simulation steps until Webots is stopping the controller

objects = [pole1_node, pole2_node, pole3_node, pole4_node, panel1_node, panel2_node]

ocuppied_pos = []

for object in objects:
    print(object)
    ocuppied_pos.append(object.getPosition())
    
dronePose = crazyflie_node.getPosition()


while robot.step(timestep) != -1:
    dronePose = crazyflie_node.getPosition()
    #if i == 0:
    #    panel3_pose.setSFVec3f([3.5, -3.5, 0])
    #    panel3_rot.setSFRotation([0, 0, 1, 1.5708])
    print(dronePose[0])
    print(dronePose[1])
    cooldown = 5.5
    
    timer = robot.getTime()
    if (timer > cooldown):
        occupied_pos = []
        cooldown = timer + 5
        timer = robot.getTime()



        for object in objects:
            object_pose  = object.getPosition()
            if abs(dronePose[0] - object_pose[0]) < 3.5 and abs(dronePose[1] - object_pose[1]) < 3.5:
                #print('entrou1')
                if dronePose[0] > 0 and dronePose[1] > 0 and object_pose[0] > dronePose[0]:
            ##drone passou o objeto - ja posso mover
                    print('entrou2')
                    #for object in objects:
                    while(True):
                        object_pose  = object.getPosition()
                        new_pos = [random.randint(-4, 4), random.randint(-4, 0), 0]
                        print('entrou3')    
                        if not(new_pos in ocuppied_pos):
                            print('mudou1')
                            ocuppied_pos.remove(object_pose)
                            (object.getField('translation')).setSFVec3f(new_pos)
                            ocuppied_pos.append(new_pos)
                        #muda posicao
                            break
            #checar quantos objetos estao no quadrante de destino antes de mover

                elif dronePose[0] > 0 and dronePose[1] < 0 and dronePose[1] > object_pose[1]:
                    print('entrou4')
                    #for object in objects:
                    while(True):
                        object_pose  = object.getPosition()
                        new_pos = [random.randint(-4, 0), random.randint(-4, 4), 0]
                        print('entrou5')    
                        if not(new_pos in ocuppied_pos):
                            print('mudou2')
                            ocuppied_pos.remove(object_pose)
                            (object.getField('translation')).setSFVec3f(new_pos)
                            ocuppied_pos.append(new_pos)
                            #muda posicao
                            break


                elif dronePose[0] < 0 and dronePose[1] > 0 and dronePose[1] < object_pose[1]:
                    print('entrou6')
                    #for object in objects:
                    while(True):
                        object_pose  = object.getPosition()
                        new_pos = [random.randint(0, 4), random.randint(-4, 4), 0]
                        print('entrou7')    
                        if not(new_pos in ocuppied_pos):
                            print('mudou3')
                            ocuppied_pos.remove(object_pose)
                            (object.getField('translation')).setSFVec3f(new_pos)
                            ocuppied_pos.append(new_pos)
                            #muda posicao
                            break

                elif dronePose[0] <0 and dronePose[1] < 0 and dronePose[0] > object_pose[0]:
                    print('entrou8')
                    #for object in objects:
                    while(True):
                        object_pose  = object.getPosition()
                        new_pos = [random.randint(-4, 4), random.randint(0, 4), 0]

                        if not(new_pos in ocuppied_pos):
                            print('mudou4')
                            ocuppied_pos.remove(object_pose)
                            (object.getField('translation')).setSFVec3f(new_pos)
                            ocuppied_pos.append(new_pos)
                            #muda posicao
                            break
