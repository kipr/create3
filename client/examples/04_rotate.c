#include <kipr/create3/client/client.h>

#include <stdio.h>

int main()
{
  printf("Connecting to Create3...\n");

  create3_connect_manual("192.168.125.1", 50051);
  
  printf("Connected!\n");


  printf("Rotate at 0.5 m/s for 1 second...\n");

  // Move forward at 0.5 m/s for 1 second
  create3_rotate(1.57, 0.5);

  // Wait for 1 second
  create3_wait();

  printf("Done!\n");

  return 0;
}