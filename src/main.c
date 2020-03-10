#include "cw2.c"

int int main(int argc, char const *argv[]) {
  /* code */

  mmap_setup();
  gpio_setup();

  printf("\nBeginning Game\n");

  int start = game();

  if(start) {

    printf("You Win!!\n");

    blink(GREEN);


  }

  else {

    printf("You Lost!\n");
  }
  return 0;
}
