/* 
 * CS:APP Data Lab 
 * 
 * Author: Hongshuo Wang
 * Email: hkw5146@psu.edu
 * 
 * bits.c - Source file with your solutions to the Lab.
 *          This is the file you will hand in to your instructor.
 *
 * WARNING: Do not include the <stdio.h> header; it confuses the dlc
 * compiler. You can still use printf for debugging without including
 * <stdio.h>, although you might get a compiler warning. In general,
 * it's not good practice to ignore compiler warnings, but in this
 * case it's OK.  
 */

#if 0
/*
 * Instructions to Students:
 *
 * STEP 1: Read the following instructions carefully.
 */

You will provide your solution to the Data Lab by
editing the collection of functions in this source file.

INTEGER CODING RULES:
 
  Replace the "return" statement in each function with one
  or more lines of C code that implements the function. Your code 
  must conform to the following style:
 
  int Funct(arg1, arg2, ...) {
      /* brief description of how your implementation works */
      int var1 = Expr1;
      ...
      int varM = ExprM;

      varJ = ExprJ;
      ...
      varN = ExprN;
      return ExprR;
  }

  Each "Expr" is an expression using ONLY the following:
  1. Integer constants 0 through 255 (0xFF), inclusive. You are
      not allowed to use big constants such as 0xffffffff.
  2. Function arguments and local variables (no global variables).
  3. Unary integer operations ! ~
  4. Binary integer operations & ^ | + << >>
    
  Some of the problems restrict the set of allowed operators even further.
  Each "Expr" may consist of multiple operators. You are not restricted to
  one operator per line.

  You are expressly forbidden to:
  1. Use any control constructs such as if, do, while, for, switch, etc.
  2. Define or use any macros.
  3. Define any additional functions in this file.
  4. Call any functions.
  5. Use any other operations, such as &&, ||, -, or ?:
  6. Use any form of casting.
  7. Use any data type other than int.  This implies that you
     cannot use arrays, structs, or unions.

 
  You may assume that your machine:
  1. Uses 2s complement, 32-bit representations of integers.
  2. Performs right shifts arithmetically.
  3. Has unpredictable behavior when shifting if the shift amount
     is less than 0 or greater than 31.


EXAMPLES OF ACCEPTABLE CODING STYLE:
  /*
   * pow2plus1 - returns 2^x + 1, where 0 <= x <= 31
   */
  int pow2plus1(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     return (1 << x) + 1;
  }

  /*
   * pow2plus4 - returns 2^x + 4, where 0 <= x <= 31
   */
  int pow2plus4(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     int result = (1 << x);
     result += 4;
     return result;
  }

FLOATING POINT CODING RULES

For the problems that require you to implement floating-point operations,
the coding rules are less strict.  You are allowed to use looping and
conditional control.  You are allowed to use both ints and unsigneds.
You can use arbitrary integer and unsigned constants. You can use any arithmetic,
logical, or comparison operations on int or unsigned data.

You are expressly forbidden to:
  1. Define or use any macros.
  2. Define any additional functions in this file.
  3. Call any functions.
  4. Use any form of casting.
  5. Use any data type other than int or unsigned.  This means that you
     cannot use arrays, structs, or unions.
  6. Use any floating point data types, operations, or constants.


NOTES:
  1. Use the dlc (data lab checker) compiler (described in the handout) to 
     check the legality of your solutions.
  2. Each function has a maximum number of operations (integer, logical,
     or comparison) that you are allowed to use for your implementation
     of the function.  The max operator count is checked by dlc.
     Note that assignment ('=') is not counted; you may use as many of
     these as you want without penalty.
  3. Use the btest test harness to check your functions for correctness.
  4. Use the BDD checker to formally verify your functions
  5. The maximum number of ops for each function is given in the
     header comment for each function. If there are any inconsistencies 
     between the maximum ops in the writeup and in this file, consider
     this file the authoritative source.

/*
 * STEP 2: Modify the following functions according the coding rules.
 * 
 *   IMPORTANT. TO AVOID GRADING SURPRISES:
 *   1. Use the dlc compiler to check that your solutions conform
 *      to the coding rules.
 *   2. Use the BDD checker to formally verify that your solutions produce 
 *      the correct answers.
 */


#endif
/* Copyright (C) 1991-2012 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */
/* This header is separate from features.h so that the compiler can
   include it implicitly at the start of every compilation.  It must
   not itself include <features.h> or any other header that includes
   <features.h> because the implicit include comes before any feature
   test macros that may be defined in a source file before it first
   explicitly includes a system header.  GCC knows the name of this
   header in order to preinclude it.  */
/* We do support the IEC 559 math functionality, real and complex.  */
/* wchar_t uses ISO/IEC 10646 (2nd ed., published 2011-03-15) /
   Unicode 6.0.  */
