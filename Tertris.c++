#include "Tetris.h"

// Global variables
bool gameOver;
bool playing = true;
bool notHardDrop = true;
int Score = 0;
int thisGameHighScore = 0;
int overallHighScore;
int level = 1;
int speed = 300;
int clearedRows = 0;
int noOfHardDrops = 0;

//  Used classes
class Tetrominoes;
class gameBoard;

// Used normal functions
void gotoxy(int x, int y);
void hidecursor();
void showcursor();
void endScreen(int width, int height);
void playAgainArt(int width);
void quitArt(int width);
void scoreMessage(int height);
void tetrisInstructionScreen();

// Tetromino class
class Tetrominoes
{
public:
    vector<string> Tetromino[7]; // Array of Tetrominoes

    Tetrominoes()
    {
        Tetromino[0] = {// I-Tetromino (4×4)
                        "🟥", "🟥", "🟥", "🟥",
                        "⬜", "⬜", "⬜", "⬜",
                        "⬜", "⬜", "⬜", "⬜",
                        "⬜", "⬜", "⬜", "⬜"};

        Tetromino[1] = {// O-Tetromino (4×4)
                        "⬜", "🟨", "🟨", "⬜",
                        "⬜", "🟨", "🟨", "⬜",
                        "⬜", "⬜", "⬜", "⬜",
                        "⬜", "⬜", "⬜", "⬜"};

        Tetromino[2] = {// T-Tetromino (4×4)
                        "⬜", "🟪", "🟪", "🟪",
                        "⬜", "⬜", "🟪", "⬜",
                        "⬜", "⬜", "⬜", "⬜",
                        "⬜", "⬜", "⬜", "⬜"};

        Tetromino[3] = {// L-Tetromino (4×4)
                        "🟧", "⬜", "⬜", "⬜",
                        "🟧", "⬜", "⬜", "⬜",
                        "🟧", "🟧", "⬜", "⬜",
                        "⬜", "⬜", "⬜", "⬜"};

        Tetromino[4] = {// J-Tetromino (4×4)
                        "⬜", "⬜", "🟫", "⬜",
                        "⬜", "⬜", "🟫", "⬜",
                        "⬜", "🟫", "🟫", "⬜",
                        "⬜", "⬜", "⬜", "⬜"};

        Tetromino[5] = {// S-Tetromino (4×4)
                        "⬜", "⬜", "🟩", "🟩",
                        "⬜", "🟩", "🟩", "⬜",
                        "⬜", "⬜", "⬜", "⬜",
                        "⬜", "⬜", "⬜", "⬜"};

        Tetromino[6] = {// Z-Tetromino (4×4)
                        "🟦", "🟦", "⬜", "⬜",
                        "⬜", "🟦", "🟦", "⬜",
                        "⬜", "⬜", "⬜", "⬜",
                        "⬜", "⬜", "⬜", "⬜"};
    }
};

// gameBoard class
class gameBoard
{
public:
    int count1;
    int count2;
    bool sameAllow1;
    bool sameAllow2;
    int width;
    int height;
    int pieceX;
    int pieceY;
    int prev;
    int currTetromino; // currTetromino Index
    int nextTetromino; // nextTetromino Index
    Tetrominoes Tetrominoes;
    vector<vector<string>> field;

    gameBoard(int w, int h) // gameBoard constructor
    {
        count1 = 0; // This is for randomness
        count2 = 0; // This is for randomness
        sameAllow1 = false; // This is for randomness
        sameAllow2 = false; // This is for randomness
        width = w;
        height = h;
        pieceY = 0;
        pieceX = width / 2;
        currTetromino = rand() % 7;
        nextTetromino = rand() % 7;
        while (nextTetromino == currTetromino)
        {
            nextTetromino = rand() % 7;
        }
        field.resize(height, vector<string>(width, "⬜"));
        for (int y = 0; y < height; y++)
        {
            field[y][0] = "🧱";
            field[y][width - 1] = "🧱";
        }
        for (int x = 0; x < width; x++)
        {
            field[height - 1][x] = "🧱";
        }
    }

