// Classic snake game. Inspired from googles own web version of snake. Michael Hussein CGT215 2024
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFPhysics.h>
#include <vector>
#include <cstdlib>
#include <ctime>


// Constants
const int windowWidth = 800;
const int windowHeight = 600;
const int gridSize = 20;
const int gridWidth = windowWidth / gridSize;
const int gridHeight = windowHeight / gridSize;

// For direction
enum class Direction { Up, Down, Left, Right };

struct SnakeSegment {
    int x, y;
    SnakeSegment(int x, int y) : x(x), y(y) {}
};

class SnakeGame {
private:
    sf::RenderWindow window;
    std::vector<SnakeSegment> snake;
    SnakeSegment food;
    Direction direction;
    bool isGameOver;
    int score;
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
    sf::Texture foodTexture;
    sf::Font font;

    void resetGame() {
        snake.clear();
        snake.push_back(SnakeSegment(gridWidth / 2, gridHeight / 2));
        direction = Direction::Right;
        spawnFood();
        isGameOver = false;
        score = 0;
    }

    void spawnFood() {
        food.x = rand() % gridWidth;
        food.y = rand() % gridHeight;
    }

    void update() {
        if (isGameOver) return;

        // Move the snake by adding a new head
        SnakeSegment newHead = snake.front();
        switch (direction) {
        case Direction::Up:    newHead.y--; break;
        case Direction::Down:  newHead.y++; break;
        case Direction::Left:  newHead.x--; break;
        case Direction::Right: newHead.x++; break;
        }

        // Check for collisions with walls
        if (newHead.x < 0 || newHead.x >= gridWidth || newHead.y < 0 || newHead.y >= gridHeight) {
            isGameOver = true;
            return;
        }

        // Check for collisions with itself
        for (const auto& segment : snake) {
            if (segment.x == newHead.x && segment.y == newHead.y) {
                isGameOver = true;
                return;
            }
        }

        // Add the new head
        snake.insert(snake.begin(), newHead);

        // Check if the snake ate the food
        if (newHead.x == food.x && newHead.y == food.y) {
            score++;
            spawnFood();
        }
        else {
            // Remove the tail if no food eaten
            snake.pop_back();
        }
    }

    void draw() {
        window.clear();

        // Draw the background
        backgroundSprite.setScale(
            static_cast<float>(windowWidth) / backgroundTexture.getSize().x,
            static_cast<float>(windowHeight) / backgroundTexture.getSize().y);
        window.draw(backgroundSprite);

        // Draw the snake
        for (const auto& segment : snake) {
            sf::RectangleShape rectangle(sf::Vector2f(gridSize - 1, gridSize - 1));
            rectangle.setPosition(segment.x * gridSize, segment.y * gridSize);
            rectangle.setFillColor(sf::Color::Green);
            window.draw(rectangle);
        }

        // Draw the food
        sf::RectangleShape foodShape(sf::Vector2f(gridSize - 1, gridSize - 1));
        foodShape.setPosition(food.x * gridSize, food.y * gridSize);
        foodShape.setTexture(&foodTexture);
        window.draw(foodShape);

        // Draw the score
        sf::Text scoreText("Score: " + std::to_string(score), font, 20);
        scoreText.setFillColor(sf::Color::White);
        scoreText.setPosition(10, 10);
        window.draw(scoreText);

        if (isGameOver) {
            sf::Text gameOverText("Game Over! Press R to Restart", font, 30);
            gameOverText.setFillColor(sf::Color::Red);
            gameOverText.setPosition(windowWidth / 4, windowHeight / 2);
            window.draw(gameOverText);
        }

        window.display();
    }

public:
    SnakeGame()
        : window(sf::VideoMode(windowWidth, windowHeight), "Enhanced Snake Game"),
        food(0, 0), direction(Direction::Right), isGameOver(false), score(0) {
        resetGame();

        // Load background texture
        if (!backgroundTexture.loadFromFile("background.png")) {
            throw std::runtime_error("Failed to load background image");
        }
        backgroundSprite.setTexture(backgroundTexture);

        // Load food texture
        if (!foodTexture.loadFromFile("food.png")) {
            throw std::runtime_error("Failed to load food image");
        }

        // Load font
        if (!font.loadFromFile("arial.ttf")) {
            throw std::runtime_error("Failed to load font");
        }
    }

    void run() {
        sf::Clock clock;
        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                }
                if (event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::Up && direction != Direction::Down) {
                        direction = Direction::Up;
                    }
                    else if (event.key.code == sf::Keyboard::Down && direction != Direction::Up) {
                        direction = Direction::Down;
                    }
                    else if (event.key.code == sf::Keyboard::Left && direction != Direction::Right) {
                        direction = Direction::Left;
                    }
                    else if (event.key.code == sf::Keyboard::Right && direction != Direction::Left) {
                        direction = Direction::Right;
                    }
                    else if (event.key.code == sf::Keyboard::R && isGameOver) {
                        resetGame();
                    }
                }
            }

            if (clock.getElapsedTime().asMilliseconds() > 100) {
                update();
                clock.restart();
            }

            draw();
        }
    }
};

int main() {
    srand(static_cast<unsigned>(time(0)));
    try {
        SnakeGame game;
        game.run();
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    return 0;
}
