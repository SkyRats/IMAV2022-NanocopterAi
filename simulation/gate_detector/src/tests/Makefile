CC=gcc
RM=rm -r
IMAGE_VIEWER=xdg-open

MK_LIB=ar rcs

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

CFLAGS=-g -Wall -I$(INC_DIR)/

testGateFinder: $(BUILD_DIR)/testGateFinder.o $(BUILD_DIR)/imageIO.o $(BUILD_DIR)/convolution.o $(BUILD_DIR)/morphological_operators.o $(BUILD_DIR)/filter.o $(BUILD_DIR)/histogram.o $(BUILD_DIR)/regionGrowing.o $(BUILD_DIR)/queue.o $(BUILD_DIR)/findGate.o $(BUILD_DIR)/threshold.o $(BUILD_DIR)/stack.o $(BUILD_DIR)/queue.o
	$(MK_LIB) $(BUILD_DIR)/libTestGateFinder.a $^
	$(CC) $(CFLAGS) $< -L$(BUILD_DIR) -lTestGateFinder -o $(BUILD_DIR)/$@

$(BUILD_DIR)/testGateFinder.o: testGateFinder.c $(BUILD_DIR)/imageIO.o $(BUILD_DIR)/convolution.o $(BUILD_DIR)/morphological_operators.o $(BUILD_DIR)/filter.o $(BUILD_DIR)/histogram.o $(BUILD_DIR)/regionGrowing.o $(BUILD_DIR)/queue.o $(BUILD_DIR)/findGate.o
	$(CC) $(CFLAGS) -o $@ -c $<

$(BUILD_DIR)/%.o: $(COMMON_LIB)/%.c
	$(CC) $(CFLAGS) -o $@ -c $<

$(BUILD_DIR)/%.o: $(CONVOLUTION_LIB)/%.c
	$(CC) $(CFLAGS) -o $@ -c $<

$(BUILD_DIR)/%.o: $(DATA_STRUCT_LIB)/%.c
	$(CC) $(CFLAGS) -o $@ -c $<

$(BUILD_DIR)/%.o: $(FILTERS_LIB)/%.c
	$(CC) $(CFLAGS) -o $@ -c $<

$(BUILD_DIR)/%.o: $(MORPHOLOGICAL_LIB)/%.c
	$(CC) $(CFLAGS) -o $@ -c $<

$(BUILD_DIR)/%.o: $(OTHERS_LIB)/%.c
	$(CC) $(CFLAGS) -o $@ -c $<

$(BUILD_DIR)/%.o: $(SEGMENTATION_LIB)/%.c
	$(CC) $(CFLAGS) -o $@ -c $<

run:
	./$(BUILD_DIR)/$(EXECUTABLE) $(INPUT_IMAGE) $(OUTPUT_IMAGE)

open:
	$(IMAGE_VIEWER) $(IMG_DIR)/$(IMAGE)

.PHONY: clean

clean:
	$(RM) $(BUILD_DIR)/**
