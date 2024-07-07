#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>
#include <netinet/in.h>

uint32_t get_subnet_size(struct sockaddr_in* netmask);

#endif
