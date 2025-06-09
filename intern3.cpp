#include <iostream>
#include <deque>
#include <vector>
#include <chrono>
#include <thread>
#include <cstdlib>
#include <ctime>

const int WIDTH = 40;
const int HEIGHT = 20;

enum Direction { STOP = 0, LEFT, RIGHT, UP, DOWN };

struct Position {
    int x, y;
    bool operator==(const Position &other) const {
        return x == other.x && y == other.y;
    }
};

Direction dir;
Position food;
std::deque<Position> snake;
bool gameOver;
int score;

void Setup() {
    gameOver = false;
    dir = STOP;
    snake.clear();
    snake.push_back({WIDTH / 2, HEIGHT / 2});
    food = {rand() % WIDTH, rand() % HEIGHT};
    score = 0;
}

void ClearScreen() {
    std::cout << "\033[2J\033[1;1H";
}

void Draw() {
    ClearScreen();
    for (int i = 0; i < WIDTH + 2; i++) std::cout << "#";
    std::cout << std::endl;

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (x == 0) std::cout << "#";

            bool printed = false;
            if (x == snake.front().x && y == snake.front().y) {
                std::cout << "O";
                printed = true;
            } else {
                for (size_t i = 1; i < snake.size(); i++) {
                    if (snake[i].x == x && snake[i].y == y) {
                        std::cout << "o";
                        printed = true;
                        break;
                    }
                }
            }

            if (!printed) {
                if (x == food.x && y == food.y)
                    std::cout << "F";
                else
                    std::cout << " ";
            }

            if (x == WIDTH - 1) std::cout << "#";
        }
        std::cout << std::endl;
    }

    for (int i = 0; i < WIDTH + 2; i++) std::cout << "#";
    std::cout << "\nScore: " << score << std::endl;
}

void Input() {
    std::cout << "Enter direction (WASD) or X to quit: ";
    char ch;
    std::cin >> ch;
    switch (ch) {
        case 'a': case 'A': dir = LEFT; break;
        case 'd': case 'D': dir = RIGHT; break;
        case 'w': case 'W': dir = UP; break;
        case 's': case 'S': dir = DOWN; break;
        case 'x': case 'X': gameOver = true; break;
        default: break;
    }
}

void Logic() {
    Position newHead = snake.front();

    switch (dir) {
        case LEFT: newHead.x--; break;
        case RIGHT: newHead.x++; break;
        case UP: newHead.y--; break;
        case DOWN: newHead.y++; break;
        default: return;
    }

    if (newHead.x < 0 || newHead.x >= WIDTH || newHead.y < 0 || newHead.y >= HEIGHT) {
        gameOver = true;
        return;
    }

    for (const auto &segment : snake) {
        if (newHead == segment) {
            gameOver = true;
            return;
        }
    }

    snake.push_front(newHead);

    if (newHead == food) {
        score += 10;
        food = {rand() % WIDTH, rand() % HEIGHT};
    } else {
        snake.pop_back();
    }
}

int main() {
    srand(static_cast<unsigned int>(time(0)));
    Setup();
    while (!gameOver) {
        Draw();
        Input();
        Logic();
    }
    std::cout << "Game Over! Final Score: " << score << std::endl;
    return 0;
}
