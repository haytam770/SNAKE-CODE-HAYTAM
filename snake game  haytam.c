#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>

#define cols 25
#define rows 25
#define TOTAL_FOOD 50

char gameBoard[cols * rows];//An array of characters representing the game board.
//where each character represents a position on the board.625 elements.(25*25)
int isGameEnded = 0;//A flag to track whether the game is over.

void setupGameField() {//Sets up the initial game field, drawing the boundaries of the game.

    int x, y;

    for (y = 0; y < rows; y++) {
        for (x = 0; x < cols; x++) {
            if (x == 0 || y == 0 || x == cols - 1 || y == rows - 1) {
                gameBoard[y * cols + x] = '#';
            } else {
                gameBoard[y * cols + x] = ' ';
            }
        }
    }
}

void refreshGameScreen() {
    system("cls");   // For Windows, use 'clear' for Linux/Mac
}

void showPlayField() {
    int x, y;

    for (y = 0; y < rows; y++) {
        for (x = 0; x < cols; x++) {
            putch(gameBoard[y * cols + x]);//putch: Prints a single character.
        }
        putch('\n');//Each row is printed followed by a newline (\n) to make the board look like a grid.
    }
}

#define snakeMaxLength 256
struct SnakePart {
    int x, y;
};
struct Snake {
    int length;
    struct SnakePart part[snakeMaxLength];// An array holding the coordinates of each snake part.
};
struct Snake snake;

struct Food {
    int x, y;
    int consumed;// A flag indicating whether the food has been eaten or not.
};
struct Food gameFood[TOTAL_FOOD];

void drawSnakeBody() {
    int i;

    for (i = snake.length - 1; i > 0; i--) {
        gameBoard[snake.part[i].y * cols + snake.part[i].x] = '#';
    }
    gameBoard[snake.part[0].y * cols + snake.part[0].x] = '@';
}

void updateSnakePosition(int deltaX, int deltaY) {
    int i;

    for (i = snake.length - 1; i > 0; i--) {
        snake.part[i] = snake.part[i - 1];
    }

    snake.part[0].x += deltaX;
    snake.part[0].y += deltaY;
}

void read_keyboard() {
    int ch = getch();

    switch (ch) {
        case 'w': updateSnakePosition(0, -1); break;
        case 's': updateSnakePosition(0, 1); break;
        case 'a': updateSnakePosition(-1, 0); break;
        case 'd': updateSnakePosition(1, 0); break;
    }
}

void draw_food() {
    int i;

    for (i = 0; i < TOTAL_FOOD; i++) {
        if (!gameFood[i].consumed) {
            gameBoard[gameFood[i].y * cols + gameFood[i].x] = '+';
        }
    }
}

void setup_food() {
    int i;

    for (i = 0; i < TOTAL_FOOD; i++) {
        gameFood[i].x = 1 + rand() % (cols - 2);
        gameFood[i].y = 1 + rand() % (rows - 2);
        gameFood[i].consumed = 0;// for marking that all the food is not consumed.
    }
}
//for setting up food at random positions on the game board.
//rand() % (cols - 2)ensures the food is not placed on the boundaries (x between 1 and cols-2).
void setup_snake() {
    snake.length = 1;
    snake.part[0].x = 1 + rand() % (cols - 2);
    snake.part[0].y = 1 + rand() % (rows - 2);
}
//Initializes the snake with a length of 1 and places it at a random position on the game board.
void game_rules() {
    int i;

    for (i = 0; i < TOTAL_FOOD; i++) {
        if (!gameFood[i].consumed) {
            if (gameFood[i].x == snake.part[0].x && gameFood[i].y == snake.part[0].y) {
                gameFood[i].consumed = 1;
                snake.length++;
            }
        }
    }

    if (snake.part[0].x == 0 || snake.part[0].x == cols - 1 ||
        snake.part[0].y == 0 || snake.part[0].y == rows - 1) {
        isGameEnded = 1;
    }

    for (i = 1; i < snake.length; i++) {
        if (snake.part[0].x == snake.part[i].x && snake.part[0].y == snake.part[i].y) {
            isGameEnded = 1;//Checks if the snake hits a wall or collides with its own body. If either happens, the game ends.
        }
    }
}

int main(int argc, char **argv) {
    srand(time(0));

    setup_snake();
    setup_food();

    while (!isGameEnded) {
        setupGameField();
        draw_food();
        drawSnakeBody();
        game_rules();
        refreshGameScreen();
        printf("Snake Game, Score: %d\n", snake.length * 100);
        showPlayField();
        if (!isGameEnded) read_keyboard();
    }

    printf("Game Over, Final score: %d\n", snake.length * 100);

    while (1) getch();

    return 0;
}
