## Crazyflie

This directory houses the firmware apps utilized for testing, as well as the final one utilized at the competition. It also houses [Bitcraze's crazyflie firmware repository](https://github.com/bitcraze/crazyflie-firmware/tree/master) as a submodule, for flashing purposes.

# Flashing

In order to flash to the crazyflie memory, first make sure to be connected to the drone with the radio antenna, and set the drone to flashing mode by holding the power button for a few seconds.

Then, navigate to the desired apps folder, and compile the firmware code.
```bash
make all
```

Finally, flash the compiled code with the following command:
```bash
make cload
```
Just remember to be careful,  as soon as the flash is finished the drone will start executing the code, meaning it will take off.

For more information on how to flash the apps to the crazyflie firmware, please follow [Bitcraze's development tutorial](https://www.bitcraze.io/documentation/tutorials/getting-started-with-development/).