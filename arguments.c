#include "arp_scanner.h"

struct arguments arguments;

static error_t parse_opt(int key, char *arg, struct argp_state *state) {
	switch (key) {
		case 'b':
			long batch = strtol(arg, NULL, 10);
			if (batch <= 0 || batch > UINT_MAX) {
				printf("Batch size must be uint, current: %s", arg);
				return -1;
			}
			arguments.batch_size = batch;
			break;
		case 'i':
			long interval = strtol(arg, NULL, 10);
			if (interval <= 0 || interval > UINT_MAX) {
				printf("Interval must be uint, current: %s", arg);
				return -1;
			}
			arguments.sending_interval = interval;
			break;
		case ARGP_KEY_ARG: return 0;
		default: return ARGP_ERR_UNKNOWN;
	}
	return 0;
}

int parse_args(int argc, char** argv) {
	arguments = (struct arguments){
			.batch_size = 32,
			.sending_interval=4
	};
	struct argp_option opts[3] = {
			{ .name="batch", .key='b', .arg="32", .flags=0, .doc="addresses batch size."},
			{ .name="sending_interval", .key='i', .arg="4", .flags=0, .doc="Pause before sending batch (sec)."},
			{ 0 }
	};
	struct argp argp = {
			opts,
			parse_opt,
			NULL,
			"ARP subnet scanner. Send ARP probe packet to all addresses in subnet and wait ARP reply packets",
			0,
			0,
			0
	};
	return argp_parse(&argp, argc, argv, 0, 0, &arguments);
}