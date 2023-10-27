#include <kipr/create3/client/Direction.hpp>

#include <cstring>

Create3Direction create3_direction_from_string(const char* str) {
    if (std::strcmp(str, "forward") == 0) {
        return Create3DirectionForward;
    } else if (std::strcmp(str, "backward") == 0) {
        return Create3DirectionBackward;
    } else {
        throw InvalidDirectionException();
    }
}