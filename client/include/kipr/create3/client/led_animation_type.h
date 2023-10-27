#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
  Create3BlinkLights = 1,
  Create3SpinLights = 2,
  Create3InvalidType,
} Create3LedAnimationType;

Create3LedAnimationType create3_led_animation_type_from_string(const char* str);

#ifdef __cplusplus
}
#endif