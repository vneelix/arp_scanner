#include "interface.h"

int get_interface(const char* if_name, interface_t* interface) {
	unsigned int counter = 0;

	// find interface ID
	struct if_nameindex* name_index_arr = if_nameindex();
	if (name_index_arr == NULL) {
		return -1;
	}
	int	idx = 0;
	while (name_index_arr[idx].if_index != 0 && name_index_arr[idx].if_name != NULL) {
		if (strcmp(if_name, name_index_arr[idx].if_name) == 0) {
			interface->if_index = name_index_arr[idx].if_index;
			counter = 1;
			break;
		}
		idx++;
	}
	if_freenameindex(name_index_arr);
	if (counter == 0) {
		return -1;
	}

	// find interface hardware address, ipv4 address and netmask
	struct ifaddrs* addrs;
	if (getifaddrs(&addrs) != 0) {
		return -1;
	}
	counter = 0;
	struct ifaddrs* node = addrs;
	do {
		if (strcmp(if_name, node->ifa_name) == 0) {
			switch (node->ifa_addr->sa_family) {
				case AF_INET:
					interface->addr = *(struct sockaddr_in*)node->ifa_addr;
					interface->netmask = *(struct sockaddr_in*)node->ifa_netmask;
					interface->broadcast = *(struct sockaddr_in*)node->ifa_ifu.ifu_broadaddr;
					counter++;
					break;
				case AF_PACKET:
					interface->hw_addr = *(struct sockaddr_ll*)node->ifa_addr;
					counter++;
					break;
			}
		}
		node = node->ifa_next;
	} while (node != NULL && counter < 2);
	freeifaddrs(addrs);
	if (counter != 2) {
		return -1;
	}

	return 0;
}
