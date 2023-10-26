#pragma once

#include <stdint.h>

#include "led_color.h"

#ifdef __cplusplus
extern "C" {
#endif

// The Create 3's light ring is a ring of 6 RGB LEDs on the top of the robot that can be controlled individually within an array of LedColor structs.
// The LEDs are numbered 0-5, starting at the front of the robot and going clockwise.

typedef struct {
  Create3LedColor led0;
  Create3LedColor led1;
  Create3LedColor led2;
  Create3LedColor led3;
  Create3LedColor led4;
  Create3LedColor led5;
} Create3Lightring;

#ifdef __cplusplus
}
#endif