/* We do not support C11 <threads.h>.  */
/* 
 * allEvenBits - return 1 if all even-numbered bits in word set to 1
 *   where bits are numbered from 0 (least significant) to 31 (most significant)
 *   Examples allEvenBits(0xFFFFFFFE) = 0, allEvenBits(0x55555555) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 2
 *
 *   First, we need to know the min number that all even-number
 *   bist in word set to 1 is 0xAAAAAAAA
 *   Since we can only create 8 bits number
 *   Start from 0xAA by shifting 8 bits left and add itself
 *   we can get 0xAAAA
 *   Next just repeat the step, shifting 16 bits and use or to combine them
 *   Now we have the number we need
 *   Next we use not operation to get its complement which is
 *   0x555555555. The reason we got this is
 *   we just to use it with the input x by using or operation.
 *   If the input matched the min all even-bits format. we will
 *   get the -1 which is all one.
 *   If the input doesn't match the format which mean some even bits
 *   will be one. That also means our result will be not all zero.
 *   For example, 111110111111...1111. Then we just need to all one 
 *   to check whether our output is -1.
 *   Finally, since our output need to be one if we match the format
 *   but -1 + 1 = 0. We just need to use ! operation which implies
 *   if input is 0 return 1 and if input is not zero return 0.
 *
 */
int allEvenBits(int x) {
  int minEvenRightHalf = (85 << 8) + 85;
  int minEvenLeftHalf = minEvenRightHalf << 16;
  int minEven = minEvenLeftHalf | minEvenRightHalf;
  int output = !(((x) | (~minEven))+ 1);
  
  return output;
}
/*
 * bang - Compute !x without using !
 *   Examples: bang(3) = 0, bang(0) = 1
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 4
 *
 *
 *First, we need to know the most important formula
 * which I learn in cmpsc 331.
 * -x = ~x + 1 even work on 0 since -0 = 0
 * after we get -x, we can use or opeartion to force
 * the sign bit to be 1 since
 *  negative | positive = negative
 *  positive | negative = negative 
 *  Except 0 | 0 = 0.
 *  Next = we can shiftright 31 bits to get 0xffffffff
 *  number != 0 >> 31 => 0xffffffff 
 *  0 >> 31 = 0
 *  Then we use xor to decide whether the last digit is 1
 *  If it is we return 0 else we return 1.
 *  Finally use & 1 to clear every bits except the last bit.
 *  
 */

int bang(int x) {
  int negat = ~x + 1; 
  int output = negat | x;
  
  return  ((output >> 31) ^ 1) & 1;        
}
/* 
 * floatIsEqual - Compute f == g for floating point arguments f and g.
 *   Both the arguments are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representations of
 *   single-precision floating point values.
 *   If either argument is NaN, return 0.
 *   +0 and -0 are considered equal.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 25
 *   Rating: 2
 *
 *
 *   This project we are dealing with the unsigned floatpoint
 *   We just need to focus on the paricular case, otherwise the == will
 *   perfectly work
 *   infinite number which is 0x7f800000
 *   nanCase number which is xxxffxxx any number who is large than infinite bits pattern
 *   -0 and +0
 *
 *   First, to deal with +-0 just get rid of their sign bits
 *   we can get amountF and amountG
 *   Then do the ! to check whether they are equal at the same time
 *   if it is return 1
 *   Next we need to check the infinite number which is 0x7f800000
 *   use == to detect it, if it is true then return 1
 *   Finally, we need to detect all the nane case which is the number
 *   1. exponent is 0xff
 *   2. isn't infinite number that means the signbits and fraction bits are not zero
 *   if any number satisfied both 1 and 2
 *   just return 0
 *   now we can use uf == ug to deal with the other normal number 
 */
int floatIsEqual(unsigned uf, unsigned ug) {
  unsigned amountF = uf & 0x7fffffff;
  unsigned amountG = ug & 0x7fffffff;

   
  if (!(amountF) && !(amountG)) {
    return 1;
  }

  if ((uf == 0x7f800000) && (ug == 0x7f800000)) {
    return 1;
  }

  if ((((amountF << 9) != 0x0) && ((amountF >> 23) == 0xFF)) ||
    (((amountG << 9) != 0x0) && ((amountG >> 23) == 0xFF))) {
    return 0;
  }
   

  return (uf == ug); 
}
/* 
 * floatUnsigned2Float - Return bit-level equivalent of expression (float) u
 *   Result is returned as unsigned int, but
 *   it is to be interpreted as the bit-level representation of a
 *   single-precision floating point values.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4:
 */
