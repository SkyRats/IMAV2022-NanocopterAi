CC=gcc

BUILD_DIR ?= ../build
SRC_DIR ?= .

CFLAGS=-g -Wall -I$(SRC_DIR)/include

testFilterEdgeDetectionErosionAndAdaptiveThresholding: $(BUILD_DIR)/testFilterEdgeDetectionErosionAndAdaptiveThresholding.o $(BUILD_DIR)/convolution.o $(BUILD_DIR)/erosion.o $(BUILD_DIR)/imageIO.o $(BUILD_DIR)/filter.o $(BUILD_DIR)/histogram.o $(BUILD_DIR)/threshold.o $(BUILD_DIR)/regionGrowing.o $(BUILD_DIR)/stack.o $(BUILD_DIR)/queue.o $(BUILD_DIR)/findGate.o
	$(CC) $(CFLAGS) $^ -o $(BUILD_DIR)/$@

testHistogramTechniques: $(BUILD_DIR)/testHistogramTechniques.o $(BUILD_DIR)/imageIO.o $(BUILD_DIR)/filter.o $(BUILD_DIR)/histogram.o $(BUILD_DIR)/threshold.o
	$(CC) $(CFLAGS) $^ -o $(BUILD_DIR)/$@

watershedFilter: $(BUILD_DIR)/watershedFilter.o $(BUILD_DIR)/watershed.o $(BUILD_DIR)/PQueue.o $(BUILD_DIR)/queue.o $(BUILD_DIR)/vector.o $(BUILD_DIR)/imageIO.o $(BUILD_DIR)/threshold.o
	$(CC) $(CFLAGS) $^ -o $(BUILD_DIR)/$@

filterAndEdgeDetect: $(BUILD_DIR)/filterAndEdgeDetect.o $(BUILD_DIR)/threshold.o $(BUILD_DIR)/convolution.o $(BUILD_DIR)/filter.o $(BUILD_DIR)/imageIO.o
	$(CC) $(CFLAGS) $^ -o $(BUILD_DIR)/$@

testFilter: $(BUILD_DIR)/testFilter.o $(BUILD_DIR)/filter.o $(BUILD_DIR)/imageIO.o
	$(CC) $(CFLAGS) $^ -o $(BUILD_DIR)/$@

detectingEdges: $(BUILD_DIR)/detectingEdges.o $(BUILD_DIR)/threshold.o $(BUILD_DIR)/convolution.o $(BUILD_DIR)/imageIO.o
	$(CC) $(CFLAGS) $^ -o $(BUILD_DIR)/$@

testSobel: $(BUILD_DIR)/testSobel.o $(BUILD_DIR)/convolution.o $(BUILD_DIR)/imageIO.o
	$(CC) $(CFLAGS) $^ -o $(BUILD_DIR)/$@

testThreshold: $(BUILD_DIR)/testThreshold.o $(BUILD_DIR)/threshold.o $(BUILD_DIR)/imageIO.o
	$(CC) $(CFLAGS) $^ -o $(BUILD_DIR)/$@

test: $(BUILD_DIR)/test.o $(BUILD_DIR)/imageIO.o
	$(CC) $(CFLAGS) $^ -o $(BUILD_DIR)/$@

$(BUILD_DIR)/testSobel.o: testSobel.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$@ -c $<

$(BUILD_DIR)/convolution.o: convolution.c
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

$(BUILD_DIR)/detectingEdges.o: detectingEdges.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$@ -c $<

$(BUILD_DIR)/queue.o: queue.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$@ -c $<

$(BUILD_DIR)/vector.o: vector.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$@ -c $<

$(BUILD_DIR)/distanceTransform.o: distanceTransform.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$@ -c $<

$(BUILD_DIR)/filter.o: filter.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$@ -c $<

$(BUILD_DIR)/testFilter.o: testFilter.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$@ -c $<

$(BUILD_DIR)/filterAndEdgeDetect.o: filterAndEdgeDetect.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$@ -c $<

$(BUILD_DIR)/watershed.o: watershed.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$@ -c $<

$(BUILD_DIR)/erosion.o: erosion.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$@ -c $<

$(BUILD_DIR)/PQueue.o: PQueue.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$@ -c $<

$(BUILD_DIR)/watershedFilter.o: watershedFilter.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$@ -c $<

$(BUILD_DIR)/histogram.o: histogram.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$@ -c $<

$(BUILD_DIR)/testHistogramTechniques.o: testHistogramTechniques.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$@ -c $<

$(BUILD_DIR)/testFilterEdgeDetectionErosionAndAdaptiveThresholding.o: testFilterEdgeDetectionErosionAndAdaptiveThresholding.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$@ -c $<

$(BUILD_DIR)/stack.o: stack.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$@ -c $<

$(BUILD_DIR)/regionGrowing.o: regionGrowing.c $(BUILD_DIR)/imageIO.o $(BUILD_DIR)/stack.o
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$@ -c $<

$(BUILD_DIR)/findGate.o: findGate.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$@ -c $<

.PHONY: clean

clean:
	rm -r $(BUILD_DIR)/**
