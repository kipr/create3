#include <kipr/create3/client/LedAnimationType.hpp>

#include <cstring>

Create3LedAnimationType create3_led_animation_type_from_string(const char* str) {
    if (std::strcmp(str, "blink") == 0) {
        return Create3BlinkLights;
    } else if (std::strcmp(str, "spin") == 0) {
        return Create3SpinLights;
    } else {
        throw InvalidLedAnimationTypeException();
    }
}