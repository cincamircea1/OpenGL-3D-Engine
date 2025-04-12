#include "core/Engine.hpp"
#include "utils/Logger.hpp"
#include <iostream>

int main() {
    try {
        // Set log level for development (more verbose)
        Logger::getInstance().setLogLevel(Logger::LogLevel::DEBUG);
        Logger::getInstance().info("Starting OpenGL 3D Engine with rotating cube...");
        
        // Create and initialize the engine
        Engine engine("OpenGL 3D Engine - Rotating Cube", 1280, 720);
        
        if (!engine.initialize()) {
            Logger::getInstance().error("Failed to initialize engine!");
            return 1;
        }
        
        // Run the main game loop
        engine.run();
        
    } catch (const std::exception& e) {
        Logger::getInstance().fatal("Exception: " + std::string(e.what()));
        return 1;
    } catch (...) {
        Logger::getInstance().fatal("Unknown exception occurred!");
        return 1;
    }
    
    return 0;
}
