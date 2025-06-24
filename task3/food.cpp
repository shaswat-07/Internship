#include "Food.hpp"
#include <random>
#include <cmath>

Food::Food() {
    loadTexture();
}

void Food::loadTexture() {
    // Create apple texture
    sf::Image appleImage;
    appleImage.create(20, 20, sf::Color::Transparent);
    
    // Draw apple shape
    for (int x = 0; x < 20; ++x) {
        for (int y = 0; y < 20; ++y) {
            float centerX = 10.0f;
            float centerY = 12.0f;
            float distance = std::sqrt((x - centerX) * (x - centerX) + (y - centerY) * (y - centerY));
            
            if (distance <= 7.0f) {
                appleImage.setPixel(x, y, sf::Color::Red);
            }
        }
    }
    
    // Add stem
    appleImage.setPixel(10, 3, sf::Color(101, 67, 33));
    appleImage.setPixel(10, 4, sf::Color(101, 67, 33));
    appleImage.setPixel(10, 5, sf::Color(101, 67, 33));
    
    // Add leaf
    appleImage.setPixel(9, 4, sf::Color::Green);
    appleImage.setPixel(8, 5, sf::Color::Green);
    
    // Add highlight
    appleImage.setPixel(8, 8, sf::Color(255, 150, 150));
    appleImage.setPixel(7, 9, sf::Color(255, 150, 150));
    appleImage.setPixel(8, 9, sf::Color(255, 150, 150));
    
    if (!m_texture.loadFromFile("assets/textures/apple.png")) {
        m_texture.loadFromImage(appleImage);
    }
    
    m_sprite.setTexture(m_texture);
}

void Food::spawn(int gridWidth, int gridHeight, const Snake& snake) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    
    Position newPos;
    int attempts = 0;
    const int maxAttempts = 100;
    
    do {
        std::uniform_int_distribution<> xDist(0, gridWidth - 1);
        std::uniform_int_distribution<> yDist(0, gridHeight - 1);
        
        newPos.x = xDist(gen);
        newPos.y = yDist(gen);
        attempts++;
    } while (!isPositionValid(newPos, snake) && attempts < maxAttempts);
    
    m_position = newPos;
}

void Food::render(sf::RenderWindow& window, int gridSize) {
    // Add pulsing animation
    float time = m_animationClock.getElapsedTime().asSeconds();
    float scale = 1.0f + 0.1f * std::sin(time * 3.0f);
    
    m_sprite.setScale(scale, scale);
    m_sprite.setPosition(
        m_position.x * gridSize + (gridSize * (1.0f - scale)) / 2.0f,
        m_position.y * gridSize + (gridSize * (1.0f - scale)) / 2.0f
    );
    
    window.draw(m_sprite);
}

bool Food::isPositionValid(const Position& pos, const Snake& snake) const {
    const auto& body = snake.getBody();
    for (const auto& segment : body) {
        if (pos == segment) {
            return false;
        }
    }
    return true;
}