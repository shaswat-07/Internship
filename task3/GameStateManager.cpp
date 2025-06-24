#include "GameStateManager.hpp"

GameStateManager::GameStateManager() 
    : m_currentState(GameState::MENU) 
{
}

void GameStateManager::setState(GameState state) {
    m_currentState = state;
}