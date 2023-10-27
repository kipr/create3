#include <kipr/create3/client/duration.h>

Create3Duration create3_duration_from_double(double seconds) {
    Create3Duration duration;
    duration.seconds = (int32_t)seconds;
    duration.nanoseconds = (uint32_t)((seconds - duration.seconds) * 1e9);
    return duration;
}