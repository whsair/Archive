#include <stdio.h>
int main() {
  float x, y, z;
  printf("enter three floating point numbers in the form \"x y z\":\n");
  scanf("%f %f %f", &x, &y, &z);
  if ((x+y)+z == x+(y+z))
    printf("(x+y)+z (%f) == x+(y+z) (%f)\n", (x+y)+z, x+(y+z));
  else
    printf("(x+y)+z (%.10f) != x+(y+z) (%.10f)\n", (x+y)+z, x+(y+z));
  return 0;
}
