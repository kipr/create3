#include <kipr/create3/client/BumpSensor.hpp>

#include <cstring>

Create3BumpSensor create3_bump_sensor_from_string(const char* str) {
    if (std::strcmp(str, "bump_left") == 0) {
        return Create3BumpSensorLeft;
    } else if (std::strcmp(str,"bump_front_left") == 0) {
        return Create3BumpSensorFrontLeft;
    } else if (std::strcmp(str, "bump_front_right") == 0) {
        return Create3BumpSensorFrontRight;
    } else if (std::strcmp(str, "bumpright") == 0) {
        return Create3BumpSensorRight;
    } else {
        throw InvalidBumpSensorException();
    }
}