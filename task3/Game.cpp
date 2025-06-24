#include "Game.hpp"
#include <iostream>
#include <random>

Game::Game() 
    : m_window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Snake Game", sf::Style::Titlebar | sf::Style::Close)
    , m_score(0)
    , m_gameSpeed(0.2f)
    , m_moveTimer(0.0f)
    , m_gameStarted(false)
{
    m_window.setFramerateLimit(60);
    
    loadAssets();
    
    m_snake = std::make_unique<Snake>(GRID_WIDTH / 2, GRID_HEIGHT / 2);
    m_food = std::make_unique<Food>();
    m_stateManager = std::make_unique<GameStateManager>();
    
    m_food->spawn(GRID_WIDTH, GRID_HEIGHT, *m_snake);
    
    // Setup UI
    m_scoreText.setFont(m_font);
    m_scoreText.setCharacterSize(24);
    m_scoreText.setFillColor(sf::Color::White);
    m_scoreText.setPosition(10, 10);
    
    m_gameOverText.setFont(m_font);
    m_gameOverText.setCharacterSize(48);
    m_gameOverText.setFillColor(sf::Color::Red);
    m_gameOverText.setString("GAME OVER");
    sf::FloatRect textBounds = m_gameOverText.getLocalBounds();
    m_gameOverText.setPosition((WINDOW_WIDTH - textBounds.width) / 2, (WINDOW_HEIGHT - textBounds.height) / 2 - 50);
    
    m_instructionText.setFont(m_font);
    m_instructionText.setCharacterSize(18);
    m_instructionText.setFillColor(sf::Color::White);
    m_instructionText.setString("Press R to restart");
    sf::FloatRect instrBounds = m_instructionText.getLocalBounds();
    m_instructionText.setPosition((WINDOW_WIDTH - instrBounds.width) / 2, (WINDOW_HEIGHT - instrBounds.height) / 2 + 20);
    
    updateScore();
}

void Game::loadAssets() {
    // Load font
    if (!m_font.loadFromFile("assets/fonts/arial.ttf")) {
        // Try alternative font paths
        if (!m_font.loadFromFile("assets/arial.ttf") && 
            !m_font.loadFromFile("arial.ttf")) {
            std::cerr << "Warning: Could not load font. Using default font." << std::endl;
        }
    }
    
    // Load background texture
    if (!m_backgroundTexture.loadFromFile("assets/textures/background.png")) {
        // Create a simple pattern background if texture fails
        sf::Image bgImage;
        bgImage.create(WINDOW_WIDTH, WINDOW_HEIGHT, sf::Color(20, 20, 20));
        
        // Create grid pattern
        for (int x = 0; x < WINDOW_WIDTH; x += GRID_SIZE) {
            for (int y = 0; y < WINDOW_HEIGHT; y += GRID_SIZE) {
                if ((x / GRID_SIZE + y / GRID_SIZE) % 2 == 0) {
                    for (int i = 0; i < GRID_SIZE && x + i < WINDOW_WIDTH; ++i) {
                        for (int j = 0; j < GRID_SIZE && y + j < WINDOW_HEIGHT; ++j) {
                            bgImage.setPixel(x + i, y + j, sf::Color(25, 25, 25));
                        }
                    }
                }
            }
        }
        m_backgroundTexture.loadFromImage(bgImage);
    }
    m_backgroundSprite.setTexture(m_backgroundTexture);
    
    // Load sounds
    if (!m_eatSoundBuffer.loadFromFile("assets/sounds/eat.wav")) {
        std::cerr << "Warning: Could not load eat sound" << std::endl;
    } else {
        m_eatSound.setBuffer(m_eatSoundBuffer);
        m_eatSound.setVolume(50);
    }
    
    if (!m_gameOverSoundBuffer.loadFromFile("assets/sounds/gameover.wav")) {
        std::cerr << "Warning: Could not load game over sound" << std::endl;
    } else {
        m_gameOverSound.setBuffer(m_gameOverSoundBuffer);
        m_gameOverSound.setVolume(70);
    }
    
    if (!m_backgroundMusic.openFromFile("assets/sounds/background.ogg")) {
        std::cerr << "Warning: Could not load background music" << std::endl;
    } else {
        m_backgroundMusic.setLoop(true);
        m_backgroundMusic.setVolume(30);
        m_backgroundMusic.play();
    }
}

void Game::run() {
    while (m_window.isOpen()) {
        float deltaTime = m_clock.restart().asSeconds();
        
        handleEvents();
        
        if (m_stateManager->getCurrentState() == GameState::PLAYING) {
            update(deltaTime);
        }
        
        render();
    }
}