unsigned floatUnsigned2Float(unsigned u) {
  
  //do preperation to get each part of float point we need to calculate our result
  //1. exponent 2. fraction part 3. signbit 4. bias 
  // I get exponent by repeated divide the input number by 2
  // this equation I get from my homework 3
  unsigned fraction = 0;
  unsigned bias = 0;
  unsigned upperFifteenBits = 0;
  unsigned lowerEightBits = 0;
  unsigned output = 0;
  unsigned t = u;
  unsigned exponent = 0;
  unsigned upperBitsMask = 0;
  while (t % 2 != t) {
    t = t/2; 
    exponent = exponent + 1;
  }
   

  // bias is always 127; 
  bias = 0x7F; 
  
  // get the fratction part since
  fraction = u;
  fraction = fraction << (31 - exponent);
  //now we will face the tricky part round out number
  //
  // (using the even round) by the lecture we said
  // we have to focus on the last 8 bit of operation
  // and determine whether the upper 15 bits is round up
  
  upperBitsMask = 0xFFFFFF00; 
  upperFifteenBits = fraction & upperBitsMask; 
  lowerEightBits = fraction & 0xFF;
  
  //now start check our last 8 bits
  // since even round depends on the lower eights bits
  // If those eight bits less than or greater than 0x80 which is 0b10000000
  // Than we just need to do the normal round
  // The tricky part is when our last eight digits counter this suitation
  // those = 0b10000000
  // we need to check the 8 th bit which the number of 9 bits in the number
  // If it is equal 1 than we need to round up to force them become even
  // if it is not equal 1 than we just keep the upper fifteen bits which also
  // say rounded down.
  // By the way the round up also can cause the upgrade exponent problem
  // which is the the upper fifteen number is all 1
  // Now we plus 1 to the rest of our bits will become all zero then we need
  // our exponent by one to get the right answer.
  //
  if ((lowerEightBits > 0x80) || ((fraction & 0x100) && (lowerEightBits == 0x80))) {

    fraction = upperFifteenBits + 0x100;
    if (fraction == 0) {
      exponent = exponent + 1;
    }

  } else if ((lowerEightBits < 0x80) || (((fraction & 0x100) == 0x0) && (lowerEightBits == 0x80))) {
    
    fraction = upperFifteenBits;  

  }
   

  // the finally step is to combine everything
  //denormalize exponent and put the exponent to the right place 
  exponent = (exponent + bias) << 23;


  //put the fraction to the right place get rid of the 1.xxx the leading 1 bits and 
  fraction = fraction << 1 >> 1; 
  fraction = fraction >> 8;
  
  //if input u is zero we should return zero
  if (u == 0) {
    return 0;
  }
  
  //since our input float point is always positive, the signbit is always zero
  output = fraction | exponent;  
  return output;
}
/* 
 * isLess - if x < y  then return 1, else return 0 
 *   Example: isLess(4,5) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 24
 *   Rating: 3
 *
 *   this is the tricky part
 *   there are only two case we should return 1
 *
 *   one is x and y have the same sign but x -y is negatiie
 *   anther is x < 0 and y >=0
 *
 *   The x < y works normally if x and y have the same
 *   sign bits otherwise it make happend overflow
 *   If x and y have the different bits
 *   the x's signbits is the finally result 
 *   since negative - positive have to negative
 *   positive - negative should be positive
 *   Now we need to detect which situation our x < y return 1
 *   1. x < y and x y have the same bit
 *   2. x < y and x y doesn't have the same bit
 *   but x is negative, y is positive
 *   otherwise, it should be return all 0
 */
int isLess(int x, int y) {
  
  //we need -x = ~x + 1 to do the abstraction
  int negaX = ~x + 1; 

  //create the signDetector to detector whether they have the same bit
  int signDetectorY = (y >> 31) & 1;
  int signDetectorX = (x >>31) & 1;
  int signDetector = signDetectorX ^ signDetectorY; 

  // do the abstraction and get its sign
  int abstraction =  negaX + y;
  int abstractionSign = (abstraction >> 31) & 1;  

  //detect the suitation that whether abstraction and x y bits
  //if x y have the same bits and abstraction is negative which is zero
  //then we should return 1
  //if x y doesn't have the same bits, we need to check the x's bit
  //no matter what the soluation is.
  //let x y have same bits and abstraction is negative become 1
  //otherwise return zero
  int truthDetector = !(signDetector | abstractionSign);

  //check the x bits and x any y bits
  //if x is negative  and x and y are not same  we should return 1
  //otherwise we need to return 0
  int overflowDetector = signDetector & signDetectorX;

  //this is the zero detector just like we did in hw4
  //if it's zero return 0
  //if it's nonzero return 1
  //use & to mask our finally result
  int zeroDetector = !!(abstraction); 
  

  // just combine everything together
  // 2 case use the or operation to combine since they only return 1
  // if the output is 1 otherwise they both return zero
  int output = (truthDetector | overflowDetector) & zeroDetector;
   
  
  return output;
}

