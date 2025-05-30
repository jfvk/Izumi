#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class Enemy {
private:
    int hp;
    sf::Text hpText;
    sf::Sprite sprite;

    static sf::Texture texture;    // общая текстура для всех врагов
    static sf::Font font;          // общий шрифт для всех врагов
    static bool fontLoaded;

    sf::Clock actionClock;
    sf::Vector2f retreatDirection;
    float retreatTime = 0.5f;
    float cooldownTime = 1.0f;

public:
    enum class State { CHASING, ATTACKING, RETREATING, COOLDOWN };
    State state;

    Enemy(sf::Vector2f position);

    void update(sf::Vector2f playerPos);
    void draw(sf::RenderWindow& window);
    sf::Vector2f getPosition() const;

    void takeDamage(int damage);
    bool isAlive() const;
    int getHP() const;
    sf::FloatRect getBounds() const;
    bool canDamagePlayer() const;

    // Загрузка ресурсов (вызывается один раз перед созданием врагов)
    static bool loadTexture(const std::string& path);
    static bool loadFont(const std::string& path);
};

