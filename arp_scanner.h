#ifndef ARP_SCANNER_H
#define ARP_SCANNER_H

#include <argp.h>
#include <errno.h>

#include <stdio.h>
#include <curses.h>
#include <unistd.h>
#include <threads.h>
#include <arpa/inet.h>

#include <stdatomic.h>
#include <linux/futex.h>
#include <sys/syscall.h>

#include "utils/utils.h"
#include "arp_proto/arp_proto.h"
#include "interface/interface.h"

struct arguments {
	uint batch_size;
	uint sending_interval;
};
int		parse_args(int argc, char** argv);

int		send_ARP_probe_range_subnet(interface_t* interface, uint32_t bulk_size, time_t delay);
void	receive_ARP(interface_t* interface);
char*	select_interface();

int		futex(
		u_int32_t *uaddr, int futex_op, u_int32_t val, const struct timespec *timeout, uint32_t *uaddr2, uint32_t val3
);

#endif