    void clearBoard() // clearing board before placing new location of tetromino
    {
        for (int y = 0; y < 4; y++)
        {
            for (int x = 0; x < 4; x++)
            {
                if (Tetrominoes.Tetromino[currTetromino][4 * y + x] != "⬜" && pieceY + y < height - 1)
                {
                    field[pieceY + y][pieceX + x] = "⬜";
                }
            }
        }
    }
    bool canMove(int newX, int newY) // checking if Tetromino can move on the specific position
    {
        vector<string> &shape = Tetrominoes.Tetromino[currTetromino];

        for (int y = 0; y < 4; y++)
        {
            for (int x = 0; x < 4; x++)
            { // Only check relevant columns
                if (shape[4 * y + x] != "⬜")
                {
                    int boardX = newX + x;
                    int boardY = newY + y;

                    // Collision check
                    if (field[boardY][boardX] != "⬜")
                        return false;
                }
            }
        }
        return true; // Movement is valid
    }
    void rotateTetromino(bool clockwise) // Rotate function to rotate tetrominoess, clockwise bool variable to determine in which direction we have to rotate
    {
        if (currTetromino == 1)
            return;                                                   // O tetromino doesn't rotate
        vector<string> &shape = Tetrominoes.Tetromino[currTetromino]; // Reference to avoid copying
        vector<string> temp = shape;                                  // Copy of original state
        // Due to symmetry of I, S and Z Their code will be same for clockwise or anti-clockwise.
        if (currTetromino == 0) // I tetromino
        {
            if (temp[0] != "⬜") // If the position is horizontal make it vertical
            {
                temp = {
                    "⬜", "🟥", "⬜", "⬜",
                    "⬜", "🟥", "⬜", "⬜",
                    "⬜", "🟥", "⬜", "⬜",
                    "⬜", "🟥", "⬜", "⬜"};
            }
            else // If vertical make horizontal
            {
                temp = {
                    "🟥", "🟥", "🟥", "🟥",
                    "⬜", "⬜", "⬜", "⬜",
                    "⬜", "⬜", "⬜", "⬜",
                    "⬜", "⬜", "⬜", "⬜"};
            }
        }
        else if (currTetromino == 5) // S tetromino
        {
            if (temp[0] != "⬜") // If vertical make horizontal
            {
                temp = {
                    "⬜", "🟩", "🟩", "⬜",
                    "🟩", "🟩", "⬜", "⬜",
                    "⬜", "⬜", "⬜", "⬜",
                    "⬜", "⬜", "⬜", "⬜"};
            }
            else // If horizontal make vertical
            {
                temp = {
                    "🟩", "⬜", "⬜", "⬜",
                    "🟩", "🟩", "⬜", "⬜",
                    "⬜", "🟩", "⬜", "⬜",
                    "⬜", "⬜", "⬜", "⬜"};
            }
        }
        else if (currTetromino == 6) // Z tetromino
        {
            if (temp[0] != "⬜") // If horizontal make vertical
            {
                temp = {
                    "⬜", "🟦", "⬜", "⬜",
                    "🟦", "🟦", "⬜", "⬜",
                    "🟦", "⬜", "⬜", "⬜",
                    "⬜", "⬜", "⬜", "⬜"};
            }
            else // If vertical make horizontal
            {
                temp = {
                    "🟦", "🟦", "⬜", "⬜",
                    "⬜", "🟦", "🟦", "⬜",
                    "⬜", "⬜", "⬜", "⬜",
                    "⬜", "⬜", "⬜", "⬜"};
            }
        }
        else if (clockwise) // For other tetrominoes, code for clockwise rotation
        {
            for (int y = 0; y < 4; y++)
            {
                for (int x = 0; x < 4; x++)
                {
                    temp[y * 4 + x] = shape[(3 - x) * 4 + y];
                }
            }
        }
        else // For other tetrominoes, code for anti-clockwise rotation
        {
            for (int y = 0; y < 4; y++)
            {
                for (int x = 0; x < 4; x++)
                {
                    temp[y * 4 + x] = shape[x * 4 + 3 - y];
                }
            }
        }
        for (int y = 0; y < 4; y++)
        {
            for (int x = 0; x < 4; x++)
            {
                if (field[pieceY + y][pieceX + x] != "⬜" && temp[4 * y + x] != "⬜")
                {
                    return;
                }
            }
        }
        shape = temp;
        return;
    }
    void input() // Input function to handle inputs
    {
        if (_kbhit() && notHardDrop)
        {
            int ch = _getch();
            if (ch == 224) // Arrow keys return an extra byte (224) before actual keycode
            {
                ch = _getch();
                FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
                switch (ch)
                {
                case 75: // Left Arrow
                    if (canMove(pieceX - 1, pieceY))
                    {
                        pieceX--;
                    }
                    break;

                case 77: // Right Arrow
                    if (canMove(pieceX + 1, pieceY))
                    {
                        pieceX++;
                    }
                    break;

                case 80: // Down Arrow
                    if (canMove(pieceX, pieceY + 1))
                    {
                        pieceY++;
                    }
                    break;
                }
            }
            else
            {
                FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
                switch (ch)
                {
                case 'R':
                case 'r': // Rotate clockwise
                    rotateTetromino(true);
                    break;
                case 'L':
                case 'l': // Rotate counterclockwise
                    rotateTetromino(false);
                    break;
                case 'Q':
                case 'q': // Quit
                    gameOver = true;
                    break;
                case 32:
                    notHardDrop = false;
                    Score += 20;
                    noOfHardDrops++;
                    if (noOfHardDrops % 5 == 0)
                    {
                        level++;
                        speed -= 30;
                    }
                }
            }
        }
    }
    bool checkLanding() // checking if tetromino will land
    {
        // Assuming Tetromino has not fallen yet
        for (int y = 0; y < 4; y++)
        {
            for (int x = 0; x < 4; x++)
            {
                if (Tetrominoes.Tetromino[currTetromino][4 * y + x] != "⬜")
                {
                    if (field[pieceY + y + 1][pieceX + x] != "⬜")
                    {
                        return true;
                    }
                }
            }
        }
        return false;
    }
    void placeTetromino() // Placing tetromino on the board
    {
        for (int y = 0; y < 4; y++)
        {
            for (int x = 0; x < 4; x++)
            {
                if (Tetrominoes.Tetromino[currTetromino][4 * y + x] != "⬜" && pieceY + y < height - 1)
                {
                    field[pieceY + y][pieceX + x] = Tetrominoes.Tetromino[currTetromino][4 * y + x];
                }
            }
        }
    }
    void checkLineClearance() // Checking for full lines, removing them and adding points to the score
    {
        // scoreFactor to give more points, when more than 1 rows cleared at a time;
        int scoreFactor = 1;
        // we will check from bottom, for the full rows. and we will avoid borders.
        for (int y = height - 2; y >= 0; y--) // y = height - 1 is a border.
        {
            bool rowClear = true;
            for (int x = width - 2; x > 0; x--) // x = width - 1 and x = 0 is a border.
            {
                if (field[y][x] == "⬜")
                {
                    rowClear = false;
                    break;
                }
            }
            if (rowClear)
            {
                clearedRows += 1;
                if (clearedRows % 3 == 0)
                {
                    level++;
                    speed -= 30;
                }
                Score += scoreFactor * 100;
                scoreFactor++;
                // pushing all the row downs, and clearing the full row
                for (int temp = y; temp > 0; temp--)
                {
                    field[temp] = field[temp - 1];
                }
                // Manually setting first row blank
                for (int x = width - 2; x > 0; x--) // yaha pe sidha loop chalaate to int x = 0; x < width - 1 mai, constant width - 1 baar, baar compute hota
                {
                    field[0][x] = "⬜";
                }
                y++; // because in next loop y wil be decreamented, here we are doing this, so same row can be checked again.
            }
        }
    }
    void getNewTetromino() // Initializing new tetromino
    {
        // Generate new Tetromino
        if (!sameAllow1 && count1 % 5 == 0)
        {
            count1++;
            sameAllow1 = true;
        }
        if (!sameAllow2 && count2 % 3 == 0)
        {
            count1 = 0;
            count2++;
            sameAllow2 = true;
        }
        prev = currTetromino;
        currTetromino = nextTetromino;
        nextTetromino = rand() % 7;
        while(!sameAllow2 && prev == nextTetromino || !sameAllow1 && currTetromino == nextTetromino)
        {
            nextTetromino = rand() % 7;
        }
        pieceX = width / 2;
        pieceY = 0;
        // Check if the new Tetromino immediately collides
        for (int y = 0; y < 4; y++)
        {
            for (int x = 0; x < 4; x++)
            {
                for (int j = 0; j <= y; j++)
                {
                    if (Tetrominoes.Tetromino[currTetromino][4 * y + x] != "⬜" &&
                        field[pieceY + j][pieceX + x] != "⬜")
                        {
                            gameOver = true;
                        }
                }
            }
        }
        // Now place the Tetromino on the board before handling game over
        placeTetromino();
    }
    void displayBoard() // Displaying board
    {
        gotoxy(0, 0);
        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < width; x++)
            {
                cout << field[y][x];
            }
            cout << endl;
        }
    }
    void displayNextTetromino() // displaying next tetromino on top right of the board
    {
        gotoxy(2 * width + 4, height - 5);
        cout << "\033[1mNext:\033[0m";
        for (int y = 0; y < 4; y++)
        {
            for (int x = 0; x < 4; x++)
            {
                gotoxy(2 * (width + x + 2), height - 4 + y);
                cout << Tetrominoes.Tetromino[nextTetromino][4 * y + x];
            }
        }
    }
    void logic() // HEart of the code: logic function
    {
        // 1. Clearing the previous position of Tetromino
        clearBoard();

        // 2. getting the input
        input();

        // 3. Checking if Tetromino has landed
        bool landed = checkLanding();

        // 4. If not landed, move the Tetromino down.
        if (!landed)
        {
            pieceY++;
        }

        // 5. Placing the Tetromino on the board
        placeTetromino();

        // 6. If the Tetromino has landed, get the new Tetromino and check for line clears.
        if (landed)
        {
            checkLineClearance();
            if (!notHardDrop)
            {
                notHardDrop = true;
            }
            getNewTetromino();
        }
    }
    void displayScore() // Function to display the score
    {
        gotoxy(0, height + 1);
        cout << "Current Score:        " << Score << endl;
        cout << "This game High-Score: " << thisGameHighScore << endl;
        cout << "Overall High-Score:   " << overallHighScore << endl;
        cout << "Level: " << level;
    }
};

