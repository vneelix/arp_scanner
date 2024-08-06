#include "arp_scanner.h"

extern uint32_t		send_barrier;
extern atomic_int	broadcast_finished;

int send_ARP_probe_range_subnet(interface_t* interface, uint32_t bulk_size, time_t delay) {
	uint32_t addresses = get_subnet_size(&interface->netmask);

	int socket_fd = socket(AF_PACKET, SOCK_DGRAM, IPPROTO_RAW);
	if (socket_fd == -1) {
		return -1;
	}

	// waiting for send barrier (disables receiver thread on ready)
	if (futex(&send_barrier, FUTEX_WAIT, 1, NULL, NULL, 0) == -1) {
		perror("Failed to WAIT receiver thread");
	}

	struct sockaddr_ll hw_broadcast = {
			.sll_family = AF_PACKET,
			.sll_protocol = htons(ETH_P_ARP),
			.sll_ifindex = (int)interface->if_index,
			.sll_hatype = 0,
			.sll_pkttype = 0,
			.sll_halen = 6,
			.sll_addr = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}
	};

	uint32_t addr = (uint32_t)interface->addr.sin_addr.s_addr & (uint32_t)interface->netmask.sin_addr.s_addr;

	arp_header_t header;
	create_arp_probe_header(
			&header, &interface->hw_addr, interface->addr.sin_addr, interface->addr.sin_addr
	);

	printf("Sending started: subnet size %u, total probe requests %u\n", addresses, addresses - 1);

	while (addresses)  {

		for (uint32_t c = 0; addresses && c < bulk_size; c++) {
			// convert ipv4 address to big endian format for simple increment
			// 192.168.0.0 --big endian--> 0.0.168.192 + 1 = 1.0.168.192 --little endian--> 192.168.0.1
			addr = ntohl(htonl(addr) + 1);
			header.tpa = addr;

			// skip sending ARP Probe for self
			if (interface->addr.sin_addr.s_addr == addr) {
				addresses--;
				continue;
			}

			ssize_t ret = sendto(
					socket_fd, &header, sizeof(header), 0,
					(struct sockaddr*)&hw_broadcast, sizeof(struct sockaddr_ll)
			);
			if (ret <= 0) {
				return -1;
			}
			addresses--;
		}

		if (addresses && delay) {
			sleep(delay);
		}
	}

	atomic_exchange((atomic_int*)&broadcast_finished, 1);
	close(socket_fd);

	return 0;
}
