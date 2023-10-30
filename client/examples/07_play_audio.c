#include <kipr/create3/client/client.h>

#include <stdio.h>

int main()
{
  printf("Connecting to Create3 robot...\n");

  create3_connect_manual("192.168.125.1", 50051);
  
  printf("Connected!\n");

  printf("Twinkle twinkle little star...\n");
  
  // Play a twinkle twinkle little star with notes in Hz and durations in seconds
  Create3AudioNote notes[] =
  {
    (Create3AudioNote){ .frequency = 261, .seconds = 0.5 },
    (Create3AudioNote){ .frequency = 261, .seconds = 0.5 },
    (Create3AudioNote){ .frequency = 392, .seconds = 0.5 },
    (Create3AudioNote){ .frequency = 392, .seconds = 0.5 },
    (Create3AudioNote){ .frequency = 440, .seconds = 0.5 },
    (Create3AudioNote){ .frequency = 440, .seconds = 0.5 },
    (Create3AudioNote){ .frequency = 392, .seconds = 1.0 },
    (Create3AudioNote){ .frequency = 349, .seconds = 0.5 },
    (Create3AudioNote){ .frequency = 349, .seconds = 0.5 },
    (Create3AudioNote){ .frequency = 330, .seconds = 0.5 },
    (Create3AudioNote){ .frequency = 330, .seconds = 0.5 },
    (Create3AudioNote){ .frequency = 294, .seconds = 0.5 },
    (Create3AudioNote){ .frequency = 294, .seconds = 0.5 },
    (Create3AudioNote){ .frequency = 261, .seconds = 1.0 },
    (Create3AudioNote){ .frequency = 392, .seconds = 0.5 },
    (Create3AudioNote){ .frequency = 392, .seconds = 0.5 },
    (Create3AudioNote){ .frequency = 349, .seconds = 0.5 },
    (Create3AudioNote){ .frequency = 349, .seconds = 0.5 },
    (Create3AudioNote){ .frequency = 330, .seconds = 0.5 },
    (Create3AudioNote){ .frequency = 330, .seconds = 0.5 },
    (Create3AudioNote){ .frequency = 294, .seconds = 1.0 },
    (Create3AudioNote){ .frequency = 392, .seconds = 0.5 },
    (Create3AudioNote){ .frequency = 392, .seconds = 0.5 },
    (Create3AudioNote){ .frequency = 349, .seconds = 0.5 },
    (Create3AudioNote){ .frequency = 349, .seconds = 0.5 },
    (Create3AudioNote){ .frequency = 330, .seconds = 0.5 },
    (Create3AudioNote){ .frequency = 330, .seconds = 0.5 },
    (Create3AudioNote){ .frequency = 294, .seconds = 1.0 },
    (Create3AudioNote){ .frequency = 261, .seconds = 0.5 },
    (Create3AudioNote){ .frequency = 261, .seconds = 0.5 },
    (Create3AudioNote){ .frequency = 392, .seconds = 0.5 },
    (Create3AudioNote){ .frequency = 392, .seconds = 0.5 },
    (Create3AudioNote){ .frequency = 440, .seconds = 0.5 },
    (Create3AudioNote){ .frequency = 440, .seconds = 0.5 },
    (Create3AudioNote){ .frequency = 392, .seconds = 1.0 },
    (Create3AudioNote){ .frequency = 349, .seconds = 0.5 },
    (Create3AudioNote){ .frequency = 349, .seconds = 0.5 },
    (Create3AudioNote){ .frequency = 330, .seconds = 0.5 },
    (Create3AudioNote){ .frequency = 330, .seconds = 0.5 },
    (Create3AudioNote){ .frequency = 294, .seconds = 0.5 },
    (Create3AudioNote){ .frequency = 294, .seconds = 0.5 },
    (Create3AudioNote){ .frequency = 261, .seconds = 1.0 }
  };

  create3_audio_play(notes, sizeof(notes) / sizeof(Create3AudioNote), 1);
  
  // Wait for 1 second
  create3_wait();

  printf("Done!\n");

  return 0;
}