#ifndef UPPER_COMM_H
#define UPPER_COMM_H

#include "uart.h"

#include "bms_comm.h"

#define UPPER_DATA_PER_FRAME (8)
#define UPPER_FRAME_LEN (7 + UPPER_DATA_PER_FRAME + 2)

void UpperCommTask(void *argument);

#endif /* UPPER_COMM_H */