// main function
int main()
{
    SetConsoleOutputCP(CP_UTF8);
    system("cls");
    callWelcomeFunctions();
    Sleep(2000);
    system("cls");
    tetrisInstructionScreen();
    system("cls");
    FILE *ptr1 = fopen("high_score.txt", "r");
    fscanf(ptr1, "%d", &overallHighScore);
    fclose(ptr1);
    srand(time(0));
    while (playing)
    {
        gameOver = false;
        gameBoard board(12, 24); // So that we can get playable field of 10 x 23
        system("cls"); // Clearing the screen Before every New Board.
        hidecursor();
        while (!gameOver)
        {
            board.logic();
            if (!gameOver)
            {
                board.displayBoard();
                board.displayNextTetromino();
            }
            board.displayScore();
            Sleep(notHardDrop * speed);
            if (gameOver)
            {
                endScreen(board.width, board.height);
                level = 1;
                noOfHardDrops = 0;
                speed = 300;
                Sleep(200);
            }
        }
        showcursor();
        gotoxy(0, board.height + 4);
    }
    cout << endl;
    return 0;
}

// Function to hide the cursor
void hidecursor()
{
    CONSOLE_CURSOR_INFO cursorInfo;
    cursorInfo.dwSize = 1;
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}

// Function to show the cursor
void showcursor()
{
    CONSOLE_CURSOR_INFO cursorInfo;
    cursorInfo.dwSize = 1;
    cursorInfo.bVisible = TRUE;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}

