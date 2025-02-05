#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include <curses.h>

#include "grid.h"
#include "cell.h"

grid *GenerateBoard(uint16_t Width, uint16_t Height, uint32_t Mines){
    uint32_t MaxTiles = Width * Height;
    uint32_t MineCount = (Mines > MaxTiles) ? MaxTiles : Mines;

    vector2i **Positions = (vector2i**)malloc(MaxTiles * sizeof(vector2i*));

    grid *NewGrid = (grid*)malloc(sizeof(grid));
    NewGrid->UncoveredTiles = 0;
    NewGrid->FlaggedTiles = 0;
    NewGrid->Mines = MineCount;
    NewGrid->Width = Width;
    NewGrid->Height = Height;
    NewGrid->CursorX = 0;
    NewGrid->CursorY = 0;
    NewGrid->Cells = (cell**)malloc(MaxTiles * sizeof(cell*));
    uint32_t CellIndex = 0;

    for (uint16_t Y = 0; Y < Height; Y++){
        for (uint16_t X = 0; X < Width; X++){
            cell *NewCell = MakeCell(X, Y);
            Positions[CellIndex] = MakeVector2i(X, Y);
            NewGrid->Cells[CellIndex] = NewCell;
            CellIndex++;
        }
    }

    ShufflePositions(Positions, MaxTiles);

    for (uint32_t I = 0; I < MineCount; I++){
        vector2i *ChosenPosition = Positions[I];
        uint16_t ChosenX = ChosenPosition->X;
        uint16_t ChosenY = ChosenPosition->Y;
        cell *PositionCell = GetCellAtPosition(NewGrid,ChosenX,ChosenY);
        if (PositionCell != NULL){
            PositionCell->HasMine = true;
        }
    }
    
    DeletePositions(Positions, MaxTiles);
    
    for (uint16_t Y = 0; Y < Height; Y++){
        for (uint16_t X = 0; X < Width; X++){
            cell *Cell = GetCellAtPosition(NewGrid, X, Y);
			if (Cell != NULL){
				Cell->MinesNearby = GetNearbyMines(NewGrid, Cell);
			}
        }
    }

    return NewGrid;
}

void MoveCursorRight(grid *Grid){
    if (Grid->CursorX < Grid->Width - 1){
        Grid->CursorX++;
    }
}

void MoveCursorLeft(grid *Grid){
    if (Grid->CursorX > 0){
        Grid->CursorX--;
    }
}

void MoveCursorUp(grid *Grid){
    if (Grid->CursorY > 0){
        Grid->CursorY--;
    }
}

void MoveCursorDown(grid *Grid){
    if (Grid->CursorY < Grid->Height - 1){
        Grid->CursorY++;
    }
}

void DrawGrid(grid *Grid, uint16_t StartX, uint16_t StartY){
    uint16_t LocalX;
    uint16_t LocalY;
    for (uint16_t Y = 0; Y < Grid->Height; Y++){
        LocalY = StartY + Y;
        
        for (uint16_t X = 0; X < Grid->Width; X++){
            LocalX = StartX + (2 * X);

            cell *Cell = GetCellAtPosition(Grid, X, Y);
            if (Cell != NULL){
                if (Cell->Uncovered){
                    if (Cell->HasMine){
                        mvinsch(LocalY, LocalX, 'X');
                    }else{
                        uint8_t Mines = Cell->MinesNearby;
                        char Symbol = Mines > 0 ? Mines + '0' : '.'; 
                        mvaddch(LocalY, LocalX, Symbol);
                    }
                }else{
                    if (Cell->Flagged){
                        mvinsch(LocalY, LocalX, '>');
                    }else{
                        mvinsch(LocalY, LocalX, '#');
                    }
                }
            }
        }
    }
    move(StartY + Grid->CursorY, StartX + (2 * Grid->CursorX));
}

