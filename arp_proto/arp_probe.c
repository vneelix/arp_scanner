#include "arp_proto.h"

arp_header_t* create_arp_probe_header(
		arp_header_t* header, struct sockaddr_ll* hw_addr, struct in_addr source_addr, struct in_addr target_addr
) {
	if (header == NULL) {
		return NULL;
	}

	*header = (arp_header_t){
		.htype = htons(1),
		.ptype = htons(ETH_P_IP),
		.hlen = 6,
		.plen = 4,
		.oper = htons(1),
		.spa = (uint32_t)source_addr.s_addr,
		.tha = {0, 0, 0, 0, 0, 0},
		.tpa = (uint32_t)target_addr.s_addr
	};
	memcpy(&header->sha, hw_addr->sll_addr, 6);

	return header;
}
