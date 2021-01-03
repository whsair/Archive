#include <stdio.h>
#include <string.h>

int main() {
  int a = 0;
  int b = 0;
  char op[3];
  int count = 0;
  printf("Enter an expression of the form a op b:\n");
  printf("With a b as hexadecimal(without 0x) and lower case letters.\n");
  printf("And op is one of (&, |, ^, &&, ||, <<, >>):\n");

  printf("Or Enter an expression of the form op b:\n");
  printf("With b as hexadecimal(without 0x) and lower case letters.\n");
  printf("And op is one of (! ~):\n");
  if ((count = scanf("%x %2s %x", &a, op, &b)) == 3) {
    if (!strcmp(op, "&"))
      printf("0x%x & 0x%x = 0x%x\n", a, b, a&b);
    else if (!strcmp(op, "|"))
      printf("0x%x | 0x%x = 0x%x\n", a, b, a|b);
    else if (!strcmp(op, "^"))
      printf("0x%x ^ 0x%x = 0x%x\n", a, b, a^b);
    else if (!strcmp(op, "&&"))
      printf("0x%x && 0x%x = 0x%x\n", a, b, a&&b);
    else if (!strcmp(op, "||"))
      printf("0x%x || 0x%x = 0x%x\n", a, b, a||b);
    else if (!strcmp(op, "<<"))
      printf("0x%x << 0x%x = 0x%x\n", a, b, a<<b);
    else if (!strcmp(op, ">>"))
      printf("0x%x >> 0x%x = 0x%x\n", a, b, a>>b);
    else
      printf("Error evaluating 0x%x %s 0x%x\n", a, op, b);
  }
  else if ((count = scanf("%2s %x", op, &b)) == 2) {
    if (!strcmp(op, "~"))
      printf("~ 0x%x = 0x%x\n", b, ~b);
    else if (!strcmp(op, "!"))
      printf("! 0x%x = 0x%x\n", b, !b);
    else
      printf("Error evaluating %s 0x%x\n", op, b);
  }
  else {
    printf("Error reading expression.\n");
  }

  return 0;
}

