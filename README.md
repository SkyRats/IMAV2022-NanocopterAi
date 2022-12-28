## Cloning this repository

In order for all submodules to be cloned alongside this repository, please do it through one of the following ways:

- The more straightforward way

```bash

git clone --recurse-submodules https://github.com/SkyRats/IMAV2022-NanocopterAi.git

```

- Alternatively

```bash

git clone https://github.com/SkyRats/IMAV2022-NanocopterAi.git
git submodule update --init --recursive

```

# IMAV2022 Nanocopter AI Challenge

This repository contains the Crazyflie and AI-deck code which granted Skyrats the third place in [2022 IMAV's Nanocopoter AI Challenge](https://github.com/pulp-platform/pulp-dronet).

- `simulation` contains code for simulating an environment with part of our controller code and image processing logic on Webots;
- `crazyflie` contains the code uploaded on the crazyflie;
- `AI-deck` contains the code uploaded on the AI-deck (including PULP's [PULP DroNet](https://github.com/pulp-platform/pulp-dronet)).


