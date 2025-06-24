#pragma once

enum class GameState {
    MENU,
    PLAYING,
    GAME_OVER
};

class GameStateManager {
public:
    GameStateManager();
    
    void setState(GameState state);
    GameState getCurrentState() const { return m_currentState; }
    
private:
    GameState m_currentState;
};