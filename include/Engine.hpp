#pragma once

#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

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
    // Window properties
    std::string m_title;
    int m_width;
    int m_height;
    bool m_isRunning;
    
    // GLFW window
    GLFWwindow* m_window;
    
    /**
     * Process input
     */
    void processInput();
    
    /**
     * Render frame
     */
    void render();
    
    /**
     * Update frame
     */
    void update();
    
    /**
     * Error callback for GLFW
     */
    static void errorCallback(int error, const char* description);
    
    /**
     * Window resize callback
     */
    static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
}; 