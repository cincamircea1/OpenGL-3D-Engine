#pragma once

#include "Mesh.hpp"
#include "Shader.hpp"
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Cube {
public:
    // Constructor
    Cube();
    
    // Destructor
    ~Cube();
    
    // Update cube rotation based on elapsed time
    void update(float deltaTime);
    
    // Render the cube
    void render(const glm::mat4& projection, const glm::mat4& view);
    
private:
    // Mesh for the cube
    std::unique_ptr<Mesh> m_mesh;
    
    // Shader for rendering
    std::unique_ptr<Shader> m_shader;
    
    // Cube properties
    glm::vec3 m_position;
    glm::vec3 m_rotation;
    float m_rotationSpeed;
    
    // Creates cube mesh
    void createCubeMesh();
}; 