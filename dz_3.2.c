/*
Задание 3. Пауза
Написать функцию, с помощью которой можно будет поставить игру на паузу
при нажатии клавиши ‘P’. Добавить пояснительную надпись на главный экран.

 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <unistd.h>
#include <stdint.h>

#define MAX_X 15
#define MAX_Y 15

typedef struct tail_t {
    int x;
    int y;
} tail_t;

typedef struct snake_t {
    int x;
    int y;
    struct tail_t *tail;
    size_t tsize;
} snake_t;

typedef struct food_t {
    int x;
    int y;
} food_t;

#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3

int level = 0; // Global variable to keep track of the level

// Function to check if the direction is correct
int checkDirection(snake_t *snake, int32_t key) {
    if (snake->tsize == 0) {
        return 1;
    }

    int head_x = snake->x;
    int head_y = snake->y;

    int tail_x = snake->tail[0].x;
    int tail_y = snake->tail[0].y;

    if ((key == LEFT && head_x - 1 == tail_x && head_y == tail_y) ||
        (key == RIGHT && head_x + 1 == tail_x && head_y == tail_y) ||
        (key == UP && head_y - 1 == tail_y && head_x == tail_x) ||
        (key == DOWN && head_y + 1 == tail_y && head_x == tail_x)) {
        return 0;
    }

    return 1;
}

// Initialize the snake
struct snake_t initSnake(int x, int y, size_t tsize) {
    struct snake_t snake;
    snake.x = x;
    snake.y = y;
    snake.tsize = tsize;
    snake.tail = (tail_t *)malloc(sizeof(tail_t) * 100);
    for (int i = 0; i < tsize; ++i) {
        snake.tail[i].x = x + i + 1;
        snake.tail[i].y = y;
    }
    return snake;
}

// Generate food in a random position
food_t generateFood() {
    food_t food;
    food.x = rand() % MAX_X;
    food.y = rand() % MAX_Y;
    return food;
}

// Check if the snake has eaten the food
int checkFoodCollision(snake_t *snake, food_t *food) {
    if (snake->x == food->x && snake->y == food->y) {
        level++;
        return 1;
    }
    return 0;
}

// Move the snake left
snake_t moveLeft(snake_t snake) {
    for (int i = snake.tsize - 1; i > 0; i--) {
        snake.tail[i] = snake.tail[i - 1];
    }
    snake.tail[0].x = snake.x;
    snake.tail[0].y = snake.y;

    snake.x = snake.x - 1;
    if (snake.x < 0) {
        snake.x = MAX_X - 1;
    }
    return snake;
}

// Move the snake right
snake_t moveRight(snake_t snake) {
    for (int i = snake.tsize - 1; i > 0; i--) {
        snake.tail[i] = snake.tail[i - 1];
    }
    snake.tail[0].x = snake.x;
    snake.tail[0].y = snake.y;

    snake.x = snake.x + 1;
    if (snake.x >= MAX_X) {
        snake.x = 0;
    }
    return snake;
}

// Move the snake up
snake_t moveUp(snake_t snake) {
    for (int i = snake.tsize - 1; i > 0; i--) {
        snake.tail[i] = snake.tail[i - 1];
    }
    snake.tail[0].x = snake.x;
    snake.tail[0].y = snake.y;

    snake.y = snake.y - 1;
    if (snake.y < 0) {
        snake.y = MAX_Y - 1;
    }
    return snake;
}

// Move the snake down
snake_t moveDown(snake_t snake) {
    for (int i = snake.tsize - 1; i > 0; i--) {
        snake.tail[i] = snake.tail[i - 1];
    }
    snake.tail[0].x = snake.x;
    snake.tail[0].y = snake.y;

    snake.y = snake.y + 1;
    if (snake.y >= MAX_Y) {
        snake.y = 0;
    }
    return snake;
}

// Print the snake and the food
void printSnake(struct snake_t snake, struct food_t food) {
    char matrix[MAX_X][MAX_Y];
    for (int i = 0; i < MAX_X; ++i) {
        for (int j = 0; j < MAX_Y; ++j) {
            matrix[i][j] = ' ';
        }
    }

    matrix[snake.x][snake.y] = '@';
    for (int i = 0; i < snake.tsize; ++i) {
        matrix[snake.tail[i].x][snake.tail[i].y] = '*';
    }

    matrix[food.x][food.y] = 'F'; // Display food as 'F'

    for (int j = 0; j < MAX_Y; ++j) {
        for (int i = 0; i < MAX_X; ++i) {
            printf("%c", matrix[i][j]);
        }
        printf("\n");
    }

    printf("Level: %d\n", level); // Print current level
    printf("Press 'P' to pause/resume the game.\n"); // Instructions to pause/resume
}

// Print the level during the game
void printLevel(struct snake_t *snake) {
    printf("Current level: %d\n", level);
}

// Print final result when the game ends
void printExit(struct snake_t *snake) {
    printf("Game over! Final level: %d\n", level);
}

void togglePause() {
    printf("\nGame paused. Press 'P' to resume.\n");
    while (1) {
        int32_t key = _getch();
        if (key == 'p' || key == 'P') {
            break; // Resume the game
        }
    }
}

int main() {
    struct snake_t snake = initSnake(10, 5, 2);
    food_t food = generateFood(); // Generate initial food position
    printSnake(snake, food);

    int32_t direction = LEFT; // Initial direction

    while (1) {
        int32_t key;

        // Wait for user input
        if (_kbhit()) {
            key = _getch();

            // Check if 'P' was pressed to pause the game
            if (key == 'p' || key == 'P') {
                togglePause();
                printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n"); // Clear screen
                printSnake(snake, food);
                continue; // Continue the game loop
            }

            // Convert user input to direction
            switch (key) {
                case 'w':
                    key = UP;
                    break;
                case 's':
                    key = DOWN;
                    break;
                case 'a':
                    key = LEFT;
                    break;
                case 'd':
                    key = RIGHT;
                    break;
                default:
                    continue;
            }

            // If direction is correct, update movement direction
            if (checkDirection(&snake, key)) {
                direction = key;
            }
        }

        // Update snake position based on the current direction
        switch (direction) {
            case LEFT:
                snake = moveLeft(snake);
                break;
            case RIGHT:
                snake = moveRight(snake);
                break;
            case UP:
                snake = moveUp(snake);
                break;
            case DOWN:
                snake = moveDown(snake);
                break;
        }

        // Check for food collision
        if (checkFoodCollision(&snake, &food)) {
            snake.tsize++;
            food = generateFood(); // Generate new food position
        }

        printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n"); // Clear screen
        printSnake(snake, food);

        usleep(100000); // Delay to control snake speed
    }

    printExit(&snake); // Game over, print final result

    return 0;
}
