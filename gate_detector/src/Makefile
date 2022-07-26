CC=gcc

BUILD_DIR ?= ../build
SRC_DIR ?= .

CFLAGS=-g -Wall -I$(SRC_DIR)/include

testSobel: $(BUILD_DIR)/testSobel.o $(BUILD_DIR)/sobelEdgeDetector.o $(BUILD_DIR)/imageIO.o
	$(CC) $(CFLAGS) $^ -o $(BUILD_DIR)/$@

testThreshold: $(BUILD_DIR)/testThreshold.o $(BUILD_DIR)/threshold.o $(BUILD_DIR)/imageIO.o
	$(CC) $(CFLAGS) $^ -o $(BUILD_DIR)/$@

edgeDetector: $(BUILD_DIR)/edgeDetector.o $(BUILD_DIR)/kernelUtils.o $(BUILD_DIR)/imageIO.o
	$(CC) $(CFLAGS) $^ -o $(BUILD_DIR)/$@

test: $(BUILD_DIR)/test.o $(BUILD_DIR)/imageIO.o
	$(CC) $(CFLAGS) $^ -o $(BUILD_DIR)/$@

$(BUILD_DIR)/testSobel.o: testSobel.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$@ -c $<

$(BUILD_DIR)/sobelEdgeDetector.o: sobelEdgeDetector.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$@ -c $<

$(BUILD_DIR)/edgeDetector.o: edgeDetector.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$@ -c $<

$(BUILD_DIR)/kernelUtils.o: kernelUtils.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$@ -c $<

$(BUILD_DIR)/testThreshold.o: testThreshold.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$@ -c $<

$(BUILD_DIR)/threshold.o: threshold.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$@ -c $<

$(BUILD_DIR)/imageIO.o: imageIO.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$@ -c $<

$(BUILD_DIR)/test.o: test.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$@ -c $<

.PHONY: clean

clean:
	rm -r $(BUILD_DIR)/**
