#ifndef __GATE_FINDER_H__
#define __GATE_FINDER_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "config.h"
#include "imageIO.h"
#include "convolution.h"
#include "morphological.h"
#include "filter.h"
#include "histogram.h"
#include "regionGrowing.h"
#include "queue.h"
#include "findGate.h"


Point GateFinder(PGMImage *image);

#endif