void Game::handleEvents() {
    sf::Event event;
    while (m_window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            m_window.close();
        }
        
        if (event.type == sf::Event::KeyPressed) {
            switch (m_stateManager->getCurrentState()) {
                case GameState::MENU:
                    if (event.key.code == sf::Key::Space) {
                        m_stateManager->setState(GameState::PLAYING);
                        m_gameStarted = true;
                    }
                    break;
                    
                case GameState::PLAYING:
                    switch (event.key.code) {
                        case sf::Key::Up:
                        case sf::Key::W:
                            m_snake->setDirection(Direction::UP);
                            break;
                        case sf::Key::Down:
                        case sf::Key::S:
                            m_snake->setDirection(Direction::DOWN);
                            break;
                        case sf::Key::Left:
                        case sf::Key::A:
                            m_snake->setDirection(Direction::LEFT);
                            break;
                        case sf::Key::Right:
                        case sf::Key::D:
                            m_snake->setDirection(Direction::RIGHT);
                            break;
                    }
                    break;
                    
                case GameState::GAME_OVER:
                    if (event.key.code == sf::Key::R) {
                        resetGame();
                    }
                    break;
            }
        }
    }
}

void Game::update(float deltaTime) {
    m_moveTimer += deltaTime;
    
    if (m_moveTimer >= m_gameSpeed) {
        m_moveTimer = 0.0f;
        
        // Move snake
        m_snake->move();
        
        // Check collision with walls
        auto head = m_snake->getHead();
        if (head.x < 0 || head.x >= GRID_WIDTH || head.y < 0 || head.y >= GRID_HEIGHT) {
            m_stateManager->setState(GameState::GAME_OVER);
            m_gameOverSound.play();
            return;
        }
        
        // Check collision with self
        if (m_snake->checkSelfCollision()) {
            m_stateManager->setState(GameState::GAME_OVER);
            m_gameOverSound.play();
            return;
        }
        
        // Check collision with food
        if (head.x == m_food->getPosition().x && head.y == m_food->getPosition().y) {
            m_snake->grow();
            m_food->spawn(GRID_WIDTH, GRID_HEIGHT, *m_snake);
            m_score += 10;
            updateScore();
            increaseSpeed();
            m_eatSound.play();
        }
    }
}

void Game::render() {
    m_window.clear();
    
    // Draw background
    m_window.draw(m_backgroundSprite);
    
    switch (m_stateManager->getCurrentState()) {
        case GameState::MENU: {
            sf::Text menuText;
            menuText.setFont(m_font);
            menuText.setCharacterSize(36);
            menuText.setFillColor(sf::Color::White);
            menuText.setString("SNAKE GAME");
            sf::FloatRect bounds = menuText.getLocalBounds();
            menuText.setPosition((WINDOW_WIDTH - bounds.width) / 2, (WINDOW_HEIGHT - bounds.height) / 2 - 50);
            m_window.draw(menuText);
            
            sf::Text startText;
            startText.setFont(m_font);
            startText.setCharacterSize(18);
            startText.setFillColor(sf::Color::Yellow);
            startText.setString("Press SPACE to start");
            sf::FloatRect startBounds = startText.getLocalBounds();
            startText.setPosition((WINDOW_WIDTH - startBounds.width) / 2, (WINDOW_HEIGHT - startBounds.height) / 2 + 20);
            m_window.draw(startText);
            break;
        }
        
        case GameState::PLAYING:
            m_snake->render(m_window, GRID_SIZE);
            m_food->render(m_window, GRID_SIZE);
            m_window.draw(m_scoreText);
            break;
            
        case GameState::GAME_OVER:
            m_snake->render(m_window, GRID_SIZE);
            m_food->render(m_window, GRID_SIZE);
            m_window.draw(m_scoreText);
            m_window.draw(m_gameOverText);
            m_window.draw(m_instructionText);
            break;
    }
    
    m_window.display();
}

void Game::resetGame() {
    m_snake = std::make_unique<Snake>(GRID_WIDTH / 2, GRID_HEIGHT / 2);
    m_food->spawn(GRID_WIDTH, GRID_HEIGHT, *m_snake);
    m_score = 0;
    m_gameSpeed = 0.2f;
    m_moveTimer = 0.0f;
    updateScore();
    m_stateManager->setState(GameState::PLAYING);
}

void Game::updateScore() {
    m_scoreText.setString("Score: " + std::to_string(m_score));
}

void Game::increaseSpeed() {
    if (m_gameSpeed > 0.05f) {
        m_gameSpeed -= 0.005f;
    }
}