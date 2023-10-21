#include <kipr/create3/client/client.h>

#include <stdio.h>

int main()
{
  printf("Connecting to Create3...\n");

  create3_connect();
  
  printf("Connected!\n");


  printf("Moving forward at 0.5 m/s for 1 second...\n");
  // Move forward at 0.5 m/s for 1 second
  create3_velocity_set_components(0.5, 0.0);

  // Wait for 1 second
  usleep(1000000);

  // Stop
  create3_velocity_set_components(0.0, 0.0);

  printf("Done!\n");

  return 0;
}