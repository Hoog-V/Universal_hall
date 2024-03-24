#ifndef IRQ_TYPEDEFS_H
#define IRQ_TYPEDEFS_H
#include <stdint.h>

typedef struct {
    uint8_t transaction_type;
    uint8_t instance_num;
    const uint8_t *write_buffer;
    uint8_t *read_buffer;
    uint8_t buf_size;
    uint8_t buf_cnt;
    uint8_t status;
} bustransaction_t;

#endif /* IRQ_TYPEDEFS_H */