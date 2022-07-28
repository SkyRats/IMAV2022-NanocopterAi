# Detecting Gates

This directory hosts C code intended to enable gate detection by [Bitcraze's AI deck](https://www.bitcraze.io/products/ai-deck/).

Most of the code is experimental.

## Strategy

- Low-pixel filtering (non-convolution)
- Histogram valley thresholding;
    - Acquire histogram;
    - Threshold image at valley between first two peaks.
- Edge detection (convolution);
- Region growing;
- Gate detection;

### Optional

- High-pass filtering (convolutional)
- Erosion after edge detection
- histogram smoothing
- histogram equalization


### Notes

- Avoid multiplication and division
- Avoid floating point arithmetic
- Avoid memory access
- Ideally, each function should be comprised of loops containing a small amount of instructions

## Second option

- Otsu threshold (Igor/Gentil)
- Erosion (Mateus)
- distanceTransform (Igor)
- Watershed (Gentil/Igor/Mateus)
