#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
  if(argc != 2) // error case check first
  {
    printf("usage: page_calc [VIRTUAL ADDRESS NUMBER]\n\n");
    return 1;
  }

  // every var. cannot be negative
  unsigned int address = atoi(argv[1]); // address string to unsigned int
  unsigned int page_num = address / 4096; // location of page number
  unsigned int offset = address % 4096; // remainedr will be an offset

  printf("The address %u contains:\n", address);
  printf("Page Number: %u\n", page_num);
  printf("Offset: %u\n\n", offset);

  return 0;
}
