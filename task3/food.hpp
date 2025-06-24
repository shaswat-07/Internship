#pragma once
#include <SFML/Graphics.hpp>
#include "Snake.hpp"

class Food {
public:
    Food();
    
    void spawn(int gridWidth, int gridHeight, const Snake& snake);
    void render(sf::RenderWindow& window, int gridSize);
    Position getPosition() const { return m_position; }
    
private:
    void loadTexture();
    bool isPositionValid(const Position& pos, const Snake& snake) const;
    
    Position m_position;
    sf::Texture m_texture;
    sf::Sprite m_sprite;
    sf::Clock m_animationClock;
};