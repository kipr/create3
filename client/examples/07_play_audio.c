#include <kipr/create3/client/client.h>

#include <stdio.h>

int main()
{
  printf("Connecting to Create3 robot...\n");

  create3_connect_manual("192.168.125.1", 50051);
  
  printf("Connected!\n");

  printf("Twinkle twinkle little star...\n");
  
  // Play a twinkle twinkle little star with notes in Hz and durations in seconds
  Create3AudioNote notes[] = {
    { 261, 0.5 },
    { 261, 0.5 },
    { 392, 0.5 },
    { 392, 0.5 },
    { 440, 0.5 },
    { 440, 0.5 },
    { 392, 1.0 },
    { 349, 0.5 },
    { 349, 0.5 },
    { 330, 0.5 },
    { 330, 0.5 },
    { 294, 0.5 },
    { 294, 0.5 },
    { 261, 1.0 },
    { 392, 0.5 },
    { 392, 0.5 },
    { 349, 0.5 },
    { 349, 0.5 },
    { 330, 0.5 },
    { 330, 0.5 },
    { 294, 1.0 },
    { 392, 0.5 },
    { 392, 0.5 },
    { 349, 0.5 },
    { 349, 0.5 },
    { 330, 0.5 },
    { 330, 0.5 },
    { 294, 1.0 },
    { 261, 0.5 },
    { 261, 0.5 },
    { 392, 0.5 },
    { 392, 0.5 },
    { 440, 0.5 },
    { 440, 0.5 },
    { 392, 1.0 },
    { 349, 0.5 },
    { 349, 0.5 },
    { 330, 0.5 },
    { 330, 0.5 },
    { 294, 0.5 },
    { 294, 0.5 },
    { 261, 1.0 }
  };

  create3_audio_play(notes, sizeof(notes) / sizeof(Create3AudioNote), 1);
  
  // Wait for 1 second
  create3_wait();

  printf("Done!\n");

  return 0;
}