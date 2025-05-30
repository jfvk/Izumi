#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class Menu {
public:
    Menu(float width, float height);
    void draw(sf::RenderWindow& window);
    void moveUp();
    void moveDown();
    int getSelectedIndex() const;
    int getItemIndexAtPosition(sf::Vector2f mousePos);

private:
    int selectedIndex;
    sf::Font font;
    std::vector<sf::Text> menuItems;

    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;

};
