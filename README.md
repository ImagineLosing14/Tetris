## 🕹️ Features
* **User Profiles:** Enter your username to track your progress and high scores.
* **Save & Resume:** Life got in the way? Your game state is saved automatically, allowing you to "Continue" from the main menu.
* **Persistent Leaderboard:** Compete against other local users for the top spot.
* **Classic Mechanics:** Smooth Tetromino rotation and line-clearing logic powered by Ncurses.

## ⌨️ Controls
| Key | Action |
|-----|--------|
| ← / → | Move Left/Right |
| ↑ | Rotate Piece |
| ↓ | Soft Drop |
| Q | Quit to Menu |

## 🛠️ Troubleshooting
If you encounter display issues, ensure your terminal window is large enough (at least 80x24) and that your `TERM` environment variable is set to `xterm-256color`.

## Prerequisites
* GCC compiler
* Ncurses library (`libncurses5-dev`)


## INSTALLATION
 LINUX:
 sudo apt-get install libncurses5-dev libncursesw5-dev

 MACOS:
 brew install ncurses


## INSTALLATION AND BUILD
git clone https://github.com/yourusername/tetris.git
cd tetris


## Build & Run
make
./tetris
