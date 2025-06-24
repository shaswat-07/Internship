#include "Snake.hpp"
#include <iostream>

Snake::Snake(int startX, int startY) 
    : m_direction(Direction::RIGHT)
    , m_nextDirection(Direction::RIGHT)
    , m_shouldGrow(false)
{
    m_body.push_back(Position(startX, startY));
    m_body.push_back(Position(startX - 1, startY));
    m_body.push_back(Position(startX - 2, startY));
    
    loadTextures();
}

void Snake::loadTextures() {
    // Create head texture
    sf::Image headImage;
    headImage.create(20, 20, sf::Color(0, 200, 0));
    
    // Add some detail to the head
    for (int i = 6; i < 10; ++i) {
        for (int j = 6; j < 10; ++j) {
            headImage.setPixel(i, j, sf::Color(0, 255, 0));
        }
    }
    
    // Eyes
    headImage.setPixel(5, 5, sf::Color::Black);
    headImage.setPixel(14, 5, sf::Color::Black);
    headImage.setPixel(5, 6, sf::Color::Black);
    headImage.setPixel(14, 6, sf::Color::Black);
    
    if (!m_headTexture.loadFromFile("assets/textures/snake_head.png")) {
        m_headTexture.loadFromImage(headImage);
    }
    
    // Create body texture
    sf::Image bodyImage;
    bodyImage.create(20, 20, sf::Color(0, 150, 0));
    
    // Add scale pattern
    for (int i = 2; i < 18; ++i) {
        for (int j = 2; j < 18; ++j) {
            if ((i + j) % 4 == 0) {
                bodyImage.setPixel(i, j, sf::Color(0, 180, 0));
            }
        }
    }
    
    if (!m_bodyTexture.loadFromFile("assets/textures/snake_body.png")) {
        m_bodyTexture.loadFromImage(bodyImage);
    }
    
    m_headSprite.setTexture(m_headTexture);
    m_bodySprite.setTexture(m_bodyTexture);
}

void Snake::setDirection(Direction dir) {
    // Prevent snake from moving in opposite direction
    if ((m_direction == Direction::UP && dir == Direction::DOWN) ||
        (m_direction == Direction::DOWN && dir == Direction::UP) ||
        (m_direction == Direction::LEFT && dir == Direction::RIGHT) ||
        (m_direction == Direction::RIGHT && dir == Direction::LEFT)) {
        return;
    }
    
    m_nextDirection = dir;
}

void Snake::move() {
    m_direction = m_nextDirection;
    
    Position newHead = m_body.front();
    
    switch (m_direction) {
        case Direction::UP:
            newHead.y--;
            break;
        case Direction::DOWN:
            newHead.y++;
            break;
        case Direction::LEFT:
            newHead.x--;
            break;
        case Direction::RIGHT:
            newHead.x++;
            break;
    }
    
    m_body.push_front(newHead);
    
    if (!m_shouldGrow) {
        m_body.pop_back();
    } else {
        m_shouldGrow = false;
    }
}

void Snake::grow() {
    m_shouldGrow = true;
}

void Snake::render(sf::RenderWindow& window, int gridSize) {
    // Draw body
    for (size_t i = 1; i < m_body.size(); ++i) {
        m_bodySprite.setPosition(m_body[i].x * gridSize, m_body[i].y * gridSize);
        window.draw(m_bodySprite);
    }
    
    // Draw head
    if (!m_body.empty()) {
        m_headSprite.setPosition(m_body[0].x * gridSize, m_body[0].y * gridSize);
        
        // Rotate head based on direction
        switch (m_direction) {
            case Direction::UP:
                m_headSprite.setRotation(270);
                m_headSprite.setPosition(m_body[0].x * gridSize + gridSize, m_body[0].y * gridSize);
                break;
            case Direction::DOWN:
                m_headSprite.setRotation(90);
                m_headSprite.setPosition(m_body[0].x * gridSize, m_body[0].y * gridSize + gridSize);
                break;
            case Direction::LEFT:
                m_headSprite.setRotation(180);
                m_headSprite.setPosition(m_body[0].x * gridSize + gridSize, m_body[0].y * gridSize + gridSize);
                break;
            case Direction::RIGHT:
                m_headSprite.setRotation(0);
                m_headSprite.setPosition(m_body[0].x * gridSize, m_body[0].y * gridSize);
                break;
        }
        
        window.draw(m_headSprite);
    }
}

Position Snake::getHead() const {
    return m_body.empty() ? Position() : m_body.front();
}

bool Snake::checkSelfCollision() const {
    if (m_body.size() < 2) return false;
    
    const Position& head = m_body.front();
    for (size_t i = 1; i < m_body.size(); ++i) {
        if (head == m_body[i]) {
            return true;
        }
    }
    return false;
}