// Things to do once game is over
void endScreen(int width, int height)
{
    scoreMessage(height);
    if (Score >= thisGameHighScore)
    {
        thisGameHighScore = Score;
        if (overallHighScore < thisGameHighScore)
        {
            overallHighScore = thisGameHighScore;
        }
    }
    playAgainArt(width);
    quitArt(width);
    Sleep(1000);
    bool takeInput = true;
    char ch = 0;
    while (takeInput)
    {
        if (_kbhit())
        {
            ch = _getch();
        }
        if (ch == 'p' || ch == 'P')
        {
            takeInput = false;
        }
        else if (ch == 'Q' || ch == 'q')
        {
            FILE *ptr2 = fopen("high_score.txt", "w");
            fprintf(ptr2, "%d", overallHighScore);
            fclose(ptr2);
            playing = false;
            takeInput = false;
        }
    }
    Score = 0;
}

// Block-art for play again
void playAgainArt(int width)
{
    gotoxy(2 * width + 4, 0);
    cout << "⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜";
    gotoxy(2 * width + 4, 1);
    cout << "⬜⬜⬛⬛⬛⬜⬜⬛⬜⬜⬜⬜⬜⬜⬛⬜⬜⬜⬛⬜⬜⬜⬛⬜⬜⬜⬜⬛⬜⬜⬜⬜⬛⬛⬛⬛⬜⬜⬜⬜⬛⬜⬜⬜⬛⬛⬛⬛⬛⬜⬛⬜⬜⬜⬜⬛⬜⬜⬛⬛⬛⬜⬜⬜⬜⬛⬜⬛⬛⬛⬜⬜⬛⬜";
    gotoxy(2 * width + 4, 2);
    cout << "⬜⬜⬛⬜⬜⬛⬜⬛⬜⬜⬜⬜⬜⬛⬜⬛⬜⬜⬛⬜⬜⬜⬛⬜⬜⬜⬛⬜⬛⬜⬜⬛⬜⬜⬜⬜⬜⬜⬜⬛⬜⬛⬜⬜⬜⬜⬛⬜⬜⬜⬛⬛⬜⬜⬜⬛⬜⬛⬜⬜⬜⬛⬜⬜⬜⬛⬜⬛⬜⬜⬛⬜⬛⬜";
    gotoxy(2 * width + 4, 3);
    cout << "⬜⬜⬛⬜⬜⬛⬜⬛⬜⬜⬜⬜⬛⬜⬜⬜⬛⬜⬜⬛⬜⬛⬜⬜⬜⬛⬜⬜⬜⬛⬜⬛⬜⬜⬜⬜⬜⬜⬛⬜⬜⬜⬛⬜⬜⬜⬛⬜⬜⬜⬛⬛⬛⬜⬜⬛⬜⬛⬜⬜⬜⬛⬜⬜⬜⬜⬜⬛⬜⬜⬛⬜⬜⬜";
    gotoxy(2 * width + 4, 4);
    cout << "⬜⬜⬛⬛⬛⬜⬜⬛⬜⬜⬜⬜⬛⬛⬛⬛⬛⬜⬜⬜⬛⬜⬜⬜⬜⬛⬛⬛⬛⬛⬜⬛⬜⬜⬜⬜⬜⬜⬛⬛⬛⬛⬛⬜⬜⬜⬛⬜⬜⬜⬛⬜⬛⬛⬜⬛⬜⬜⬜⬜⬛⬜⬜⬜⬜⬜⬜⬛⬛⬛⬜⬜⬜⬜";
    gotoxy(2 * width + 4, 5);
    cout << "⬜⬜⬛⬜⬜⬜⬜⬛⬜⬜⬜⬜⬛⬜⬜⬜⬛⬜⬜⬜⬛⬜⬜⬜⬜⬛⬜⬜⬜⬛⬜⬛⬜⬜⬛⬛⬛⬜⬛⬜⬜⬜⬛⬜⬜⬜⬛⬜⬜⬜⬛⬜⬜⬛⬛⬛⬜⬜⬜⬛⬜⬜⬜⬜⬜⬜⬜⬛⬜⬜⬜⬜⬜⬜";
    gotoxy(2 * width + 4, 6);
    cout << "⬜⬜⬛⬜⬜⬜⬜⬛⬜⬜⬜⬜⬛⬜⬜⬜⬛⬜⬜⬜⬛⬜⬜⬜⬜⬛⬜⬜⬜⬛⬜⬛⬜⬜⬜⬛⬜⬜⬛⬜⬜⬜⬛⬜⬜⬜⬛⬜⬜⬜⬛⬜⬜⬜⬛⬛⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬛⬜⬜⬜⬜⬜⬜";
    gotoxy(2 * width + 4, 7);
    cout << "⬜⬜⬛⬜⬜⬜⬜⬛⬛⬛⬛⬜⬛⬜⬜⬜⬛⬜⬜⬜⬛⬜⬜⬜⬜⬛⬜⬜⬜⬛⬜⬜⬛⬛⬛⬛⬜⬜⬛⬜⬜⬜⬛⬜⬛⬛⬛⬛⬛⬜⬛⬜⬜⬜⬜⬛⬜⬜⬜⬛⬜⬜⬜⬜⬜⬜⬜⬛⬜⬜⬜⬜⬜⬜";
    gotoxy(2 * width + 4, 8);
    cout << "⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜";
}

