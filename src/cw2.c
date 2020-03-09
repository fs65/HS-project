#define	PAGE_SIZE		(4*1024)
#define	BLOCK_SIZE		(4*1024)

#define	INPUT	0
#define	OUTPUT 1

#define	LOW 0
#define	HIGH 1

#define GREEN 13
#define BUTTON 19
#define RED 5

static volatile unsigned int gpiobase ;
static volatile uint32_t *gpio ;


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
