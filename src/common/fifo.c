#include "fifo.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

int fifo_init(Fifo* fifo, uint32_t length) {
    assert(fifo != NULL);

    uint32_t fifo_byte_length = length * sizeof(FifoElement);

    fifo->list      = malloc(fifo_byte_length);
    fifo->length    = length;
    fifo->index     = 0;
    fifo->items_num = 0;

    if (fifo->list == NULL) { return -1; }

    memset(fifo->list, 0, fifo_byte_length);

    return 0;
}

int fifo_destroy(Fifo* fifo) {
    assert(fifo != NULL);

    for (unsigned i = fifo->index; i < fifo->index + fifo->items_num; i++) {
        free(fifo->list[i].data);
    }

    free(fifo->list);
    fifo->list      = NULL;
    fifo->length    = 0;
    fifo->index     = 0;
    fifo->items_num = 0;

    return 0;
}

int fifo_push(Fifo* fifo, void* data, uint32_t data_size) {
    assert(fifo != NULL);
    assert(data != NULL);

    if (fifo->items_num == fifo->length) {
        // fifo would be overwritten
        return -1;
    }

    unsigned element_idx = (fifo->index + fifo->items_num) % fifo->length;

    fifo->list[element_idx].data_size = data_size;
    fifo->list[element_idx].data      = malloc(data_size);
    memcpy(fifo->list[element_idx].data, data, data_size);

    fifo->items_num++;

    return 0;
}

int fifo_pop(Fifo* fifo, void* data, uint32_t* data_size) {
    assert(fifo != NULL);

    if (fifo->items_num == 0) {
        // fifo empty
        return -1;
    }

    // if either output arguments are null, just remove the top item.
    if (data != NULL && data_size != NULL) {
        memcpy(data, fifo->list[fifo->index].data,
               fifo->list[fifo->index].data_size);
        *data_size = fifo->list[fifo->index].data_size;
    }

    fifo->list[fifo->index].data      = NULL;
    fifo->list[fifo->index].data_size = 0;

    fifo->index = (fifo->index + 1) % fifo->length;
    fifo->items_num--;

    return 0;
}
