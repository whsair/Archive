#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned char *byte_pointer;

void show_bytes(byte_pointer start, size_t len) {
  size_t i;
  for (i = 0; i < len; i++)
    printf("%p\t0x%.2x\n", &start[i], start[i]); 
  printf("\n");
}

void test_show_bytes(int val) {
  int ival = val;
  float fval = (float) ival;
  int *pval = &ival;
  printf("Stack variable ival = %d\n", ival);
  printf("(int)ival:\n");
  show_bytes((byte_pointer)&ival, sizeof(int));
  printf("(float)ival:\n");
  show_bytes((byte_pointer)&fval, sizeof(float));
  printf("&ival:\n");
  show_bytes((byte_pointer)&pval, sizeof(int *));
}

int main(int argc, char *argv[])
{
  int val = 15213;

  if (argc > 1) {
    val = strtol(argv[1], NULL, 0);
    printf("calling test_show_bytes\n");
    test_show_bytes(val);
  } else {
    printf("Usage: %s number\n", argv[0]);
  }
  return 0;
}
