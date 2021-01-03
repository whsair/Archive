#include <stdio.h>

int main() {

  printf("size of char %zu\n", sizeof(char));
  printf("size of short %zu\n", sizeof(short));
  printf("size of int %zu\n", sizeof(int));
  printf("size of long %zu\n", sizeof(long));
  printf("size of float %zu\n", sizeof(float));
  printf("size of double %zu\n", sizeof(double));
  printf("size of long double %zu\n", sizeof(long double));
  printf("size of pointer(to int) %zu\n", sizeof(int*));
  printf("size of pointer(to char) %zu\n", sizeof(char*));
  printf("size of pointer(to double) %zu\n", sizeof(double*));
  
  return 0;
}
