#include <kipr/create3/client/client.h>

#include <stdio.h>

int main()
{
  printf("Connecting to Create3 robot...\n");
  create3_connect_manual("192.168.125.1", 50051);
  printf("Connected!\n");

  return 0;
}