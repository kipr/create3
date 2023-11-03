#include <kipr/create3/client/BumpSensor.hpp>

#include <cstring>
#include <string>
#include <map>

std::map<int, std::string> create3_bump_sensor_string_map = {
    {Create3BumpSensorLeft, "bump_left"},
    {Create3BumpSensorFrontLeft, "bump_front_left"},
    {Create3BumpSensorFrontCenter, "bump_front_center"},
    {Create3BumpSensorFrontRight, "bump_front_right"},
    {Create3BumpSensorRight, "bump_right"}
};

int create3_bump_sensor_to_string(int sensor, const char* sensor_string) {
  if(std::strstr(sensor_string, create3_bump_sensor_string_map[sensor].c_str()) != nullptr) {
    return 1;
  }
  return 0;
}
