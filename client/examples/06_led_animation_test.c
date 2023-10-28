#include <kipr/create3/client/client.h>

#include <stdio.h>

int main()
{
  printf("Connecting to Create3 robot...\n");

  create3_connect_manual("192.168.125.1", 50051);
  
  printf("Connected!\n");

  printf("Blinking lights...\n");
  // Blink the lights green for 5 seconds.
  
  Create3LedColor green;
  green.r = 0;
  green.g = 255;
  green.b = 0;

  Create3Lightring lightring;
  lightring.led0 = green;
  lightring.led1 = green;
  lightring.led2 = green;
  lightring.led3 = green;
  lightring.led4 = green;
  lightring.led5 = green;

  create3_led_animation(Create3BlinkLights, lightring, 5.0); // Blink the lights green for 5 seconds.
  
  // Wait for 1 second
  create3_wait();

  printf("Done!\n");

  return 0;
}