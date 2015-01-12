#include <inttypes.h>
#include <stdio.h>

uint16_t decode(uint8_t* buf) {
  return buf[0] | (buf[1] << 8);
}

int main() {
  uint8_t buf[2] = {0xef, 0xbe};
  uint16_t x = decode(buf);
  printf("%" PRIx16 " from little endian: %" PRIx8 " %" PRIx8 "\n",
         x, buf[0], buf[1]);
}