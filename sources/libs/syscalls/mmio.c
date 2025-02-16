#include <brutal/log.h>
#include <syscalls/helpers.h>
#include <syscalls/mmio.h>
#include <syscalls/syscalls.h>

void br_mmio_init(BrMmio *self, uintptr_t paddr, uintptr_t size)
{
    BrCreateArgs mem_obj = {
        .type = BR_OBJECT_MEMORY,
        .mem_obj = {
            .addr = paddr,
            .size = size,
            .flags = BR_MEM_OBJ_PMM,
        },
    };

    assert_truth(br_create(&mem_obj) == BR_SUCCESS);

    BrMapArgs mem_map = {
        .space = BR_SPACE_SELF,
        .mem_obj = mem_obj.handle,
    };

    assert_truth(br_map(&mem_map) == BR_SUCCESS);

    self->base = mem_map.vaddr;
    self->size = size;
    self->handle = mem_obj.handle;
}

void br_mmio_deinit(BrMmio *self)
{
    BrUnmapArgs mem_unmap = {
        .space = BR_SPACE_SELF,
        .vaddr = self->base,
        .size = self->size,
    };

    assert_truth(br_unmap(&mem_unmap) == BR_SUCCESS);

    brh_close(self->handle);
}