/* 
 * isNonNegative - return 1 if x >= 0, return 0 otherwise 
 *   Example: isNonNegative(-1) = 0.  isNonNegative(0) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 6
 *   Rating: 2
 *
 *   like I did in previous question,
 *   First we get the signbit.
 *   If x is negative, then x >> 31 will get -1.
 *   Else we will get 0.
 *   Finally we add 1 to force -1 one to 0 and 
 *   no negative number return 1
 */
int isNonNegative(int x) {
  int signDetector = x>>31;
  return signDetector + 1; 
}
/*
 * isTmax - returns 1 if x is the maximum, two's complement number,
 *     and 0 otherwise 
 *   Legal ops: ! ~ & ^ | +
 *   Max ops: 10
 *   Rating: 1
 *
 *   by Hint, we need to make most of complement x which is max negative
 *   number in 32 bits, 0x80000000
 *   Next, we make most of the overflow:
 *   if x = 0x80000000, then x + 1  = min = ~x
 *   Else x + 1  != ~x;
 *   Except x = -1 then x + 1 = 0 = ~x
 *   We need to detect whether the x is -1 or maximum.
 *   If x = -1, then ~x = 0.
 *   !!x will return 0 if x = 0
 *   else it will return 1
 *   Then x ( no -1) will plus one.
 *   And the only suitation that we get x + 1 = ~x will be x is mamimum.
 *   Next we use a xor to detect whether it is ~x.
 *   If it is ~x then return 0 else return 1.
 *   using ! to determint whether it is a zero
 */
int isTmax(int x) {
  int complement = ~x;
  return ! ((x + !!(~x)) ^ complement);
}
/* 
 * rotateLeft - Rotate x to the left by n
 *   Can assume that 0 <= n <= 31
 *   Examples: rotateLeft(0x87654321,4) = 0x76543218
 *   Legal ops: ~ & ^ | + << >> !
 *   Max ops: 25
 *   Rating: 3 
 *   
 *   First, creat a mask which is deal with the negative number since
 *   its right shift is created lots of 1
 *   This mark is for movedPart which is
 *   got 32 - n zero combined with n one
 *   Then leftshift the restPart to
 *   create the empty space for the movedpart
 *   at the end of the binary string
 *   mask the movedPart to get pure number which we want to converted
 *   Finally. combine the restPart and movedPart by using or opearation
 *   since restPart have already created some zeros at the end.
 */
int rotateLeft(int x, int n) {  
  int mask = ~(~(0x0) << n);   
  int restPart = x << n;
  int movedPart = mask & (x >> (32 + (~n + 1)));
  int output = movedPart | restPart;
  return output;
}
/* 
 * signMag2TwosComp - Convert from sign-magnitude to two's complement
 *   where the MSB is the sign bit
 *   Example: signMag2TwosComp(0x80000005) = -5.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 4
 *
 *   The key is to take the amount which is the binary set delete
 *   the sign bits
 *   Then get the abosulte value of it.
 *  Finally add its signbits 
 *   First, we create a signMask to get the amount of value
 *   by create a 0x80000000 than xor it
 *   Now we need to covert our value to 2's complement format
 *   If our value is positive, it's nothing to do it
 *   We focus on the negative. We need to create a mask
 *   that get the absulte value then add it's signbit
 *   Actually, I combine the steps together.
 *   Seem our value always got zero at first bits
 *   Then if we xor it with -1 we will get first number 
 *   convert to one then therest number will become  its value number.
 *   Those two step allow as get the complement number of the amount
 *   Now we just add 1 to amount's negation. If number is positive 
 *   the end part will add 0 which deose not change our result*   
 */
int signMag2TwosComp(int x) {
  int absMask = x >> 31;
  int signMask = absMask << 31; 
  int amount =  x ^ signMask;
  int absAmount = (amount ^ absMask) + !(absMask+1); 
  return absAmount;
}
/* 
 * thirdBits - return word with every third bit (starting from the LSB) set to 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 8
 *   Rating: 1
 *
 *   the key is create 01001001001001001001001001001001
 *   Since we only can create 8 bits binary number,
 *   we need to copy our number 2 and combine our number
 *   to 32 bits
 *   First, creat 0x01001001 = 0d73
 *   then we copy and combine it by shift 9 bits left and add them up
 *   Now, we will get 1001001001001001
 *   Next we just repest the step by shifting 18 bits left and and them up
 *   Finally, we will get the number above.
 */
int thirdBits(void) {
  int a = 73;
  int b = a << 9;
  int c = a + b; 
  return  c + (c << 18); 
}