// Block-art for quit
void quitArt(int width)
{
    gotoxy(2 * width + 4, 9);
    cout << "⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛";
    gotoxy(2 * width + 4, 10);
    cout << "⬛⬛⬛💀💀💀⬛⬛⬛⬛💀⬛⬛⬛⬛💀⬛💀💀💀💀💀⬛💀💀💀💀💀⬛⬛⬛⬛💀⬛⬛💀💀💀⬛⬛💀⬛⬛";
    gotoxy(2 * width + 4, 11);
    cout << "⬛⬛💀⬛⬛⬛💀⬛⬛⬛💀⬛⬛⬛⬛💀⬛⬛⬛💀⬛⬛⬛⬛⬛💀⬛⬛⬛⬛⬛⬛💀⬛💀⬛⬛⬛💀⬛💀⬛⬛";
    gotoxy(2 * width + 4, 12);
    cout << "⬛⬛💀⬛⬛⬛💀⬛⬛⬛💀⬛⬛⬛⬛💀⬛⬛⬛💀⬛⬛⬛⬛⬛💀⬛⬛⬛⬛⬛⬛⬛⬛💀⬛⬛⬛💀⬛⬛⬛⬛";
    gotoxy(2 * width + 4, 13);
    cout << "⬛⬛💀⬛⬛⬛💀⬛⬛⬛💀⬛⬛⬛⬛💀⬛⬛⬛💀⬛⬛⬛⬛⬛💀⬛⬛⬛⬛⬛⬛⬛⬛💀⬛⬛⬛💀⬛⬛⬛⬛";
    gotoxy(2 * width + 4, 14);
    cout << "⬛⬛💀⬛💀⬛💀⬛⬛⬛💀⬛⬛⬛⬛💀⬛⬛⬛💀⬛⬛⬛⬛⬛💀⬛⬛⬛⬛⬛⬛⬛⬛💀⬛💀⬛💀⬛⬛⬛⬛";
    gotoxy(2 * width + 4, 15);
    cout << "⬛⬛💀⬛⬛💀💀⬛⬛⬛💀⬛⬛⬛⬛💀⬛⬛⬛💀⬛⬛⬛⬛⬛💀⬛⬛⬛⬛⬛⬛⬛⬛💀⬛⬛💀💀⬛⬛⬛⬛";
    gotoxy(2 * width + 4, 16);
    cout << "⬛⬛⬛💀💀💀💀⬛⬛⬛💀⬛⬛⬛⬛💀⬛⬛⬛💀⬛⬛⬛⬛⬛💀⬛⬛⬛⬛⬛⬛⬛⬛⬛💀💀💀💀⬛⬛⬛⬛";
    gotoxy(2 * width + 4, 17);
    cout << "⬛⬛⬛⬛⬛⬛⬛💀⬛⬛⬛💀💀💀💀⬛⬛💀💀💀💀💀⬛⬛⬛💀⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛💀⬛⬛⬛";
    gotoxy(2 * width + 4, 18);
    cout << "⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛";
}

