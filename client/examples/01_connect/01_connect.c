#include <kipr/create3/client/client.h>

#include <stdio.h>

int main()
{
  printf("Connecting to Create3...\n");
  create3_connect();
  printf("Connected!\n");

  return 0;
}