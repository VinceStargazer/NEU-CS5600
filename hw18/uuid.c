#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include "uuid.h"

// Function to generate a uuid
void generate_uuid(char* uuid) {
    struct timeval tv;
    gettimeofday(&tv, NULL);

    // Time-based UUID version 1
    uint64_t time_value = (uint64_t)tv.tv_sec * 10000000 + (uint64_t)tv.tv_usec * 10 + 0x01B21DD213814000;
    uint32_t time_low = (uint32_t)time_value;
    uint16_t time_mid = (uint16_t)(time_value >> 32);
    uint16_t time_hi_and_version = (uint16_t)(time_value >> 48);
    time_hi_and_version |= 0x1000; // UUID version 1

    // Clock sequence and node (machine-specific)
    uint16_t clock_seq = (uint16_t)(random() & 0x3FFF); // Generate random clock sequence
    uint32_t node_value = (uint32_t)getpid();
    uint8_t node[6];
    memcpy(node, &node_value, 6);

    snprintf(uuid, 37, "%08X-%04X-%04X-%04X-%02X%02X%02X%02X%02X%02X",
            time_low, time_mid, time_hi_and_version,
            clock_seq, node[0], node[1], node[2],
            node[3], node[4], node[5]);
}
