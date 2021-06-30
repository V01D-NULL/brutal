#include <brutal/sync.h>
#include <host/log.h>
#include "arch/arch.h"

static Lock logger_lock;

void host_log_lock(void)
{
    lock_acquire(&logger_lock);
}

struct writer *host_log_writer(void)
{
    return arch_debug();
}

void host_log_unlock(void)
{
    lock_release(&logger_lock);
}

void host_log_panic(void)
{
    arch_stop();
}
