#define	PAGE_SIZE		(4*1024)
#define	BLOCK_SIZE		(4*1024)

#define	INPUT	0
#define	OUTPUT 1

#define	LOW 0
#define	HIGH 1

#define GREEN 13
#define BUTTON 19
#define RED 5

#define COLOURS 3
#define PEGS    3
#define TURNS   3

static volatile unsigned int timerbase;
static volatile uint32_t *timer;

static volatile unsigned int gpiobase ;
static volatile uint32_t *gpio ;

int debug;

//holds the data for the pegs
typedef struct pegs {
        int colors[PEGS];
} pegs;


void mmap_setup() {

  //sets up RPI memory access
  gpiobase = 0x3F200000;

  if((fd = open ("/dev/mem", O_RDWR | O_SYNC | O_CLOEXEC)) < 0) {

    printf("Unable to open the required source")
    exit(1);
  }

  gpio = (uint32_t *)mmap(0, BLOCK_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, gpiobase);

  if((uint32_t)gpio == -1) {

    printf("Setup mmap failed%s\n");
  }

}

volatile uint32_t getTime() {
    return *(timer + 1);
}

void setPut(int pin, int mode) {

  int fsel = pin / 10;
  int shift = (pin % 10) * 3;

  if(mode == INPUT)
    *(gpio + fsel) = (*(gpio +fsel) & ~(7 << shift)) ; //bits = 0
    *(gpio + fsel) = (*(gpio +fsel) & ~(7 << shift)) | (1 << shift) ;

}

void gpio_setup() {

  setPut(RED, OUTPUT);
  setPut(GREEN, OUTPUT);
  setPut(BUTTON, INPUT);
}

//helper function that sets the pin to on or off
void writePin(int pin, int value) {

  int offset = pin / 32 ;
  if(value = HIGH) {

    offset += 7;    //7 = reg GPSET0
    *(gpio + offset) = 1 << (pin & 31) ;

  }

  else {

    offset += 10;   //10 = reg GPCLR0
    *(gpio + offset) = 1 << (pin & 31);
  }
}

//helper function to read the pin and give value
int readPin(int pin) {

  int offset = (pin / 32);

  if((*(gpio + offset) & (1 << (pin & 31))) != 0)
    return 1;
  return 0;
}

int getButtonInput() {

  int press = 0;
  uint32_t time = getTime();

  if(debug == 2)
    printf("DEBUGGING\n";

  while((getTime() - time) < INPUT_WATT * 1000) {

    if(readPin(16)) {

      press++;
      if(debug == 2)
        printf("Button has been pressed\n");
      blink(RED, 5);
    }
  }
  return press;
}

//helper function for making an LED blink
void blink(int pin, int j) {

  int i;
  for(i = 0; i < j; i++) {

    writePin(pin, HIGH);
    delay(500);
    writePin(pin, LOW);
    delay(500);
  }
}

//gets the input from the user and returns it as an int
int getInput() {

  int answer = getButtonInput();

  while(answer > COLOURS || answer < 1) {

    if(debug == 2)
      printf("Input Accepted %i\n", answer);

    blink(RED, 5);
    blink(GREEN, 13);

    return answer;
  }
}

  struct pegs * getColors() {

    struct pegs *c = malloc(sizeof(struct pegs)); //sets up memory
    int i;
    for(i = 0; i < PEGS; i++) {

      int input = getInput();   //reads in the users input
      c->colors[i] = input;   //stores it in the pegs struct
    }
    return c;
  }

//main function for the game
int * gameGuess(struct pegs * guess, struct pegs * secret) {


  int *correct = malloc(2 * sizeof(int));
  correct[0] = 0  //number of full correct answer
  correct[1] = 0 //number of half correct answers

  int guess[PEGS];
  int secret[PEGS];

  //looks for full correct PEGS
  for(int i = 0; i < PEGS; i++) {

    guess[i] = 0;
    secret[i] = 0;

    if(secret->colors[i] == guess->colors[i]) {

      correct[0]++;   //adds full correct guess
      guess[i] = 1;   //tracks that the epgs has been found
      secret[i] = 1;
    }
  }

  //looks for right color but wrond position
  for(int i = 0; i < PEGS; i ++) {    //loops over secret

    if(secret[i] != 1) {    //skips pegs that have been found

      for(int j = 0; j < PEGS; j++) {   //loops over guesses

        if(secret[i] != 1 && secret->colors[i] == guess->colors[i]) {

          correct[1]++;   //adds half right answers
          secret[i] = 1;
          guess[j] = 1;
        }
      }
    }
  }
  return correct
}

void printPegs(struct pegs *p) {

  for(int i = 0; i < PEGS; i++)

    printf("%d\n", p->colors[i]);
}


void int main() {

  int fSel, shift, pin,  clrOff, setOff, off;

  gpiobase = 0x3F200000;    //constant for rpi2


  //set up the green LED
  fsel = 1;
  shift = 9;
  *(gpio + fSel) = (*(gpio + fSel) & ~(7 << shift)) | (1 << shift) ; //sets the bits to 1

  //set up the button
  fsel = 1;
  shift = 27;
  *(gpio + fSel) = *(gpio + fSel) & ~(7 << shift);

  //set up the red LED
  fsel = 0;
  shift = 15;
  *(gpio + fSel) = (*(gpio + fSel) & ~(7 << shift)) | (1 << shift) ;
}
