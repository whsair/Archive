#include <stdio.h>
#include <limits.h>
#include <string.h>
#include "hw4.h"
typedef struct {
  int x;
  int y;
  int z;
} Trio;
void testingRotate() {
  Trio sources[] = {{4,5,6},{-1, 1, -1},{1,2,3}, {-100, 0, 100}, {1, 1, 1}};
  const Trio expected[] = {{6, 4, 5}, {-1, -1, 1}, {3, 1, 2}, {100, -100, 0}, {1, 1, 1}};
  int i;
  for (i = 0; i < sizeof(sources)/sizeof(Trio); i ++) {
    printf("Before rotate: %d, %d, %d\n", sources[i].x, sources[i].y, sources[i].z);
    rotate(&(sources[i].x), &(sources[i].y), &(sources[i].z));
    printf("After rotate: %d, %d, %d\n", sources[i].x, sources[i].y, sources[i].z);
    if (sources[i].x == expected[i].x
        && sources[i].y == expected[i].y
        && sources[i].z == expected[i].z) {
      printf("----pass----\n");
    }
    else {
      printf("----fail----\n");
    }
  }
}
  
void testingReverse() {
  const char *sources[] = {
    "abcdefghyjklmn",
    " 1 3 5 2 1  ",
    "c_b",
    "Welcome to CMPSC 311!",
    "abcdef",
    "_ ab  cd  ef *",
    ""
  };
  const char *expected[] = {
    "nmlkjyhgfedcba",
    "  1 2 5 3 1 ",
    "b_c",
    "!113 CSPMC ot emocleW",
    "fedcba",
    "* fe  dc  ba _",
    ""
  };
  char destination[50];
  int i;
  for (i = 0; i < sizeof(sources)/sizeof(char*); i++) {
    printf("source: \"%s\"\n", sources[i]);
    // these two lines is to set up destination
    // to make it easy to detect when students didn't
    // add terminating null character.
    memset(destination, 'x', 49);
    destination[49] = 0;
    reverse(sources[i], destination);
    printf("destination: \"%.49s\"\n", destination);
    if (strcmp(destination, expected[i]) == 0) {
      printf("----pass----\n");
    }
    else {
      printf("----fail----\n");
    }
  }
}

int mySign(int x) {
  if (x > 0)
    return 1;
  else if (x == 0)
    return 0;
  else
    return -1;
}

void testingSign() {
  int x[] = { 0, -1, 1, INT_MIN, INT_MAX, 0x0000ffff, 0xffff0000, -2, 3, 0x1fffffff, 0xcfffffff, -10, 0xfffabfff, 0xfffaa, 15, -15  };
  int i;
  int signCorrect;
  int signStudent;
  for (i = 0; i < sizeof(x)/sizeof(int); i++) {
    printf("x = %d\n", x[i]);
    signCorrect = mySign(x[i]);
    signStudent = sign(x[i]);
    printf("sign = %d\n", signStudent);
    if (signCorrect == signStudent) {
      printf("----pass----\n");
    }
    else {
      printf("----fail----\n");
    }
  }
  return;
}


int main() {
  printf("Testing rotate:\n");
  testingRotate();
  printf("====================\n");
  printf("Testing reverse:\n");
  testingReverse();
  printf("====================\n");
  printf("Testing sign:\n");
  testingSign();
  return 0;
}
