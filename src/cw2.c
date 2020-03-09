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

//holds the data for the pegs
typedef struct pegs {
        int colors[PEGS];
} pegs;


void mmap_setup() {

  //sets up RPI memory access
  gpiobase = 0x3F200000;

  if((fd = open ("/dev/mem", O_RDWR, O_SYNC, O_CLOEXEX)) < 0) {

    printf("Unable to open the required source")
    exit(1);
  }

  gpio = (uint32_t *)mmap(0, BLOCK_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fd, gpiobase);

  if((uint32_t)gpio == -1) {

    printf("Setup mmap failed%s\n");
  }

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