void scoreMessage(int height)
{
    gotoxy(0, height + 6);
    if (Score > overallHighScore)
    {
        // 🎉🎊🧨✨🎆🎇🎈🎀🗿💩🤡🦾🤓👍
        cout << "✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨" << endl;
        cout << "✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨" << endl;
        // cout << "🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉" << endl;
        // cout << "🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉" << endl;
        cout << "✨✨ CONGRATULATIONS!!! YOU JUST BROKE THE OVERALL HIGH-SCORE  ✨✨" << endl;
        cout << "✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨" << endl;
        cout << "✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨" << endl;
        // cout << "🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉" << endl;
        // cout << "🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉" << endl;
    }
    else if (Score == overallHighScore)
    {
        cout << "🗿🗿🗿🗿🗿🗿🗿🗿🗿🗿🗿🗿🗿🗿🗿🗿🗿🗿🗿🗿🗿🗿🗿🗿🗿🗿🗿🗿🗿" << endl;
        cout << "🗿🗿🗿🗿🗿🗿🗿🗿🗿🗿🗿🗿🗿🗿🗿🗿🗿🗿🗿🗿🗿🗿🗿🗿🗿🗿🗿🗿🗿" << endl;
        cout << "🗿🗿 YOU TIED UP WITH OVERALL HIGH-SCORE! TRUE SIGMA! 🗿🗿" << endl;
        cout << "🗿🗿🗿🗿🗿🗿🗿🗿🗿🗿🗿🗿🗿🗿🗿🗿🗿🗿🗿🗿🗿🗿🗿🗿🗿🗿🗿🗿🗿" << endl;
        cout << "🗿🗿🗿🗿🗿🗿🗿🗿🗿🗿🗿🗿🗿🗿🗿🗿🗿🗿🗿🗿🗿🗿🗿🗿🗿🗿🗿🗿🗿" << endl;
    }
    else if (Score > thisGameHighScore)
    {
        cout << "🎊🎊🎊🎊🎊🎊🎊🎊🎊🎊🎊🎊🎊🎊🎊🎊🎊🎊🎊🎊🎊🎊🎊🎊🎊🎊🎊🎊🎊🎊🎊🎊" << endl;
        cout << "🎊🎊🎊🎊🎊🎊🎊🎊🎊🎊🎊🎊🎊🎊🎊🎊🎊🎊🎊🎊🎊🎊🎊🎊🎊🎊🎊🎊🎊🎊🎊🎊" << endl;
        cout << "🎊🎊 You just Broke your This Game High-Score! Keep it up! 🎊🎊" << endl;
        cout << "🎊🎊🎊🎊🎊🎊🎊🎊🎊🎊🎊🎊🎊🎊🎊🎊🎊🎊🎊🎊🎊🎊🎊🎊🎊🎊🎊🎊🎊🎊🎊🎊" << endl;
        cout << "🎊🎊🎊🎊🎊🎊🎊🎊🎊🎊🎊🎊🎊🎊🎊🎊🎊🎊🎊🎊🎊🎊🎊🎊🎊🎊🎊🎊🎊🎊🎊🎊" << endl;
    }
    else if (Score == thisGameHighScore)
    {
        cout << "🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾" << endl;
        cout << "🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾" << endl;
        cout << "🦾🦾 Tying up with This Game High-Score is not a big deal, Compete with Overall High-Score 🦾🦾" << endl;
        cout << "🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾" << endl;
        cout << "🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾🦾" << endl;
    }
    else if (Score == 0)
    {
        cout << "💩💩💩💩💩💩💩💩💩💩💩💩💩💩💩💩💩💩💩💩💩💩💩💩💩💩💩💩💩💩💩💩💩💩💩💩💩💩" << endl;
        cout << "💩💩💩💩💩💩💩💩💩💩💩💩💩💩💩💩💩💩💩💩💩💩💩💩💩💩💩💩💩💩💩💩💩💩💩💩💩💩" << endl;
        cout << "💩💩 Your Gameplay Skill is just like a border of this text NOOOOOOB!!! 💩💩" << endl;
        cout << "💩💩💩💩💩💩💩💩💩💩💩💩💩💩💩💩💩💩💩💩💩💩💩💩💩💩💩💩💩💩💩💩💩💩💩💩💩💩" << endl;
        cout << "💩💩💩💩💩💩💩💩💩💩💩💩💩💩💩💩💩💩💩💩💩💩💩💩💩💩💩💩💩💩💩💩💩💩💩💩💩💩" << endl;
    }
    else if (Score < 401)
    {
        cout << "🤡🤡🤡🤡🤡🤡🤡🤡🤡🤡🤡🤡🤡🤡🤡🤡🤡🤡🤡🤡🤡🤡🤡🤡🤡🤡🤡🤡🤡🤡🤡🤡🤡🤡🤡" << endl;
        cout << "🤡🤡🤡🤡🤡🤡🤡🤡🤡🤡🤡🤡🤡🤡🤡🤡🤡🤡🤡🤡🤡🤡🤡🤡🤡🤡🤡🤡🤡🤡🤡🤡🤡🤡🤡" << endl;
        cout << "🤡🤡 Improve your Skills! 4th class kid is better than you NOOB!!! 🤡🤡" << endl;
        cout << "🤡🤡🤡🤡🤡🤡🤡🤡🤡🤡🤡🤡🤡🤡🤡🤡🤡🤡🤡🤡🤡🤡🤡🤡🤡🤡🤡🤡🤡🤡🤡🤡🤡🤡🤡" << endl;
        cout << "🤡🤡🤡🤡🤡🤡🤡🤡🤡🤡🤡🤡🤡🤡🤡🤡🤡🤡🤡🤡🤡🤡🤡🤡🤡🤡🤡🤡🤡🤡🤡🤡🤡🤡🤡" << endl;
    }
    else if (Score < 801)
    {
        cout << "🤓🤓🤓🤓🤓🤓🤓🤓🤓🤓🤓🤓🤓🤓🤓🤓🤓🤓🤓🤓🤓🤓🤓🤓🤓🤓🤓🤓🤓🤓🤓🤓🤓🤓🤓🤓🤓🤓🤓🤓🤓" << endl;
        cout << "🤓🤓🤓🤓🤓🤓🤓🤓🤓🤓🤓🤓🤓🤓🤓🤓🤓🤓🤓🤓🤓🤓🤓🤓🤓🤓🤓🤓🤓🤓🤓🤓🤓🤓🤓🤓🤓🤓🤓🤓🤓" << endl;
        cout << "🤓🤓 Not that great performance to be honest, You are not that much PRO NERD! 🤓🤓" << endl;
        cout << "🤓🤓🤓🤓🤓🤓🤓🤓🤓🤓🤓🤓🤓🤓🤓🤓🤓🤓🤓🤓🤓🤓🤓🤓🤓🤓🤓🤓🤓🤓🤓🤓🤓🤓🤓🤓🤓🤓🤓🤓🤓" << endl;
        cout << "🤓🤓🤓🤓🤓🤓🤓🤓🤓🤓🤓🤓🤓🤓🤓🤓🤓🤓🤓🤓🤓🤓🤓🤓🤓🤓🤓🤓🤓🤓🤓🤓🤓🤓🤓🤓🤓🤓🤓🤓🤓" << endl;
    }
    else
    {
        cout << "👍👍👍👍👍👍👍👍👍👍👍👍👍👍👍👍👍👍👍👍👍👍👍👍👍👍👍👍👍👍👍👍👍👍" << endl;
        cout << "👍👍👍👍👍👍👍👍👍👍👍👍👍👍👍👍👍👍👍👍👍👍👍👍👍👍👍👍👍👍👍👍👍👍" << endl;
        cout << "👍👍 You have a potential, But for now, Better luck next time. 👍👍" << endl;
        cout << "👍👍👍👍👍👍👍👍👍👍👍👍👍👍👍👍👍👍👍👍👍👍👍👍👍👍👍👍👍👍👍👍👍👍" << endl;
        cout << "👍👍👍👍👍👍👍👍👍👍👍👍👍👍👍👍👍👍👍👍👍👍👍👍👍👍👍👍👍👍👍👍👍👍" << endl;
    }
}

