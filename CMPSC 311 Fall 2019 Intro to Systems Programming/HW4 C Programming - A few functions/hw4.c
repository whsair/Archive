// Author: Hongshuo Wang
// Email: hkw5146@psu.edu
#include <stdlib.h>
#include <string.h>
// rotate the values pointed to by three pointers
// so values move from xp to yp, yp to zp and zp to x

// creat a temporary integer variable to store value y which is *yp since xp yp zp are pointer type variables
// *yp mean the value of variable that the pointer yp pointed
// assign the value of x to y, and assign the value of z to x
// Finally assign temp's value which is the value of y to z
void rotate(int *xp, int *yp, int *zp) {
  int temp =*yp;
  *yp = *xp;
  *xp = *zp;
  *zp = temp;
}

// Write a function that returns 0 if x is 0, returns -1 
// if x < 0, returns 1 if x > 0
// Your code must follow the Bit-Level Integer Coding Rules
// on the textbook (located between hw 2.60 and 2.61).
// You can assume w = 32. 
// The only allowed operations in your code are:
// ! ~ & ^ | + << >>
// This requirement is more restrictive than the coding rules. 


// since positive or zero number will lead with 0, and negative number will lead with 1 in 2's complement
// just right shift 31 bits in order to get their first digit
// In sign casting or right shifts, repeating the sign digit to take over the shifted bits.
// in this case, negative number will get -1 and positive number or zero number will get 0 by shifting 31 bits
// then we use !!(x) to decide whether the value is 0
// !(x) return 1 if the value is 0, return 0 if the value is not zero
// !!(x)return 1 if the value is not 0, return 0 if the value is zero
// then we just use or operator since the positive number will get 0 or 1 = 1 and zero number will get 0 or 0 = 0
// the negative number will get 0xffffffff or 0x0 = -1 
int sign(int x) {
  return (x >> 31) | (!(!x));
}

// Given a source string, and a destination string, write the
// reversed source string into destination. Make sure you terminate
// the reversed string properly with an integer value 0.
// You may assume that the destination array is big enough to hold
// the reversed string. 
// 
// initial destination char array and define its length to be the length of sourse array + 1. since we will have a 0 to 
// terminate our array
// use a for loop to assignment the last char which is the last element in source array to the head of destination array 
// util we assigned the first elements of source array to the second last elment of destination array then append the last 0 to terminate the string
void reverse(const char source[], char destination[]) {
  int i;
  int len = strlen(source);
  
  for (i = 0; i < len; i++) {
    destination[i] = source[len - 1 - i];
  }
  destination[len] = '\0';
  return; 
}
