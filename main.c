#include "arp_scanner.h"

extern struct arguments arguments;

int send_wrapper(void* arg) {
	return send_ARP_probe_range_subnet(arg, arguments.batch_size, arguments.sending_interval);
}

int receive_wrapper(void* arg) {
	receive_ARP(arg);
	return 0;
}

int main(int argc, char *argv[]) {

	if (parse_args(argc, argv) != 0) {
		exit(-1);
	}

	char* interface_name = select_interface();
	if (interface_name == NULL) {
		perror("Interface not selected");
		exit(-1);
	}

	interface_t interface;
	if (get_interface(interface_name, &interface) != 0) {
		printf("Interface %s not found or not configured (MAC and IP addresses)\n", interface_name);
		perror("Message");
		exit(-1);
	}
	free(interface_name);

	thrd_t sender, receiver;
	if (thrd_create(&receiver, receive_wrapper, &interface) != thrd_success) {
		perror("Failed to create receiver thread");
		exit(-1);
	}
	if (thrd_create(&sender, send_wrapper, &interface) != thrd_success) {
		perror("Failed to create sender thread");
		exit(-1);
	}
	thrd_join(receiver, NULL);

	exit(0);
}
