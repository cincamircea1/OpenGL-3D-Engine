#pragma once

#include <string>

/**
 * Main engine class for the OpenGL 3D Engine.
 */
class Engine {
public:
    /**
     * Constructor
     * @param title Window title
     * @param width Window width
     * @param height Window height
     */
    Engine(const std::string& title, int width, int height);

    /**
     * Destructor
     */
    ~Engine();

    /**
     * Initialize the engine and OpenGL context
     * @return True if initialization succeeded
     */
    bool initialize();

    /**
     * Main game loop
     */
    void run();

    /**
     * Clean up resources
     */
    void shutdown();

private:
    std::string m_title;
    int m_width;
    int m_height;
    bool m_isRunning;

    // OpenGL context and window will be added here
}; 