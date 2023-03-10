const unsigned int WIDTH = 300;
const unsigned int HEIGHT = 300;

unsigned int BUFFER[WIDTH * HEIGHT];

void go() {
  unsigned int screen[300][300];
  int x;
  for (x = 0; x < WIDTH; x++) {
    int y;
    for (y = 0; y < HEIGHT; y++) {
      unsigned int color;
      if (y < HEIGHT / 2) {
        color = 0xff0000ff;  // RGBA little endian
      } else {
        color = 0xffff0000;  // RGBA little endian
      }

      screen[x][y] = color;
      BUFFER[WIDTH * x + y] = screen[x][y];
    }
  }
}
