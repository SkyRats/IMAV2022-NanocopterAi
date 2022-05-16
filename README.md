# IMAV2022 Nanocopter AI Challenge


This repository is meant to house the development of a [crazyflie 2.x. challenge, for the 2022 IMAV competition](https://2022.imavs.org/index.php/competition/nanocopter-ai-challenge/). 

This repository is divided in two branches, one used for simulating the drone and AI using [webots](https://cyberbotics.com/#cyberbotics), and another used for the actual drone, and eventually the final code used in the competition.

## This is the simulation branch

This repository is based on the [crazyflie_simulation by Bitcraze](https://github.com/bitcraze/crazyflie-simulation), as well as the [simulation enviroment created by IMAV](https://github.com/tudelft/crazyflie-simulation).

---

# Webots

Install webots from the [cyberbotics' website](https://cyberbotics.com/). We use version 2022a version. Do not forget to add the WEBOTS_HOME environment variable as shown in the documentation: https://cyberbotics.com/doc/guide/installation-procedure.

Then, compile the low-level controllers and set up the python virtual environment using
```
make all
```

Finally, activate the virtual environment and run webots using either:
```
source .venv/bin/activate
webots webots/worlds/crazyflie_cyberzoo_world.wbt
```
or
```
make run
```