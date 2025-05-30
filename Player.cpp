#include "Player.hpp"
#include <iostream>

// Статические переменные
sf::Texture Player::texture;
sf::Font Player::font;
bool Player::fontLoaded = false;

bool Player::loadTexture(const std::string& path) {
    if (!texture.loadFromFile(path)) {
        std::cout << "Failed to load player texture!\n";
        return false;
    }
    return true;
}

bool Player::loadFont(const std::string& path) {
    if (!font.loadFromFile(path)) {
        std::cout << "Failed to load player font!\n";
        fontLoaded = false;
        return false;
    }
    fontLoaded = true;
    return true;
}

Player::Player() {
    sprite.setTexture(texture);
    sprite.setPosition(600.f, 500.f); // Начальная позиция

    // Масштабируем под 200x200
    sf::Vector2u texSize = texture.getSize();
    sprite.setScale(
        200.f / texSize.x,
        200.f / texSize.y
    );

    hp = 100;

    if (fontLoaded) {
        hpText.setFont(font);
    }
    hpText.setCharacterSize(20);
    hpText.setFillColor(sf::Color::Black);
    hpText.setString("HP: " + std::to_string(hp));
    hpText.setPosition(sprite.getPosition().x, sprite.getPosition().y - 30);
}

void Player::handleInput() {
    sf::Vector2f movement(0.f, 0.f);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        movement.x -= speed;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        movement.x += speed;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        movement.y -= speed;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        movement.y += speed;

    sprite.move(movement);
}

void Player::update() {
    hpText.setPosition(sprite.getPosition().x, sprite.getPosition().y - 30);
    hpText.setString("HP: " + std::to_string(hp));
}

void Player::draw(sf::RenderWindow& window) {
    window.draw(sprite);
    window.draw(hpText);
}

sf::Vector2f Player::getPosition() const {
    return sprite.getPosition();
}

void Player::takeDamage(int damage) {
    hp -= damage;
    if (hp < 0) hp = 0;
}

int Player::getHP() const {
    return hp;
}
