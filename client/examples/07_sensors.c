#include <kipr/create3/client/client.h>

#include <stdio.h>

int main()
{
  printf("Connecting to Create3 robot...\n");

  create3_connect_manual("192.168.125.1", 50051);
  
  printf("Connected!\n");

  printf("Sensor Values:\n");
  printf("  Left Side Cliff: %d\n", create3_sensor_cliff(0));
  printf("  Left Front Cliff: %d\n", create3_sensor_cliff(1));
  printf("  Right Front Cliff: %d\n", create3_sensor_cliff(2));
  printf("  Right Side Cliff: %d\n", create3_sensor_cliff(3));
  printf("\n");
  printf("  Left Side IR: %d\n", create3_sensor_ir(0));
  printf("  Left IR: %d\n", create3_sensor_ir(1));
  printf("  Left Front IR: %d\n", create3_sensor_ir(2));
  printf("  Left Center IR: %d\n", create3_sensor_ir(3));
  printf("  Right Center IR: %d\n", create3_sensor_ir(4));
  printf("  Right Front IR: %d\n", create3_sensor_ir(5));
  printf("  Right IR: %d\n", create3_sensor_ir(6));

  create3_wait();

  printf("Done!\n");

  return 0;
}