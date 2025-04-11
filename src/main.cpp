#include "../include/Engine.hpp"
#include <iostream>

int main() {
    try {
        // Create and initialize the engine
        Engine engine("OpenGL 3D Engine", 1280, 720);
        
        if (!engine.initialize()) {
            std::cerr << "Failed to initialize engine!" << std::endl;
            return 1;
        }
        
        // Run the main game loop
        engine.run();
        
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "Unknown exception occurred!" << std::endl;
        return 1;
    }
    
    return 0;
}
