#include <curses.h>
#include <stdlib.h>
#include <stdbool.h>

#include "cell.h"
#include "grid.h"

int main(int argc, char *argv[])
{
    if (argc != 4){
        printf("Command Format Should Be './minesweeper [WIDTH] [HEIGHT] [MINES]'.\n");
        return 0;
    }
    
    uint16_t Size[2];
    uint32_t Mines;

    char *End = NULL;

    for (int I = 1; I < argc; I++){
        long ParamLong = strtol(argv[I], &End, 10);
        if (ParamLong <= 0) {
            printf("Parameters cannot be less than or equal to 0.\n");
            return 0;
        }
        if (I != 3){
            if (ParamLong <= UINT16_MAX){
                Size[I - 1] = (uint16_t) ParamLong;
            }else {
                printf("Width / Height Must Be >= 0 and <= %d.\n", UINT16_MAX);
                return 0;
            }
        }else{
            if (ParamLong <= UINT32_MAX){
                Mines = (uint32_t) ParamLong;
            }else{
                printf("Mines Must Be >= 0 and <= %d.\n", UINT32_MAX);
                return 0;
            }
        }
    }

	grid *MyGrid = GenerateBoard(Size[0], Size[1], Mines);
	int PressedKey;
    bool GameRunning = true;
    bool DidWin = false;
    bool ShowEnd = true;

	initscr();
	keypad(stdscr, TRUE);
	halfdelay(1);
	noecho();
	cbreak();
	while (GameRunning){
		erase();
        uint32_t MinesRemaining = MyGrid->FlaggedTiles > MyGrid->Mines ? 0 : MyGrid->Mines-MyGrid->FlaggedTiles;
        mvprintw(0, 0, "Remaining Mines: %d | [ARROWS] To Move | [SPACE] To Reveal | [F] To Flag", MinesRemaining);
        DrawGrid(MyGrid, 0, 1);
		refresh();
		PressedKey = getch();
		if (PressedKey != ERR){
			if (PressedKey == 'q'){
                GameRunning = false;
                ShowEnd = false;
            }

            if (PressedKey == KEY_UP) { MoveCursorUp(MyGrid); }
            if (PressedKey == KEY_DOWN) { MoveCursorDown(MyGrid); }
            if (PressedKey == KEY_LEFT) { MoveCursorLeft(MyGrid); }
            if (PressedKey == KEY_RIGHT) { MoveCursorRight(MyGrid); }
            
            if (PressedKey == ' '){
                cell *Cell = GetCellAtCursor(MyGrid);
                if (Cell != NULL && !Cell->Flagged){
                    bool HitMine = UncoverCell(MyGrid, Cell);
                    if (HitMine){
                        GameRunning = false;
                        DidWin = false;
                    } else if (ClearedMines(MyGrid)){
                        GameRunning = false;
                        DidWin = true;
                    }
                }
            }
            
            if (PressedKey == 'f'){
                cell *Cell = GetCellAtCursor(MyGrid);
                if (Cell != NULL){
                    Cell->Flagged = !Cell->Flagged;
                    if (Cell->Flagged){
                        MyGrid->FlaggedTiles++;
                    }else{
                        MyGrid->FlaggedTiles--;
                    }
                }
            }
		}
		clrtoeol();
	}

    erase();
    
    if (ShowEnd){
        if (DidWin){
            mvprintw(0, 0, "You Win :)");
        }else {
            mvprintw(0, 0, "You Lose :(");
        }
        refresh();
        getch();
    }
    DeleteGridCells(MyGrid);
	endwin(); 
	return 0;
}
