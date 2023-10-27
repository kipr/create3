#include <kipr/create3/client/client.h>

#include <stdio.h>

int main()
{
  printf("Connecting to Create3 robot...\n");

  create3_connect_manual("192.168.125.1", 50051);
  
  printf("Connected!\n");


  printf("Rotate at 0.5 m/s for 1 second...\n");

  // Arc of 90 deg (PI/2 rad) with radius of 0.5 m
  create3_drive_arc_degrees(0.5, 90, 0.306);
  create3_drive_arc_radians(0.5, -1.57, 0.460);

  // Wait for 1 second
  create3_wait();

  printf("Done!\n");

  return 0;
}