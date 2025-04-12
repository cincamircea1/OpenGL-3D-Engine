#include "../include/Engine.hpp"
#include "../include/Logger.hpp"
#include <iostream>

int main() {
    try {
        // Set log level (optional)
        Logger::getInstance().setLogLevel(Logger::LogLevel::INFO);
        
        // Create and initialize the engine
        Engine engine("OpenGL 3D Engine", 1280, 720);
        
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
