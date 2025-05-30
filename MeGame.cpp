#include <SFML/Graphics.hpp> 
#include <vector>
#include <algorithm>
#include <iostream>
#include "Player.hpp"
#include "Enemy.hpp"
#include "Menu.hpp"

enum class GameState {
    MENU,
    GAME,
    LEVEL_FINISHED,
    GAME_OVER
};

// Функция рисования текста с обводкой
void drawTextWithOutline(sf::RenderWindow& window, const sf::Text& text, sf::Color outlineColor, float outlineThickness = 2.f) {
    sf::Text outlineText = text;
    outlineText.setFillColor(outlineColor);

    for (float xOff = -outlineThickness; xOff <= outlineThickness; xOff += outlineThickness) {
        for (float yOff = -outlineThickness; yOff <= outlineThickness; yOff += outlineThickness) {
            if (xOff == 0 && yOff == 0) continue;
            outlineText.setPosition(text.getPosition().x + xOff, text.getPosition().y + yOff);
            window.draw(outlineText);
        }
    }
    window.draw(text);
}

void setupLevel(int level, std::vector<Enemy>& enemies, sf::Sprite& bgSprite, sf::Texture bgTextures[]) {
    enemies.clear();
    for (int i = 0; i < level; ++i) {
        float x = 100.f + i * 150.f;
        float y = 300.f;
        enemies.emplace_back(sf::Vector2f(x, y));
        std::cout << "Enemy #" << i + 1 << " created at position: " << x << ", " << y << std::endl;
    }
    if (level >= 1 && level <= 3) {
        bgSprite.setTexture(bgTextures[level - 1]);
    }
    std::cout << "Total enemies: " << enemies.size() << std::endl;
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(1200, 720), "Izumi Game", sf::Style::Default);
    sf::View view(sf::FloatRect(0, 0, 1600, 920));
    window.setView(view);
    window.setFramerateLimit(60);

    GameState state = GameState::MENU;
    Menu menu(1600, 920);

    if (!Enemy::loadTexture("enem2.png")) std::cout << "Enemy texture failed to load!\n";
    if (!Enemy::loadFont("C:/Windows/Fonts/arial.ttf")) std::cout << "Enemy font failed to load!\n";
    if (!Player::loadTexture("enem3.png")) std::cout << "Player texture failed to load!\n";
    if (!Player::loadFont("C:/Windows/Fonts/arial.ttf")) std::cout << "Player font failed to load!\n";

    Player player;
    int currentLevel = 1;
    const int maxLevel = 3;
    std::vector<Enemy> enemies;
    Enemy* selectedEnemy = nullptr;

    sf::Font font;
    if (!font.loadFromFile("C:/Windows/Fonts/hatten.ttf")) std::cout << "Failed to load font for UI text!\n";

    sf::Text endText("WOW :3 LEVEL FINISHED!!!", font, 60);
    endText.setFillColor(sf::Color::White);
    endText.setPosition(200.f, 200.f);

    sf::Text btnNextLevel("Next level!", font, 50);
    btnNextLevel.setFillColor(sf::Color::Magenta);
    btnNextLevel.setPosition(700.f, 420.f);

    sf::Text btnToMenu("Go back to the menu", font, 50);
    btnToMenu.setFillColor(sf::Color::White);
    btnToMenu.setPosition(250.f, 420.f);

    // GAME OVER UI
    sf::Text gameOverText("YOU LOST :(", font, 60);
    gameOverText.setFillColor(sf::Color::White);
    gameOverText.setPosition(1000.f, 300.f);

    sf::Text btnToMenuFromGameOver("Back to Menu", font, 50);
    btnToMenuFromGameOver.setFillColor(sf::Color(255, 105, 180));
    btnToMenuFromGameOver.setPosition(1000.f, 450.f);

    sf::Texture backgroundTextures[3];
    sf::Sprite backgroundSprite;
    if (!backgroundTextures[0].loadFromFile("back1.png")) std::cout << "Failed to load background 1\n";
    if (!backgroundTextures[1].loadFromFile("back1.png")) std::cout << "Failed to load background 2\n";
    if (!backgroundTextures[2].loadFromFile("back1.png")) std::cout << "Failed to load background 3\n";
    backgroundSprite.setTexture(backgroundTextures[0]);

    sf::Texture levelCompleteTexture;
    sf::Sprite levelCompleteSprite;
    if (!levelCompleteTexture.loadFromFile("win1 копия.png")) std::cout << "Failed to load level complete background!\n";
    levelCompleteSprite.setTexture(levelCompleteTexture);

    // Новый фон для экрана проигрыша
    sf::Texture gameOverTexture;
    sf::Sprite gameOverSprite;
    if (!gameOverTexture.loadFromFile("over.png"))  // Замените на путь к своему файлу
        std::cout << "Failed to load game over background!\n";
    gameOverSprite.setTexture(gameOverTexture);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (state == GameState::MENU)
            {
                if (event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::W) menu.moveUp();
                    else if (event.key.code == sf::Keyboard::S) menu.moveDown();
                    else if (event.key.code == sf::Keyboard::Enter) {
                        int selected = menu.getSelectedIndex();
                        if (selected == 0) {
                            state = GameState::GAME;
                            currentLevel = 1;
                            selectedEnemy = nullptr;
                            enemies.clear();
                            player = Player();
                            setupLevel(currentLevel, enemies, backgroundSprite, backgroundTextures);
                        }
                        else if (selected == 1)
                            window.close();
                    }
                }
                else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2f mousePos = window.mapPixelToCoords({ event.mouseButton.x, event.mouseButton.y });
                    int clickedIndex = menu.getItemIndexAtPosition(mousePos);

                    if (clickedIndex == 0) {
                        state = GameState::GAME;
                        currentLevel = 1;
                        selectedEnemy = nullptr;
                        enemies.clear();
                        player = Player();
                        setupLevel(currentLevel, enemies, backgroundSprite, backgroundTextures);
                    }
                    else if (clickedIndex == 1) {
                        window.close();
                    }
                }
            }
            else if (state == GameState::GAME)
            {
                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2f mousePos = window.mapPixelToCoords({ event.mouseButton.x, event.mouseButton.y });
                    selectedEnemy = nullptr;
                    for (auto& enemy : enemies) {
                        sf::FloatRect bounds(enemy.getPosition(), sf::Vector2f(200.f, 200.f));
                        if (bounds.contains(mousePos)) {
                            selectedEnemy = &enemy;
                            break;
                        }
                    }
                }

                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) {
                    if (selectedEnemy && selectedEnemy->isAlive()) {
                        selectedEnemy->takeDamage(10);
                    }
                }
            }
            else if (state == GameState::LEVEL_FINISHED)
            {
                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2f mousePos = window.mapPixelToCoords({ event.mouseButton.x, event.mouseButton.y });

                    if (btnNextLevel.getGlobalBounds().contains(mousePos)) {
                        if (currentLevel < maxLevel) {
                            currentLevel++;
                            selectedEnemy = nullptr;
                            enemies.clear();
                            setupLevel(currentLevel, enemies, backgroundSprite, backgroundTextures);
                            state = GameState::GAME;
                        }
                    }

                    if (btnToMenu.getGlobalBounds().contains(mousePos)) {
                        state = GameState::MENU;
                        currentLevel = 1;
                        enemies.clear();
                        selectedEnemy = nullptr;
                        player = Player();
                    }
                }
            }
            else if (state == GameState::GAME_OVER)
            {
                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2f mousePos = window.mapPixelToCoords({ event.mouseButton.x, event.mouseButton.y });

                    if (btnToMenuFromGameOver.getGlobalBounds().contains(mousePos)) {
                        state = GameState::MENU;
                        currentLevel = 1;
                        enemies.clear();
                        selectedEnemy = nullptr;
                        player = Player();
                    }
                }
            }
        }

        window.clear(sf::Color::White);

        if (state == GameState::MENU)
        {
            menu.draw(window);
        }
        else if (state == GameState::GAME)
        {
            window.draw(backgroundSprite);

            player.handleInput();
            player.update();

            if (player.getHP() <= 0) {
                state = GameState::GAME_OVER;
            }

            for (auto& enemy : enemies) {
                enemy.update(player.getPosition());
                if (enemy.canDamagePlayer()) {
                    player.takeDamage(1);
                }
            }

            if (selectedEnemy && !selectedEnemy->isAlive()) {
                selectedEnemy = nullptr;
            }

            enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
                [](const Enemy& e) { return !e.isAlive(); }),
                enemies.end());

            if (selectedEnemy) {
                bool found = false;
                for (auto& enemy : enemies) {
                    if (&enemy == selectedEnemy) {
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    selectedEnemy = nullptr;
                }
            }

            if (enemies.empty()) {
                state = GameState::LEVEL_FINISHED;
            }

            player.draw(window);
            for (auto& enemy : enemies)
                enemy.draw(window);
        }
        else if (state == GameState::LEVEL_FINISHED)
        {
            window.draw(levelCompleteSprite);
            endText.setString(currentLevel >= maxLevel ? "YOU WIN! :D" : "WOW :3 LEVEL FINISHED!!!");
            drawTextWithOutline(window, endText, sf::Color::Magenta, 2.f);
            if (currentLevel < maxLevel) window.draw(btnNextLevel);
            window.draw(btnToMenu);
        }
        else if (state == GameState::GAME_OVER)
        {
            window.draw(gameOverSprite);  // фон экрана проигрыша
            drawTextWithOutline(window, gameOverText, sf::Color::Magenta, 2.f);
            window.draw(btnToMenuFromGameOver);
        }

        window.display();
    }

    return 0;
}
