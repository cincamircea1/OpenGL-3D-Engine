#include "../include/Engine.hpp"
#include "../include/Logger.hpp"
#include <iostream>

// Static callback function needs to access the Engine instance
void Engine::errorCallback(int error, const char* description) {
    Logger::getInstance().error("GLFW Error " + std::to_string(error) + ": " + description);
}

void Engine::framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

Engine::Engine(const std::string& title, int width, int height)
    : m_title(title)
    , m_width(width)
    , m_height(height)
    , m_isRunning(false)
    , m_window(nullptr)
{
}

Engine::~Engine()
{
    shutdown();
}

bool Engine::initialize()
{
    Logger::getInstance().info("Initializing Engine...");
    
    // Set error callback
    glfwSetErrorCallback(errorCallback);
    
    // Initialize GLFW
    if (!glfwInit()) {
        Logger::getInstance().error("Failed to initialize GLFW");
        return false;
    }
    
    // Configure GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    // Create window
    m_window = glfwCreateWindow(m_width, m_height, m_title.c_str(), nullptr, nullptr);
    if (!m_window) {
        Logger::getInstance().error("Failed to create GLFW window");
        glfwTerminate();
        return false;
    }
    
    // Make OpenGL context current
    glfwMakeContextCurrent(m_window);
    
    // Set callbacks
    glfwSetFramebufferSizeCallback(m_window, framebufferSizeCallback);
    
    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        Logger::getInstance().error("Failed to initialize GLAD");
        return false;
    }
    
    // Configure OpenGL
    glViewport(0, 0, m_width, m_height);
    
    Logger::getInstance().info("OpenGL Version: " + std::string((const char*)glGetString(GL_VERSION)));
    Logger::getInstance().info("GLSL Version: " + std::string((const char*)glGetString(GL_SHADING_LANGUAGE_VERSION)));
    Logger::getInstance().info("Vendor: " + std::string((const char*)glGetString(GL_VENDOR)));
    Logger::getInstance().info("Renderer: " + std::string((const char*)glGetString(GL_RENDERER)));
    
    m_isRunning = true;
    return true;
}

void Engine::processInput() {
    // Exit on escape key
    if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(m_window, true);
    }
}

void Engine::update() {
    // Update game state here
}

void Engine::render() {
    // Clear the screen
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    // Render objects here
    
    // Swap buffers
    glfwSwapBuffers(m_window);
}

void Engine::run()
{
    Logger::getInstance().info("Engine running...");
    
    // Main loop
    while (!glfwWindowShouldClose(m_window)) {
        // Process input
        processInput();
        
        // Update game state
        update();
        
        // Render frame
        render();
        
        // Poll for events
        glfwPollEvents();
    }
    
    Logger::getInstance().info("Engine stopped.");
}

void Engine::shutdown()
{
    if (m_isRunning) {
        Logger::getInstance().info("Shutting down Engine...");
        
        if (m_window) {
            glfwDestroyWindow(m_window);
            m_window = nullptr;
        }
        
        glfwTerminate();
        m_isRunning = false;
    }
} 