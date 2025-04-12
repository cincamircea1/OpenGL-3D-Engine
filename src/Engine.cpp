#include "../include/Engine.hpp"
#include <iostream>

// Static callback function needs to access the Engine instance
void Engine::errorCallback(int error, const char* description) {
    std::cerr << "GLFW Error " << error << ": " << description << std::endl;
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
    std::cout << "Initializing Engine..." << std::endl;
    
    // Set error callback
    glfwSetErrorCallback(errorCallback);
    
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return false;
    }
    
    // Configure GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    // Create window
    m_window = glfwCreateWindow(m_width, m_height, m_title.c_str(), nullptr, nullptr);
    if (!m_window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }
    
    // Make OpenGL context current
    glfwMakeContextCurrent(m_window);
    
    // Set callbacks
    glfwSetFramebufferSizeCallback(m_window, framebufferSizeCallback);
    
    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return false;
    }
    
    // Configure OpenGL
    glViewport(0, 0, m_width, m_height);
    
    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    
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
    std::cout << "Engine running..." << std::endl;
    
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
    
    std::cout << "Engine stopped." << std::endl;
}

void Engine::shutdown()
{
    if (m_isRunning) {
        std::cout << "Shutting down Engine..." << std::endl;
        
        if (m_window) {
            glfwDestroyWindow(m_window);
            m_window = nullptr;
        }
        
        glfwTerminate();
        m_isRunning = false;
    }
} 