# Installing and deploying the code

## Install GAP SDK docker and build autotiler (at least v3.9)

```
docker run --rm -it --name myAiDeckContainer bitcraze/aideck
cd /gap_sdk
source configs/ai_deck.sh
make autotiler
```

Then, after configuring your license, run, on **separate** terminal:

```
docker commit myAiDeckContainer aideck-with-autotiler
```

Finally, you can go back to the terminal running the container and close it

```
exit
```

## Deploying the code

**IMPORTANT:** always run the ai_deck.sh in a fresh new terminal. Never source this file two times in the same terminal (might have issues)

Connect your JTAG cable (J-link in this example) and run the following commands:

```
docker run --rm -it -v $HOME:/home --device /dev/ttyUSB0 --privileged -P aideck-with-autotiler
source /gap_sdk/configs/ai_deck.sh && export GAPY_OPENOCD_CABLE=/gap_sdk/tools/gap8-openocd/tcl/interface/jlink.cfg
make clean all image flash CORE=8 platform=board
```

After this, the neural network and gate detection will be flashed into your AI-deck memory.

If you want to debug via your JTAG cable, then run

```
make run platform=board
```
