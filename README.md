# Minix Minesweeper
An Implementation of Minesweeper in C using the original curses library, written over a few days exclusively in Minix with nano on VirtualBox (And PuTTY). A Timelapse is WIP.

### Gameplay:
![minesweeper_gameplay](https://github.com/user-attachments/assets/31ba7ad6-1627-4241-8a14-3e989927450f)

### Info:
For some reason, using VirtualBox's built in terminal causes weird issues to happen with spacing, which I couldn't find a fix for online. As such, I recommend following a tutorial like [this](https://mike632t.wordpress.com/2022/03/24/installing-and-configuring-ssh-on-minix-3/) and SSH'ing in instead.

### How To Play:
1. Download `minesweeper.bin` from releases or build it.
2. Run the program with the following syntax: `./minesweeper.bin [WIDTH] [HEIGHT] [MINES]`.
3. Use `ARROWS` to Move, `SPACE` to Uncover Tiles and `F` to Flag Tiles.

### How To Build:
**I CAN'T GUARANTEE THAT BUILDING WILL WORK**
1. Ensure you have `clang` and `curses` installed. *Minix contains both by default.*
2. Set your current working directory to the `Build/` folder.
3. Run `./build.sh`.
4. `minesweeper.bin` should now be in the folder.
