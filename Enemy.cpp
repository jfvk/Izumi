#include "Enemy.hpp"
#include <iostream>
#include <cmath>

// Инициализация статических переменных
sf::Texture Enemy::texture;
sf::Font Enemy::font;
bool Enemy::fontLoaded = false;

bool Enemy::loadTexture(const std::string& path) {
    if (!texture.loadFromFile(path)) {
        std::cout << "Failed to load enemy texture!\n";
        return false;
    }
    return true;
}

bool Enemy::loadFont(const std::string& path) {
    if (!font.loadFromFile(path)) {
        std::cout << "Failed to load enemy font!\n";
        fontLoaded = false;
        return false;
    }
    fontLoaded = true;
    return true;
}

Enemy::Enemy(sf::Vector2f position) {
    sprite.setTexture(texture);
    sprite.setPosition(position);

    hp = 100;

    if (fontLoaded) {
        hpText.setFont(font);
    }
    hpText.setCharacterSize(20);
    hpText.setFillColor(sf::Color::Black);
    hpText.setString("HP: " + std::to_string(hp));
    hpText.setPosition(position.x, position.y - 20);

    state = State::CHASING;
}

void Enemy::update(sf::Vector2f playerPos) {
    float speed = 1.5f;

    switch (state) {
    case State::CHASING: {
        sf::Vector2f dir = playerPos - sprite.getPosition();
        float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);
        if (len != 0) dir /= len;

        sprite.move(dir * speed);

        if (sprite.getGlobalBounds().intersects(sf::FloatRect(playerPos, sf::Vector2f(50, 50)))) {
            state = State::RETREATING;
            retreatDirection = -dir;
            actionClock.restart();
        }
        break;
    }
    case State::RETREATING: {
        sprite.move(retreatDirection * speed * 2.0f);
        if (actionClock.getElapsedTime().asSeconds() > retreatTime) {
            state = State::COOLDOWN;
            actionClock.restart();
        }
        break;
    }
    case State::COOLDOWN: {
        if (actionClock.getElapsedTime().asSeconds() > cooldownTime) {
            state = State::CHASING;
        }
        break;
    }
    case State::ATTACKING:
        // Зарезервировано под будущую логику
        break;
    }

    hpText.setPosition(sprite.getPosition().x, sprite.getPosition().y - 20);
    hpText.setString("HP: " + std::to_string(hp));
}

void Enemy::draw(sf::RenderWindow& window) {
    window.draw(sprite);
    window.draw(hpText);
}

sf::Vector2f Enemy::getPosition() const {
    return sprite.getPosition();
}

void Enemy::takeDamage(int damage) {
    hp -= damage;
    if (hp < 0) hp = 0;
}

bool Enemy::isAlive() const {
    return hp > 0;
}

int Enemy::getHP() const {
    return hp;
}

sf::FloatRect Enemy::getBounds() const {
    return sprite.getGlobalBounds();
}

bool Enemy::canDamagePlayer() const {
    return state == State::RETREATING && actionClock.getElapsedTime().asSeconds() < 0.1f;
}

