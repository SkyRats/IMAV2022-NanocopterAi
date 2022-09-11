# Copyright (C) 2017 GreenWaves Technologies
# All rights reserved.

# This software may be modified and distributed under the terms
# of the BSD license.  See the LICENSE file for details.

GATE_FINDER_INC=../app_imav/app_imav_ai-deck/include
GATE_FINDER_SRC=../app_imav/app_imav_ai-deck/src

COMMON_LIB = $(GATE_FINDER_SRC)/common
CONVOLUTION_LIB = $(GATE_FINDER_SRC)/convolution
DATA_STRUCT_LIB = $(GATE_FINDER_SRC)/data_structures
FILTERS_LIB = $(GATE_FINDER_SRC)/filters
MORPHOLOGICAL_LIB = $(GATE_FINDER_SRC)/morphological_operators
OTHERS_LIB = $(GATE_FINDER_SRC)/others
SEGMENTATION_LIB = $(GATE_FINDER_SRC)/segmentation


ifndef GAP_SDK_HOME
  $(error Source sourceme in gap_sdk first)
endif

include common.mk

IMAGE=$(CURDIR)/images/frame_2.pgm
include common/model_decl.mk

# Here we set the memory allocation for the generated kernels
# REMEMBER THAT THE L1 MEMORY ALLOCATION MUST INCLUDE SPACE
# FOR ALLOCATED STACKS!
CLUSTER_STACK_SIZE?=6096
CLUSTER_SLAVE_STACK_SIZE?=1024
TOTAL_STACK_SIZE = $(shell expr $(CLUSTER_STACK_SIZE) \+ $(CLUSTER_SLAVE_STACK_SIZE) \* 7)

FREQ_CL?=175
FREQ_FC?=250
MODEL_L1_MEMORY=$(shell expr 60000 \- $(TOTAL_STACK_SIZE))
MODEL_L2_MEMORY?=200000
MODEL_L3_MEMORY=8000000

# hram - HyperBus RAM
# qspiram - Quad SPI RAM
MODEL_L3_EXEC=hram
# hflash - HyperBus Flash
# qpsiflash - Quad SPI Flash
MODEL_L3_CONST=hflash

pulpChip = GAP
PULP_APP = imagenet
USE_PMSIS_BSP=1

APP = imagenet
#MAIN ?= main.c
MAIN ?= gate_and_ai.c
APP_SRCS += $(MAIN) $(MODEL_GEN_C) $(MODEL_COMMON_SRCS) $(CNN_LIB)
APP_SRCS += ../app_imav/app_imav_ai-deck/src/tests/board/GateDetect.c
APP_SRCS += $(FILTERS_LIB)/filter.c
APP_SRCS += $(CONVOLUTION_LIB)/convolution.c
APP_SRCS += $(SEGMENTATION_LIB)/threshold.c
APP_SRCS += $(MORPHOLOGICAL_LIB)/morphological_operators.c
APP_SRCS += $(DATA_STRUCT_LIB)/queue.c
APP_SRCS += $(DATA_STRUCT_LIB)/stack.c
APP_SRCS += $(OTHERS_LIB)/findGate.c
APP_SRCS += $(OTHERS_LIB)/histogram.c
APP_SRCS += $(SEGMENTATION_LIB)/regionGrowing.c

APP_INC  += $(GAP_LIB_PATH)/include

ifndef CORE
CORE=1
else
CORE= $(CORE)
endif

APP_CFLAGS += -DNUM_CORES=$(CORE) -DGAP_SDK=1 -g -O3 -w -mno-memcpy -fno-tree-loop-distribute-patterns
# list of includes file
APP_CFLAGS += -I. -I$(MODEL_COMMON_INC) -I$(TILER_EMU_INC) -I$(TILER_INC) $(CNN_LIB_INCLUDE) -I$(MODEL_BUILD) -I$(GATE_FINDER_INC)
# pass also macro defines to the compiler
APP_CFLAGS += -DAT_MODEL_PREFIX=$(MODEL_PREFIX) $(MODEL_SIZE_CFLAGS)
APP_CFLAGS += -DSTACK_SIZE=$(CLUSTER_STACK_SIZE) -DSLAVE_STACK_SIZE=$(CLUSTER_SLAVE_STACK_SIZE)
APP_CFLAGS += -DAT_IMAGE=$(IMAGE) -DPERF -DMODEL_ID=$(MODEL_ID) -DFREQ_FC=$(FREQ_FC) -DFREQ_CL=$(FREQ_CL)
APP_CFLAGS += -DAT_CONSTRUCT=$(AT_CONSTRUCT) -DAT_DESTRUCT=$(AT_DESTRUCT) -DAT_CNN=$(AT_CNN) -DAT_L3_ADDR=$(AT_L3_ADDR) -DFROM_FILE
# APP_CFLAGS 	+= -DASYNC=1

APP_LDFLAGS += -lgaptools -lgaplib

# this line is needed to flash into the chip the model tensors
# and other constants needed by the Autotiler
#READFS_FILES=$(abspath $(MODEL_TENSORS))
READFS_FILES := BUILD_MODEL_SQ8BIT/network_L3_Flash_Const.dat
PLPBRIDGE_FLAGS += -f

# all depends on the model
all:: model

clean:: clean_model

include common/model_rules.mk
$(info APP_SRCS... $(APP_SRCS))
$(info APP_CFLAGS... $(APP_CFLAGS))
include $(RULES_DIR)/pmsis_rules.mk

