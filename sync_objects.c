#include <stdint.h>
#include <stdatomic.h>

uint32_t	send_barrier = 1;
atomic_int	broadcast_finished = 0;
