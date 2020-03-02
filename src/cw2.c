
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
