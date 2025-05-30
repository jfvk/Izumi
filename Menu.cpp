#include "Menu.hpp"
#include <iostream>

Menu::Menu(float width, float height) {
    if (!backgroundTexture.loadFromFile("Imagelast.jpg")) {
        std::cout << "Не удалось загрузить фон меню!\n";
    }
    else {
        backgroundSprite.setTexture(backgroundTexture);
    

            // Масштабируем фон под размер окна
            float scaleX = width / backgroundTexture.getSize().x;
            float scaleY = height / backgroundTexture.getSize().y;
            backgroundSprite.setScale(scaleX, scaleY);
    }
    
    if (!font.loadFromFile("C:/Windows/Fonts/hatten.ttf")) {
        std::cout << "Не удалось загрузить шрифт!\n";
    }

std::vector<std::string> labels = { "START", "EXIT" };
const float spacing = 100.f;  // Расстояние между пунктами меню
const float totalHeight = spacing * (labels.size() - 1);  // Общая высота меню
const float startY = height / 2.f - totalHeight / 2.f;  // Центрирование по вертикали

for (size_t i = 0; i < labels.size(); ++i) {
    sf::Text text;
    text.setFont(font);
    text.setString(labels[i]);
    text.setCharacterSize(68);
    text.setFillColor(i == 0 ? sf::Color(255, 105, 180)  : sf::Color::White);
    text.setPosition(width / 2.f - 100.f, startY + i * spacing);
    menuItems.push_back(text);
}


    selectedIndex = 0;
}

void Menu::draw(sf::RenderWindow& window) {
    window.draw(backgroundSprite);
    for (const auto& item : menuItems)
        window.draw(item);
}

void Menu::moveUp() {
    if (selectedIndex > 0) {
        menuItems[selectedIndex].setFillColor(sf::Color::White);
        selectedIndex--;
        menuItems[selectedIndex].setFillColor(sf::Color(255, 105, 180));
    }
}

void Menu::moveDown() {
    if (selectedIndex < menuItems.size() - 1) {
        menuItems[selectedIndex].setFillColor(sf::Color::White);
        selectedIndex++;
        menuItems[selectedIndex].setFillColor(sf::Color(255, 105, 180));
    }
}

int Menu::getSelectedIndex() const {
    return selectedIndex;
}

int Menu::getItemIndexAtPosition(sf::Vector2f mousePos) {
    for (size_t i = 0; i < menuItems.size(); ++i) {
        if (menuItems[i].getGlobalBounds().contains(mousePos)) {
            return static_cast<int>(i);
        }
    }
    return -1; // ничего не выбрано
}
