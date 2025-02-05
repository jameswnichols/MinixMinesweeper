#ifndef GRID_H
#define GRID_H

#include <curses.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

#include "cell.h"

typedef struct vector2i{
    uint16_t X;
    uint16_t Y;
} vector2i;

typedef struct grid{
    uint16_t Width;
    uint16_t Height;
    
    uint16_t CursorX;
    uint16_t CursorY;
    
    uint32_t Mines;
    uint32_t UncoveredTiles;    
    uint32_t FlaggedTiles;

    cell **Cells;
} grid;

grid *GenerateBoard(uint16_t Width, uint16_t Height, uint32_t Mines);

cell *GetCellAtPosition(grid *CurrentGrid, uint16_t X, uint16_t Y);

cell *GetCellAtCursor(grid *Grid);

cell **GetNeighbours(grid *Grid, cell *CurrentCell, uint8_t *Count);

uint8_t GetNearbyMines(grid *Grid, cell *Cell);

void DeleteNeighbours(cell **Neighbours);

vector2i *MakeVector2i(uint16_t X, uint16_t Y);

void ShufflePositions(vector2i **Positions, uint32_t Size);

void DeletePositions(vector2i **Positions, uint32_t Size);

void DeleteGridCells(grid *Grid);

void DrawGrid(grid *Grid, uint16_t StartX, uint16_t StartY);

bool UncoverCell(grid *Grid, cell *Cell);

bool ClearedMines(grid *Grid);

void MoveCursorLeft(grid *Grid);

void MoveCursorRight(grid *Grid);

void MoveCursorUp(grid *Grid);

void MoveCursorDown(grid *Grid);

#endif
