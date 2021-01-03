/* csapp3 */
#include <stdio.h>
typedef struct {
  int a[2];
  double d;
} struct_t;

double fun(int i) {
  volatile struct_t s;
  s.d = 3.14;
  // index i could be out of bound.
  s.a[i] = 1073741824;
  return s.d;
}

int main() {
  int i = 0;
  for (i = 0; i < 10; i++) {
    printf("fun(%d) = %.14lf\n", i, fun(i));
  }
  return 0;
}
