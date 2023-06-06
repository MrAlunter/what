#ifndef TYPES_H
#define TYPES_H 1

#define PB1 PIN4_bm
#define PB2 PIN5_bm
#define PB3 PIN6_bm
#define PB4 PIN7_bm

typedef enum
{
    PAUSED,
    PLAYING
} State;

typedef enum
{
    AWAITING_COMMAND,
    AWAITING_PAYLOAD
} Serial_State;

typedef enum
{
    ACK,
    NACK,
    WAITING
} Command_Response;

#endif
