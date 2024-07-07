#include "arp_scanner.h"

extern uint32_t		send_barrier;
extern atomic_int	broadcast_finished;

static int create_listen_ARP_socket(interface_t* interface) {
	int socket_fd= socket(AF_PACKET, SOCK_DGRAM, htons(ETH_P_ARP));
	if (socket_fd == -1) {
		perror("Failed to create arp_proto listen sock");
		return -1;
	}

	struct sockaddr_ll addr;
	memset(&addr, 0, sizeof(struct sockaddr_ll));
	addr.sll_family = AF_PACKET;
	addr.sll_ifindex = (int)interface->if_index;
	addr.sll_protocol = htons(ETH_P_ARP);

	if (bind(socket_fd, (struct sockaddr*)&addr, sizeof(struct sockaddr_ll)) != 0) {
		perror("Failed to bind arp_proto listen sock");
		close(socket_fd);
		return -1;
	}

	struct timeval tv = {.tv_sec = 8, .tv_usec = 0};
	if (setsockopt(socket_fd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv) != 0) {
		perror("Failed to set recv timeout");
		close(socket_fd);
		return -1;
	}

	return socket_fd;
}

static int arp_reply_stdout_stream(arp_header_t* header) {
	char source_in_addr[64];
	char source_hw_addr[64];

	if (inet_ntop(AF_INET, &header->spa, source_in_addr, 64) == NULL) {
		sprintf(source_in_addr, "undefined");
	}
	snprintf(source_hw_addr, sizeof(source_hw_addr), "%02x:%02x:%02x:%02x:%02x:%02x",
			 header->sha[0], header->sha[1], header->sha[2], header->sha[3], header->sha[4], header->sha[5]);

	printf("ip: %-15s mac: %s\n", source_in_addr, source_hw_addr);
	return 0;
}

void receive_ARP(interface_t* interface) {
	uint8_t rb[128];
	int socket_fd = create_listen_ARP_socket(interface);
	if (socket_fd == -1) {
		return;
	}

	// Awake sender thread after creating listen socket
	send_barrier = 0;
	if (futex(&send_barrier, FUTEX_WAKE, 1, NULL, NULL, 0) == -1) {
		perror("Failed to WAKE sender thread");
		close(socket_fd);
		return;
	}

	for (; ;) {
		if (recv(socket_fd, rb, 128, 0) == -1) {
			if (errno == EAGAIN) {
				if (atomic_load((int*)&broadcast_finished) == 1) {
					close(socket_fd);
					return;
				}
			} else {
				perror("Undefined error recv function");
				close(socket_fd);
				return;
			}
		}
		arp_header_t* header = (arp_header_t*)rb;
		if (ntohs(header->oper) == 2) {
			arp_reply_stdout_stream(header);
		}
	}
}
