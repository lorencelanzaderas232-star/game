#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
using namespace sf;
using namespace std;

const int TILE = 32;

vector<string> maze = {
    "####################",
    "#........##........#",
    "#.##.###.##.###.##.#",
    "#..................#",
    "#.##.#.######.#.##.#",
    "#....#...##...#....#",
    "####.###.##.###.####",
    "#..................#",
    "#.##.##.####.##.##.#",
    "#....##......##....#",
    "####.##.####.##.####",
    "#..................#",
    "#.##.###.##.###.##.#",
    "#........##........#",
    "####################"
};

int score = 0;
int lives = 3;

struct Ghost {
    CircleShape shape;
    Vector2f direction;
};

int main() {
    RenderWindow window(
        VideoMode(640, 520),
        "PAC-MAN C++ GAME"
    );

    window.setFramerateLimit(60);

    CircleShape pacman(12);
    pacman.setFillColor(Color::Yellow);
    pacman.setPosition(48, 48);

    vector<Ghost> ghosts;

    Color ghostColors[] = {
        Color::Red, Color::Cyan,
        Color::Magenta, Color::Green
    };

    for (int i = 0; i < 4; i++) {
        Ghost g;
        g.shape.setRadius(12);
        g.shape.setFillColor(ghostColors[i]);
        g.shape.setPosition(280 + i * 32, 240);
        g.direction = Vector2f(2, 0);
        ghosts.push_back(g);
    }

    Font font;
    if (!font.loadFromFile("arial.ttf")) {
        cout << "Place arial.ttf beside the program.\n";
        return 1;
    }

    Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(20);
    scoreText.setFillColor(Color::White);
    scoreText.setPosition(20, 490);

    Clock clock;
    Vector2f movement(0, 0);

    while (window.isOpen()) {
        Event event;

        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();

            if (event.type == Event::KeyPressed) {
                if (event.key.code == Keyboard::Up)
                    movement = Vector2f(0, -3);

                if (event.key.code == Keyboard::Down)
                    movement = Vector2f(0, 3);

                if (event.key.code == Keyboard::Left)
                    movement = Vector2f(-3, 0);

                if (event.key.code == Keyboard::Right)
                    movement = Vector2f(3, 0);
            }
        }

        float dt = clock.restart().asSeconds();

        Vector2f next = pacman.getPosition()
                      + movement * (dt * 60.0f);

        int col = (int)(next.x + 12) / TILE;
        int row = (int)(next.y + 12) / TILE;

        if (row >= 0 && row < (int)maze.size() &&
            col >= 0 && col < (int)maze[0].size() &&
            maze[row][col] != '#') {

            pacman.setPosition(next);

            if (maze[row][col] == '.') {
                maze[row][col] = ' ';
                score += 10;
            }
        }

        // Move ghosts
        for (auto &g : ghosts) {
            Vector2f p = g.shape.getPosition();
            Vector2f n = p + g.direction;

            int c = (int)(n.x + 12) / TILE;
            int r = (int)(n.y + 12) / TILE;

            if (r < 0 || r >= (int)maze.size() ||
                c < 0 || c >= (int)maze[0].size() ||
                maze[r][c] == '#') {
                g.direction = Vector2f(
                    -g.direction.y,
                    g.direction.x
                );
            } else {
                g.shape.move(g.direction);
            }

            // Collision detection
            if (abs(pacman.getPosition().x -
                    g.shape.getPosition().x) < 20 &&
                abs(pacman.getPosition().y -
                    g.shape.getPosition().y) < 20) {

                lives--;
                pacman.setPosition(48, 48);

                if (lives <= 0) {
                    cout << "GAME OVER! Final score: "
                         << score << endl;
                    window.close();
                }
            }
        }

        window.clear(Color::Black);

        // Draw maze and dots
        for (int r = 0; r < (int)maze.size(); r++) {
            for (int c = 0; c < (int)maze[r].size(); c++) {

                RectangleShape tile(
                    Vector2f(TILE - 2, TILE - 2)
                );

                tile.setPosition(c * TILE, r * TILE);

                if (maze[r][c] == '#') {
                    tile.setFillColor(Color(20, 40, 200));
                    window.draw(tile);
                }

                if (maze[r][c] == '.') {
                    CircleShape dot(3);
                    dot.setFillColor(Color::White);
                    dot.setPosition(
                        c * TILE + 13,
                        r * TILE + 13
                    );
                    window.draw(dot);
                }
            }
        }

        // Draw Pac-Man
        window.draw(pacman);

        // Draw ghosts
        for (auto &g : ghosts)
            window.draw(g.shape);

        scoreText.setString(
            "Score: " + to_string(score) +
            "     Lives: " + to_string(lives)
        );

        window.draw(scoreText);
        window.display();
    }

    return 0;
}
