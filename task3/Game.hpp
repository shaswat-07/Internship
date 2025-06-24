#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <memory>
#include "Snake.hpp"
#include "Food.hpp"
#include "GameStateManager.hpp"

class Game {
public:
    static const int WINDOW_WIDTH = 800;
    static const int WINDOW_HEIGHT = 600;
    static const int GRID_SIZE = 20;
    static const int GRID_WIDTH = WINDOW_WIDTH / GRID_SIZE;
    static const int GRID_HEIGHT = WINDOW_HEIGHT / GRID_SIZE;

    Game();
    void run();

private:
    void handleEvents();
    void update(float deltaTime);
    void render();
    void loadAssets();
    void resetGame();
    void updateScore();
    void increaseSpeed();

    sf::RenderWindow m_window;
    sf::Clock m_clock;
    sf::Font m_font;
    sf::Text m_scoreText;
    sf::Text m_gameOverText;
    sf::Text m_instructionText;
    sf::Texture m_backgroundTexture;
    sf::Sprite m_backgroundSprite;
    
    // Audio
    sf::SoundBuffer m_eatSoundBuffer;
    sf::SoundBuffer m_gameOverSoundBuffer;
    sf::Sound m_eatSound;
    sf::Sound m_gameOverSound;
    sf::Music m_backgroundMusic;
    
    // Game objects
    std::unique_ptr<Snake> m_snake;
    std::unique_ptr<Food> m_food;
    std::unique_ptr<GameStateManager> m_stateManager;
    
    // Game variables
    int m_score;
    float m_gameSpeed;
    float m_moveTimer;
    bool m_gameStarted;
};