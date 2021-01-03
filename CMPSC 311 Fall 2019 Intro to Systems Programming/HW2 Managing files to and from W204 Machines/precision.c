#include <stdio.h>
int main() {
  double x;
  int precision;
  printf("Enter a number to store in double:");
  scanf("%lf", &x);
  printf("Enter the number of digits to print after decimal:");
  scanf("%d", &precision);
  printf("x = %.*lf\n", precision, x);
  return 0;
}
