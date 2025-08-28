#include <iostream>
#include <cstdlib>
#include <ctime>
#include <iomanip>

#ifdef _WIN32
void clearScreen() {
    system("cls");
}
#else
void clearScreen() {
    system("clear");
}
#endif

class Ship {
public:
    int x, y;
    int health;

    Ship(int startX, int startY, int startHealth) : x(startX), y(startY), health(startHealth) {}

    void move(int d, int mapsize) {
        // 0: Left 1: Right 2: Up 3: Down

        // Check the direction and update coordinates accordingly
        if (d == 0 && y > 0) {
            y--;
        } else if (d == 1 && y < mapsize - 1) {
            y++;
        } else if (d == 2 && x > 0) {
            x--;
        } else if (d == 3 && x < mapsize - 1) {
            x++;
        }
    }

    void resetPosition(int startX, int startY) {
        x = startX;
        y = startY;
    }
};

class Game {
private:
    const int mapsize = 10;
    int mapIndices[10][10] = {0}; // Initialize mapIndices with zeros
    Ship player{0, 0, 3}; // Initialize player position and health
    int score;

public:
    void initializeGame() {
        // Initialize the game, set player position, and place enemies randomly
        srand(static_cast<unsigned>(time(0)));
        player.x = rand() % mapsize;
        player.y = rand() % mapsize;

        // Place enemies randomly
        for (int i = 0; i <= mapsize; ++i) {
            int enemyX = rand() % mapsize;
            int enemyY = rand() % mapsize;
            mapIndices[enemyX][enemyY] = 2; // Mark enemy position
        }

        score = 0;
    }

    void updateGame() {
        // Update the game logic
    }

    void gameOver() {
        // Handle game over
        clearScreen(); // Clear the screen
        std::cout << "MISSION FAILED!" << std::endl;
        std::cout << "Your score: " << score << std::endl;
    }

    void gameWin() {
        // Handle game win
        clearScreen(); // Clear the screen
        std::cout << "Congratulations! MISSION ACCOMPLISHED!" << std::endl;
        std::cout << "Your score: " << score << std::endl;
    }

    void renderGame() {
        // Render the game
        clearScreen(); // Clear the screen before rendering

        // Calculate padding to center the board
        int padding = (80 - 4 * mapsize) / 2;

        for (int i = 0; i < mapsize; i++) {
            for (int p = 0; p < padding; p++)
                std::cout << " ";
            
            for (int k = 0; k < mapsize; k++)
                std::cout << " ---";
            std::cout << std::endl;

            for (int p = 0; p < padding; p++)
                std::cout << " ";

            for (int j = 0; j < mapsize; j++) {
                std::cout << "|";
                if (player.x == i && player.y == j)
                    std::cout << " O "; // player
                else if (mapIndices[i][j] == 2)
                    std::cout << " * "; // enemy
                else
                    std::cout << "   "; // blank space
            }

            std::cout << "|" << std::endl;
        }

        for (int p = 0; p < padding; p++)
            std::cout << " ";

        for (int k = 0; k < mapsize; k++)
            std::cout << " ---";
        std::cout << std::endl;

        std::cout << std::setw(padding) << " "; // Add padding for player status
        std::cout << "Player Health: " << player.health << " | Score: " << score << std::endl;
    }

    bool isGameOver() {
        return player.health <= 0;
    }

    bool allEnemiesDestroyed() const {
        for (int i = 0; i < mapsize; ++i) {
            for (int j = 0; j < mapsize; ++j) {
                if (mapIndices[i][j] == 2) {
                    return false; // There is an enemy remaining
                }
            }
        }
        return true; // No enemies remaining
    }

    void handleInput(char input) {
        // Handle user input
        if (input == 'S') {
            shoot();
        } else {
            int direction;
            switch (input) {
                case 'L':
                    direction = 0; // Left
                    break;
                case 'R':
                    direction = 1; // Right
                    break;
                case 'U':
                    direction = 2; // Up
                    break;
                case 'D':
                    direction = 3; // Down
                    break;
                default:
                    direction = -1; // Invalid direction
                    break;
            }

            if (direction != -1) {
                move(direction);
            }
        }
    }

    void move(int direction) {
        // Store the player's initial position before moving
        int initialX = player.x;
        int initialY = player.y;

        player.move(direction, mapsize);

        // Check for collision with an enemy after each move
        if (mapIndices[player.x][player.y] == 2) {
            std::cout << "Collision with enemy! Lose one health." << std::endl;
            player.health--;

            // Check if player health is zero
            if (player.health <= 0) {
                gameOver();
                return;
            }

            // Reset player position to the initial position after collision
            player.resetPosition(initialX, initialY);
        }
    }

    void shoot() {
        // Find the nearest enemy in the same row
        int nearestEnemy = -1;
        int minDistance = mapsize;

        for (int j = 0; j < mapsize; j++) {
            if (mapIndices[player.x][j] == 2) {
                int distance = std::abs(j - player.y);

                if (distance < minDistance) {
                    minDistance = distance;
                    nearestEnemy = j;
                }
            }
        }

        if (nearestEnemy != -1) {
            std::cout << "Shooting! Nearest enemy destroyed!" << std::endl;
            mapIndices[player.x][nearestEnemy] = 0; // Mark nearest enemy position as destroyed

            // Increase the score after each successful shot
            score++;

            // Check if all enemies are destroyed
            if (allEnemiesDestroyed()) {
                gameWin();
            }
        } else {
            // If no enemy found in the row
            std::cout << "No enemy in sight. Missed the shot!" << std::endl;
        }
    }
};

int main() {
    char playAgainInput;

    do {
        std::cout << "Welcome to Star Wars!" << "\n" << " Would you like to start? (Y/N): ";
        char startInput;
        std::cin >> startInput;

        if (startInput != 'Y' && startInput != 'y') {
            std::cout << "We Hope To Meet You Again!" << std::endl;
            return 0;
        }

        Game game;
        game.initializeGame();

        char userInput;
        bool isGameOver = false;

        do {
            game.renderGame();
            std::cout << "\t\t" << "    " << "Enter move (L/R/U/D/S): ";
            std::cin >> userInput;

            game.handleInput(userInput);
            game.updateGame();

            if (game.isGameOver() || game.allEnemiesDestroyed()) {
                isGameOver = true;
                break;
            }

            static int moveCount = 0;
            if (++moveCount >= 1000) {
                isGameOver = true;
            }

        } while (!isGameOver);

        std::cout << "Do you want to play again? (Y/N): ";
        std::cin >> playAgainInput;

    } while (playAgainInput == 'Y' || playAgainInput == 'y');

    std::cout << "Thanks for playing! We Hope To Meet You Again!" << std::endl;

    return 0;
}
