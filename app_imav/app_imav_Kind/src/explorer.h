/**
  * explorer.h
  * Created on: Aug 3, 2022
  * Author: Igor Pontes Tresolavy
  */

#ifndef __EXPLORER_H__
#define __EXPLORER_H__

#include <stdint.h>
#include <stdbool.h>

#define BUFFERSIZE 8

typedef enum
{
    IDLE, // do nothing
    TAKEOFF,
    HOVER,
    GO_FORWARD,
    FIND_GATE,
    TURN_BACK_TO_ARENA,
    CHANGE_HEADING
} ExplorerState;

typedef struct Point
{
    float x;
    float y;
} CfPosition;

#endif /* __EXPLORER_H__ */

