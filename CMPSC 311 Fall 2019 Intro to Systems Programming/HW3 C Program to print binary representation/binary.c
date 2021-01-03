#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int main(int argc, char *argv[]) {
  unsigned long int i;
  // checking there is a command line argument
  if (argc != 2) {
    printf("Usage: %s number\n", argv[0]);
    return 0;
  }
  // converting the command line argument to a unsigned long
  // and exit program if there is an error during the conversion.
  errno = 0;
  i = strtoul(argv[1], NULL, 0);
  if (errno != 0) {
    perror("Failed to convert number");
    return 0; 
  }
  // write your solutions below this line
  // define a result array in order to store each digit of input number
  unsigned int result[64];

  //According to our lecture, recursively dividing input decimal number by 2 to get each digit
  //each division get only one digit starting from lower to higher until i <= 2
  int index = 0;
  while (i % 2 != i) {
    result[index] = i%2;
    i = i/2;
    index++;
  }

  //get the last one digit which is the reminder of gcd(i,2)
  result[index] = i%2;

  //start from the last index, revsersly print out our stored digits 
  //print out each digit 
  int e;
  for (e = index; e >= 0; e--)  {
    printf("%u", result[e]);
  
  }
  printf("\n");
  // write your solutions above this line
  return 0;
}
