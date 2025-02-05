#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "cell.h"

cell *MakeCell(uint16_t X, uint16_t Y){
    cell *NewCell = (cell*)malloc(sizeof(cell));
    NewCell->MinesNearby = 0;
    NewCell->HasMine = false;
    NewCell->Uncovered = false;
    NewCell->XPosition = X;
    NewCell->YPosition = Y;
    return NewCell;
}
