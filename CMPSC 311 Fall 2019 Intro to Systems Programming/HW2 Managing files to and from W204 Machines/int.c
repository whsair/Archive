#include <stdio.h>
int main() {
  int x;
  printf("Enter an integer value:\n");
  scanf("%i", &x);
  if (x*x >= 0)
    printf("x*x = %d >= 0\n", x*x);
  else
    printf("x*x = %d < 0\n", x*x);
  return 0;
}