void tetrisInstructionScreen()
{
    cout << "\033[93m\n";
    cout << "\n";
    cout << "\t\t\t\t\t\t\t\t\t=\033[91m-\033[93m=\033[91m-\033[93m=\033[91m-\033[93m=\033[91m-\033[93m=\033[91m-\033[93m=\033[91m-\033[93m=\033[91m-\033[93m=\033[91m-\033[93m=\033[91m-\033[93m=\033[91m-\033[93m=\033[91m-\033[93m=\033[91m-\033[93m=\033[91m-\033[93m=\033[91m-\033[93m=\033[91m-\033[93m=\033[91m-\033[93m=\033[91m-\033[93m=\033[91m-\033[93m=\033[91m-\033[93m=\033[91m-\033[93m=\033[91m-\033[93m=\033[91m-\033[93m=\033[91m-\033[93m=\033[91m-\033[93m=\n";
    cout << "\t\t\t\t\t\t\t\t\t\033[94m           🎮 TETRIS INSTRUCTIONS 🎮 \033[93m\n";
    cout << "\t\t\t\t\t\t\t\t\t=\033[91m-\033[93m=\033[91m-\033[93m=\033[91m-\033[93m=\033[91m-\033[93m=\033[91m-\033[93m=\033[91m-\033[93m=\033[91m-\033[93m=\033[91m-\033[93m=\033[91m-\033[93m=\033[91m-\033[93m=\033[91m-\033[93m=\033[91m-\033[93m=\033[91m-\033[93m=\033[91m-\033[93m=\033[91m-\033[93m=\033[91m-\033[93m=\033[91m-\033[93m=\033[91m-\033[93m=\033[91m-\033[93m=\033[91m-\033[93m=\033[91m-\033[93m=\033[91m-\033[93m=\033[91m-\033[93m=\033[91m-\033[93m=\n";
    cout << "\033[92m\n";
    cout << "\t\t\t\t\t\t\t\t\t📌 \033[95mControls:\n";
    cout << "\t\t\t\t\t\t\t\t\t   \033[92m👉 Press \033[94mAny Key\033[92m to start the game:\n";
    cout << "\t\t\t\t\t\t\t\t\t   \033[92m👉 Use the \033[94mArrow Keys\033[92m to move the tetromino:\n";
    cout << "\t\t\t\t\t\t\t\t\t      \033[94mLeft Arrow\033[92m: Move Left,\n";
    cout << "\t\t\t\t\t\t\t\t\t      \033[94mRight Arrow\033[92m: Move Right,\n"; 
    cout << "\t\t\t\t\t\t\t\t\t      \033[94mDown Arrow\033[92m: Soft Drop\n"; 
    cout << "\t\t\t\t\t\t\t\t\t   \033[92m👉 Press \033[94m'R'\033[92m to Rotate Clockwise, \033[94m'L'\033[92m to Rotate Counterclockwise\n";
    cout << "\t\t\t\t\t\t\t\t\t   \033[92m👉 Press \033[94mSpace\033[92m for Hard Drop\n";
    cout << "\t\t\t\t\t\t\t\t\t   \033[92m👉 Press \033[94m'Q'\033[92m to Quit\n";
    cout << "\t\t\t\t\t\t\t\t\t   \033[92m👉 Press \033[94m'P'\033[92m to Play again\n";
    cout << "\n";
    cout << "\t\t\t\t\t\t\t\t\t📌 \033[95mObjective:\n";
    cout << "\t\t\t\t\t\t\t\t\t   \033[92m👉 Clear complete horizontal lines to score points\n";
    cout << "\n";
    cout << "\t\t\t\t\t\t\t\t\t📌 \033[95mAdditional Info:\n";
    cout << "\t\t\t\t\t\t\t\t\t   \033[92m👉 A preview of the next tetromino is displayed\n";
    cout << "\t\t\t\t\t\t\t\t\t   \033[92m👉 The game ends if a new tetromino collides immediately upon spawning\n";
    cout << "\t\t\t\t\t\t\t\t\t   \033[92m👉 You will be awarded a special title based on your score.\n";
    cout << "\n";
    cout << "\t\t\t\t\t\t\t\t\t\033[93m=\033[91m-\033[93m=\033[91m-\033[93m=\033[91m-\033[93m=\033[91m-\033[93m=\033[91m-\033[93m=\033[91m-\033[93m=\033[91m-\033[93m=\033[91m-\033[93m=\033[91m-\033[93m=\033[91m-\033[93m=\033[91m-\033[93m=\033[91m-\033[93m=\033[91m-\033[93m=\033[91m-\033[93m=\033[91m-\033[93m=\033[91m-\033[93m=\033[91m-\033[93m=\033[91m-\033[93m=\033[91m-\033[93m=\033[91m-\033[93m=\033[91m-\033[93m=\033[91m-\033[93m=\033[91m-\033[93m=\n";    cout << "\033[0m";
    while (true)
    {
        if (_kbhit())
        {
            break;
        }
    }
}
