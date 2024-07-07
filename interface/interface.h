#ifndef INTERFACE_H
#define INTERFACE_H

#include <net/if.h>
#include <netinet/in.h>
#include <netpacket/packet.h>

#include <string.h>
#include <ifaddrs.h>

typedef struct interface {
	unsigned int		if_index;
	struct sockaddr_in	addr;
	struct sockaddr_in	netmask;
	struct sockaddr_in	broadcast;
	struct sockaddr_ll	hw_addr;
} interface_t;

int	get_interface(const char* if_name, interface_t* interface);

#endif
