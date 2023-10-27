#include <kipr/create3/client/client.h>

#include <stdio.h>

int main()
{
  printf("Connecting to Create3 robot...\n");

  create3_connect_manual("192.168.125.1", 50051);
  
  printf("Connected!\n");


  printf("Rotate at 0.5 m/s for 1 second...\n");

  // Rotate at 90 deg/s (PI/2 rad/s) to 90 degrees (PI/2 radians)
  create3_rotate_degrees(90,90);
  create3_rotate_radians(1.57, 1.57);

  // Wait for 1 second
  create3_wait();

  printf("Done!\n");

  return 0;
}