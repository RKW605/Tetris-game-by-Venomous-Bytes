# Tetris Game - Console Based (C++)

## 🎮 Project Description
This is a **console-based Tetris game** written in **C++**. The game features real-time movement, soft drop functionality, hard drop functionality, scoring, and level progression. It is designed for Windows and uses **"windows.h"** for console manipulation.

## 🚀 Features
- **Classic Tetris Gameplay**: Rotate, move, and drop Tetrominoes to form lines.
- **Scoring System**: Earn points based on cleared lines and level progression.
- **Hard Drop**: Instantly place the Tetromino to the bottom.
- **Console Graphics**: Uses ASCII and Unicode characters for rendering Tetrominoes.
- **Windows Console Manipulation**: Uses `windows.h` for cursor movement and UI rendering.
- **Keyboard Controls**: `Arrow keys` for movement, `Spacebar` for hard drop, and `Q` to quit.

---

## 🛠️ Setup & Compilation
### **Prerequisites**
- Windows OS
- C++ Compiler (MinGW or MSVC)
- Command Prompt or Terminal

### **Compiling the Project**
```sh
 g++ Tertris.c++ -o Tetris.exe -std=c++11
```

### **Running the Game**
```sh
 ./Tetris.exe
```

---

## 📜 Header Files Used

| Header File | Purpose |
|------------|---------|
| `#include <conio.h>` | Detects keyboard input. |
| `#include <cstdlib>` | Supports random number generation. |
| `#include <ctime>` | Used for seeding random numbers. |
| `#include <iostream>` | Handles input/output operations. |
| `#include <vector>` | Manages Tetromino shapes and positions. |
| `#include <windows.h>` | Manages console rendering and cursor movement. |
| `#include "Tetris.h"` | includes all necessary libraries, and code for welcome screen |

---

## 🔍 Code Structure & Functionality
### **Global Variables**
```cpp
bool gameOver;         // Tracks if the game is over
bool playing = true;   // Indicates if the game is currently running
bool notHardDrop = true; // For Hard Drop Checking
int Score = 0;         // Player's current score
int thisGameHighScore = 0; // To manage this game high-score
int overallHighScore; // To track overall high-score
int level = 1;         // Current level
int speed = 100;       // Game speed, decreases as level increases
int clearedRows = 0; // Track the number of cleared rows
```

### **Classes**
#### **Tetrominoes Class**
Manages the different Tetrimino shapes and their rotations.
```cpp
class Tetrominoes {
    vector<string> Tetromino[7];
}
```
- Stores all **7 Tetrimino** shapes.
- Uses **Unicode symbols** for rendering.

#### **GameBoard Class**
Handles the **game grid**, display board, score and next tetromino, also handles inputs and logic of the game.
```cpp
class gameBoard {
    vector<vector<strinng>> field;
}
```
- Stores a game grid.
- Detects when rows are cleared.
- moves and rotate tetrominoes.
- check for collision detection
- places tetrommino and get new tetrominoes

### **Functions & Their Purpose**
| Function | Description |
|----------|-------------|
| `void gotoxy(int x, int y)` | Moves the console cursor to the specified position. |
| `void hidecursor()` | Hides the console cursor for smooth rendering. |
| `void showcursor()` | Displays the cursor when needed. |
| `void endScreen(int width, int height)` | Displays the game-over screen. |
| `void playAgainArt(int width)` | Displays play-again message. |
| `void quitArt(int width)` | Displays quit message. |
| `void scoreMessage(int height)` | Shows the custom message based on current-score, this game high-score and overall high-score. |
| `void tetrisInstructionScreen()` | Displays game instructions. |
| `void welcomeBack(vector<pair<int, int>> activeTetromino, int color)` | Renders the welcome screen. |

---

## 👨‍💻 Contributors
| Name | Student ID |
|------|-----------|
| Anghan Nena | 202401053 |
| Ruhan Kureshi | 202401103 |
| Jay Lavingiya | 202401105 |
| Manali Malani | 202401111 |

---

## 📌 Future Enhancements
- Add **Ghost Piece** to preview where the Tetromino will land.
- Implement **Multiplayer Mode**.
- Add a **Leaderboard** to save high scores.

---

## 📢 Acknowledgments
This project was developed as part of a semester group project. Special thanks to all contributors!

