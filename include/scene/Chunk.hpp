#pragma once

#include "graphics/Mesh.hpp"
#include "graphics/Shader.hpp"
#include <memory>
#include <vector>
#include <array>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

// Size of the chunk
constexpr int CHUNK_SIZE = 16; // 16x16x16 blocks per chunk
constexpr int CHUNK_VOLUME = CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE;

class Chunk {
public:
    // Constructor
    Chunk(const glm::vec3& position);
    
    // Destructor
    ~Chunk();
    
    // Generate the chunk with cubes
    void generate();
    
    // Update chunk (if needed)
    void update(float deltaTime);
    
    // Render the chunk
    void render(const glm::mat4& projection, const glm::mat4& view);
    
    // Get chunk position
    glm::vec3 getPosition() const { return m_position; }
    
private:
    // Chunk position (chunk origin in world space)
    glm::vec3 m_position;
    
    // 3D array to store block data (0 = empty, 1 = solid)
    std::array<unsigned char, CHUNK_VOLUME> m_blocks;
    
    // Mesh for the chunk
    std::unique_ptr<Mesh> m_mesh;
    
    // Shader for rendering
    std::unique_ptr<Shader> m_shader;
    
    // Creates a cube face at the specified position
    void addFace(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, 
                 const glm::vec3& position, int faceType, unsigned char blockType);
                 
    // Check if a block exists at the given position
    bool blockExists(int x, int y, int z) const;
    
    // Helper to convert 3D coordinates to array index
    int getIndex(int x, int y, int z) const;
}; 