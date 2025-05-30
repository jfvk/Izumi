#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class Player {
private:
    int hp;
    sf::Text hpText;
    sf::Sprite sprite;

    static sf::Texture texture;    // Общая текстура для игрока
    static sf::Font font;          // Общий шрифт для текста
    static bool fontLoaded;

    float speed = 4.0f;

public:
    Player();

    void handleInput();
    void update();
    void draw(sf::RenderWindow& window);
    sf::Vector2f getPosition() const;
    void takeDamage(int damage);
    int getHP() const;

    // Загрузка ресурсов
    static bool loadTexture(const std::string& path);
    static bool loadFont(const std::string& path);
};