void ShufflePositions(vector2i **Positions, uint32_t Size){
    srand(time(NULL));
    if (Size > 1){
        for (uint32_t I = 0; I < Size - 1; I++){
            uint32_t J = I + rand() / (RAND_MAX / (Size - I) + 1);
            vector2i *SwapMember = Positions[J];
            Positions[J] = Positions[I];
            Positions[I] = SwapMember;
        }
    }
}

void DeletePositions(vector2i **Positions, uint32_t Size){
    for (uint32_t I = 0; I < Size; I++){
        free(Positions[I]);
    }
    free(Positions);
}

vector2i *MakeVector2i(uint16_t X, uint16_t Y){
    vector2i *NewVector = (vector2i*)malloc(sizeof(vector2i));
    NewVector->X = X;
    NewVector->Y = Y;
    return NewVector;
}

cell *GetCellAtPosition(grid *CurrentGrid, uint16_t X, uint16_t Y){
    uint16_t Width = CurrentGrid->Width;
    uint16_t Height = CurrentGrid->Height;    
    if (X < 0 || X >= Width || Y < 0 || Y >= Height){
        return NULL;
    }

    uint32_t Index = (Y * Width) + (X % Width);
    
    return CurrentGrid->Cells[Index];
}

cell *GetCellAtCursor(grid *Grid){
    return GetCellAtPosition(Grid, Grid->CursorX, Grid->CursorY);
}

cell **GetNeighbours(grid *Grid, cell *CurrentCell, uint8_t *Count){
    cell **Neighbours = (cell**)malloc(8*sizeof(cell*));
    int CellX = (int) CurrentCell->XPosition;
    int CellY = (int) CurrentCell->YPosition;
    uint8_t  Index = 0;
    for (int Y = -1; Y < 2; Y++){
        for (int X = -1; X < 2; X++){
            if (X == 0 && Y == 0){
                continue;
            }

            int NewX = CellX + X;
            int NewY = CellY + Y;
            
            int NewWidth = (int)Grid->Width;
            int NewHeight = (int)Grid->Height;

            if (NewX>=0&&NewX<NewWidth&&NewY>=0&&NewY<NewHeight){
                uint16_t UNewX = (uint16_t)NewX;
                uint16_t UNewY = (uint16_t)NewY;
		        Neighbours[Index] = GetCellAtPosition(Grid,UNewX,UNewY);
                Index++;
            }
        }
    }
    *Count = Index;
    return Neighbours;
}

uint8_t GetNearbyMines(grid *Grid, cell *Cell){
    uint8_t Nearby = 0;
    uint8_t Count = 0;
    cell **Neighbours = GetNeighbours(Grid, Cell, &Count);
    for (uint8_t I = 0; I < Count; I++){
        if (Neighbours[I] != NULL){
            cell *Neighbour = Neighbours[I];
            if (Neighbour->HasMine){
                Nearby++;
            }
        }
    }
    DeleteNeighbours(Neighbours);
    return Nearby;
}

bool UncoverCell(grid *Grid, cell *Cell){
    Cell->Uncovered = true;
    Grid->UncoveredTiles++;
    if (Cell->MinesNearby == 0){
        uint8_t Count = 0;
        cell **Neighbours = GetNeighbours(Grid, Cell, &Count);
        for (uint8_t I = 0; I < Count; I++){
            if (Neighbours[I] != NULL && !Neighbours[I]->Uncovered){
                UncoverCell(Grid, Neighbours[I]);
            }
        }
        DeleteNeighbours(Neighbours);
    }
    return Cell->HasMine;
}

bool ClearedMines(grid *Grid){
    return (Grid->Width * Grid->Height) - Grid->Mines == Grid->UncoveredTiles;
}

void DeleteNeighbours(cell **Neighbours){
    free(Neighbours);
	Neighbours = NULL;
}

void DeleteGridCells(grid *Grid){
    uint32_t Size = Grid->Width * Grid->Height;
    for (uint32_t Index = 0; Index < Size; Index++){
        free(Grid->Cells[Index]);
    }
    free(Grid->Cells);
    Grid->Cells = NULL;
}
