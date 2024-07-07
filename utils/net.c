#include "utils.h"

uint32_t get_subnet_size(struct sockaddr_in* netmask) {
	uint8_t bits = 0;
	for(uint32_t addr = *(uint32_t*)&netmask->sin_addr; addr & 1; addr >>= 1, bits++) {}
	return ((uint32_t)1 << (32 - bits)) - 2;
}
