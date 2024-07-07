#ifndef ARP_PROTO_H
#define ARP_PROTO_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <netinet/in.h>
#include <linux/if_ether.h>
#include <netpacket/packet.h>

typedef struct __attribute__((packed)) arp_header {
	uint16_t		htype;
	uint16_t		ptype;

	struct {
		uint16_t	hlen : 8;
		uint16_t	plen : 8;
	};

	uint16_t		oper;

	uint8_t			sha[6];
	uint32_t		spa;

	uint8_t			tha[6];
	uint32_t		tpa;

} arp_header_t;

arp_header_t* create_arp_probe_header(
		arp_header_t* header, struct sockaddr_ll* hw_addr, struct in_addr source_addr, struct in_addr target_addr
);

#endif
