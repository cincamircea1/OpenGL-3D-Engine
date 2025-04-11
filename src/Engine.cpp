#include "../include/Engine.hpp"
#include <iostream>

Engine::Engine(const std::string& title, int width, int height)
    : m_title(title)
    , m_width(width)
    , m_height(height)
    , m_isRunning(false)
{
}

Engine::~Engine()
{
    shutdown();
}

bool Engine::initialize()
{
    std::cout << "Initializing Engine..." << std::endl;
    std::cout << "Window: " << m_title << " (" << m_width << "x" << m_height << ")" << std::endl;
    
    // TODO: Initialize OpenGL context and window
    
    m_isRunning = true;
    return true;
}

void Engine::run()
{
    std::cout << "Engine running..." << std::endl;
    
    // TODO: Implement main game loop
    
    std::cout << "Engine stopped." << std::endl;
}

void Engine::shutdown()
{
    if (m_isRunning) {
        std::cout << "Shutting down Engine..." << std::endl;
        
        // TODO: Clean up OpenGL resources
        
        m_isRunning = false;
    }
} 