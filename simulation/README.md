
## Simulation

This repository is based on the [crazyflie_simulation by Bitcraze](https://github.com/bitcraze/crazyflie-simulation), as well as the [simulation enviroment created by IMAV](https://github.com/tudelft/crazyflie-simulation).

This simulation is meant to test the use of our gate detection algorithm, the obstacle detection AI, as well as the overall strategy employed at the competition.
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
