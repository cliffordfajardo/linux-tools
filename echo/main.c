#include <stdio.h>

// echo - display a line of text

int main(int argc, char** argv) {
  for (int i = 1; i < argc; i++) {
    printf("%s ", argv[i]);
  }
  printf("\n");
  return 0;
}
