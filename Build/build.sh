clang -c ../cell.c -o cell.o
clang -c ../grid.c -o grid.o
clang -c ../main.c -o main.o
clang main.o cell.o grid.o -lcurses -o minesweeper.bin
