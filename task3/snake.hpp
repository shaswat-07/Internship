#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <deque>

enum class Direction {
    UP, DOWN, LEFT, RIGHT
};

struct Position {
    int x, y;
    Position(int x = 0, int y = 0) : x(x), y(y) {}
    bool operator==(const Position& other) const {
        return x == other.x && y == other.y;
    }
};

class Snake {
public:
    Snake(int startX, int startY);
    
    void setDirection(Direction dir);
    void move();
    void grow();
    void render(sf::RenderWindow& window, int gridSize);
    
    Position getHead() const;
    bool checkSelfCollision() const;
    const std::deque<Position>& getBody() const { return m_body; }
    
private:
    void loadTextures();
    
    std::deque<Position> m_body;
    Direction m_direction;
    Direction m_nextDirection;
    bool m_shouldGrow;
    
    sf::Texture m_headTexture;
    sf::Texture m_bodyTexture;
    sf::Sprite m_headSprite;
    sf::Sprite m_bodySprite;
};