#include "core/Engine.hpp"
#include "utils/Logger.hpp"
#include <iostream>

// Static pointer to the current Engine instance for use in callbacks
static Engine* currentEngineInstance = nullptr;

// Static callback function needs to access the Engine instance
void Engine::errorCallback(int error, const char* description) {
    Logger::getInstance().error("GLFW Error " + std::to_string(error) + ": " + description);
}

void Engine::framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void Engine::mouseCallback(GLFWwindow* window, double xpos, double ypos) {
    if (currentEngineInstance) {
        if (currentEngineInstance->m_firstMouse) {
            currentEngineInstance->m_lastX = static_cast<float>(xpos);
            currentEngineInstance->m_lastY = static_cast<float>(ypos);
            currentEngineInstance->m_firstMouse = false;
        }

        float xoffset = static_cast<float>(xpos) - currentEngineInstance->m_lastX;
        float yoffset = currentEngineInstance->m_lastY - static_cast<float>(ypos); // Reversed: y ranges from bottom to top

        currentEngineInstance->m_lastX = static_cast<float>(xpos);
        currentEngineInstance->m_lastY = static_cast<float>(ypos);

        if (currentEngineInstance->m_camera) {
            currentEngineInstance->m_camera->ProcessMouseMovement(xoffset, yoffset);
        }
    }
}

void Engine::scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    if (currentEngineInstance && currentEngineInstance->m_camera) {
        currentEngineInstance->m_camera->ProcessMouseScroll(static_cast<float>(yoffset));
    }
}

Engine::Engine(const std::string& title, int width, int height)
    : m_title(title)
    , m_width(width)
    , m_height(height)
    , m_isRunning(false)
    , m_window(nullptr)
    , m_deltaTime(0.0f)
    , m_lastFrameTime(0.0f)
    , m_firstMouse(true)
    , m_lastX(width / 2.0f)
    , m_lastY(height / 2.0f)
{
}

Engine::~Engine()
{
    shutdown();
}

void Engine::setupCallbacks() {
    // Set this instance as the current one for callbacks
    currentEngineInstance = this;
    
    // Set GLFW callbacks
    glfwSetFramebufferSizeCallback(m_window, framebufferSizeCallback);
    glfwSetCursorPosCallback(m_window, mouseCallback);
    glfwSetScrollCallback(m_window, scrollCallback);
    
    // Capture cursor for camera control
    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
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
    
    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        Logger::getInstance().error("Failed to initialize GLAD");
        return false;
    }
    
    // Configure OpenGL
    glViewport(0, 0, m_width, m_height);
    glEnable(GL_DEPTH_TEST);
    
    Logger::getInstance().info("OpenGL Version: " + std::string((const char*)glGetString(GL_VERSION)));
    Logger::getInstance().info("GLSL Version: " + std::string((const char*)glGetString(GL_SHADING_LANGUAGE_VERSION)));
    Logger::getInstance().info("Vendor: " + std::string((const char*)glGetString(GL_VENDOR)));
    Logger::getInstance().info("Renderer: " + std::string((const char*)glGetString(GL_RENDERER)));
    
    // Initialize camera
    m_camera = std::make_unique<Camera>(glm::vec3(0.0f, 0.0f, 5.0f));
    
    // Create scene objects
    m_cube = std::make_unique<Cube>();
    
    // Set up callbacks
    setupCallbacks();
    
    m_lastFrameTime = static_cast<float>(glfwGetTime());
    m_isRunning = true;
    return true;
}

void Engine::processInput() {
    // Exit on escape key
    if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(m_window, true);
    }
    
    // Camera movement with WASD keys and space/ctrl
    if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS)
        m_camera->ProcessKeyboard(CameraMovement::FORWARD, m_deltaTime);
    if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS)
        m_camera->ProcessKeyboard(CameraMovement::BACKWARD, m_deltaTime);
    if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS)
        m_camera->ProcessKeyboard(CameraMovement::LEFT, m_deltaTime);
    if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS)
        m_camera->ProcessKeyboard(CameraMovement::RIGHT, m_deltaTime);
    if (glfwGetKey(m_window, GLFW_KEY_SPACE) == GLFW_PRESS)
        m_camera->ProcessKeyboard(CameraMovement::UP, m_deltaTime);
    if (glfwGetKey(m_window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        m_camera->ProcessKeyboard(CameraMovement::DOWN, m_deltaTime);
}

void Engine::update() {
    // Calculate delta time
    float currentFrame = static_cast<float>(glfwGetTime());
    m_deltaTime = currentFrame - m_lastFrameTime;
    m_lastFrameTime = currentFrame;
    
    // Update game objects
    m_cube->update(m_deltaTime);
}

void Engine::render() {
    // Clear the screen
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Calculate projection matrix with camera zoom for FOV
    float aspectRatio = static_cast<float>(m_width) / static_cast<float>(m_height);
    glm::mat4 projection = glm::perspective(glm::radians(m_camera->Zoom), aspectRatio, 0.1f, 100.0f);
    
    // Get view matrix from camera
    glm::mat4 view = m_camera->GetViewMatrix();
    
    // Render objects
    m_cube->render(projection, view);
    
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
        
        // Clean up resources
        m_cube.reset();
        m_camera.reset();
        
        // Reset the current engine instance pointer
        currentEngineInstance = nullptr;
        
        if (m_window) {
            glfwDestroyWindow(m_window);
            m_window = nullptr;
        }
        
        glfwTerminate();
        m_isRunning = false;
    }
} 