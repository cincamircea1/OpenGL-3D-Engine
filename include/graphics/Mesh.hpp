#pragma once

#include "glad/glad.h"
#include "glm/glm.hpp"
#include <vector>
#include <string>

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

class Mesh {
public:
    // Mesh data
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    
    // Constructor
    Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
    
    // Destructor
    ~Mesh();
    
    // Render the mesh
    void Draw();
    
private:
    // Render data
    unsigned int VAO, VBO, EBO;
    
    // Initialize all buffer objects/arrays
    void setupMesh();
}; 