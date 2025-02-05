#ifndef CELL_H
#define CELL_H

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct cell {
    uint8_t MinesNearby;
    bool HasMine;
    bool Uncovered;
    bool Flagged;
    uint16_t XPosition;
    uint16_t YPosition;
} cell;

cell *MakeCell(uint16_t X, uint16_t Y);

#endif
