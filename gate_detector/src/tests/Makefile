CC=gcc
RM=rm -r
IMAGE_VIEWER=xdg-open

SRC_DIR ?= ..
BUILD_DIR ?= $(SRC_DIR)/../build
INC_DIR ?= $(SRC_DIR)/../include
IMG_DIR=../../images

COMMON_LIB = $(SRC_DIR)/common
CONVOLUTION_LIB = $(SRC_DIR)/convolution
DATA_STRUCT_LIB = $(SRC_DIR)/data_structures
FILTERS_LIB = $(SRC_DIR)/filters
MORPHOLOGICAL_LIB = $(SRC_DIR)/morphological_operators
OTHERS_LIB = $(SRC_DIR)/others
SEGMENTATION_LIB = $(SRC_DIR)/segmentation

CFLAGS=-g -Wall -I$(INC_DIR)/ -L$(BUILD_DIR)/

testGateFinder: $(BUILD_DIR)/testGateFinder.o
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$@ $(BUILD_DIR)/*.o

$(BUILD_DIR)/testGateFinder.o: testGateFinder.c $(BUILD_DIR)/imageIO.o $(BUILD_DIR)/convolution.o $(BUILD_DIR)/erosion.o $(BUILD_DIR)/filter.o $(BUILD_DIR)/histogram.o $(BUILD_DIR)/regionGrowing.o $(BUILD_DIR)/queue.o $(BUILD_DIR)/findGate.o
	$(CC) $(CFLAGS) -o $@ -c $<

$(BUILD_DIR)/imageIO.o: $(COMMON_LIB)/imageIO.c
	$(CC) $(CFLAGS) -o $@ -c $<

$(BUILD_DIR)/convolution.o: $(CONVOLUTION_LIB)/convolution.c
	$(CC) $(CFLAGS) -o $@ -c $<

$(BUILD_DIR)/erosion.o: $(MORPHOLOGICAL_LIB)/erosion.c
	$(CC) $(CFLAGS) -o $@ -c $<

$(BUILD_DIR)/filter.o: $(FILTERS_LIB)/filter.c
	$(CC) $(CFLAGS) -o $@ -c $<

$(BUILD_DIR)/histogram.o: $(OTHERS_LIB)/histogram.c $(BUILD_DIR)/threshold.o
	$(CC) $(CFLAGS) -o $@ -c $<

$(BUILD_DIR)/threshold.o: $(SEGMENTATION_LIB)/threshold.c
	$(CC) $(CFLAGS) -o $@ -c $<

$(BUILD_DIR)/regionGrowing.o: $(SEGMENTATION_LIB)/regionGrowing.c $(BUILD_DIR)/stack.o $(BUILD_DIR)/queue.o
	$(CC) $(CFLAGS) -o $@ -c $<

$(BUILD_DIR)/stack.o: $(DATA_STRUCT_LIB)/stack.c
	$(CC) $(CFLAGS) -o $@ -c $<

$(BUILD_DIR)/queue.o: $(DATA_STRUCT_LIB)/queue.c
	$(CC) $(CFLAGS) -o $@ -c $<

$(BUILD_DIR)/findGate.o: $(OTHERS_LIB)/findGate.c
	$(CC) $(CFLAGS) -o $@ -c $<


run:
	./$(BUILD_DIR)/$(EXECUTABLE) $(INPUT_IMAGE) $(OUTPUT_IMAGE)

open:
	$(IMAGE_VIEWER) $(IMG_DIR)/$(IMAGE)

.PHONY: clean

clean:
	$(RM) $(BUILD_DIR)/**
