#include "scene/Chunk.hpp"
#include "utils/Logger.hpp"
#include <vector>
#include <cmath>
#include <random>

Chunk::Chunk(const glm::vec3& position)
    : m_position(position)
{
    // Create the shader
    m_shader = std::make_unique<Shader>("assets/shaders/basic.vert", "assets/shaders/basic.frag");
    
    // Initialize block data to zeros (empty)
    m_blocks.fill(0);
    
    // Generate the chunk
    generate();
    
    Logger::getInstance().info("Chunk created at position (" + 
        std::to_string(position.x) + ", " + 
        std::to_string(position.y) + ", " + 
        std::to_string(position.z) + ")");
}

Chunk::~Chunk() {
    // Smart pointers handle cleanup
}

void Chunk::generate() {
    // Fill the block array with some pattern
    std::mt19937 rng(static_cast<unsigned int>(
        static_cast<int>(m_position.x) * 73856093 ^ 
        static_cast<int>(m_position.y) * 19349663 ^ 
        static_cast<int>(m_position.z) * 83492791));
    
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);
    
    // Simple terrain generation (just for demonstration)
    // In Minecraft-like approach, this would be more complex terrain generation
    for (int x = 0; x < CHUNK_SIZE; x++) {
        for (int z = 0; z < CHUNK_SIZE; z++) {
            // Generate a simple heightmap
            // Use chunk coordinates to get consistent terrain
            float nx = (m_position.x + x) / 16.0f;
            float nz = (m_position.z + z) / 16.0f;
            
            // Simple heightmap (more Minecraft-like)
            int height = static_cast<int>((sin(nx) + cos(nz)) * 2.0f + 10.0f);
            
            // Keep height within chunk bounds
            height = std::min(height, CHUNK_SIZE - 1);
            
            // Fill blocks below the height with solid blocks
            for (int y = 0; y < CHUNK_SIZE; y++) {
                // Convert to world coordinates
                int worldY = m_position.y + y;
                
                // Below height = solid block
                if (worldY < height + m_position.y) {
                    // Top layer (grass)
                    if (worldY == height + m_position.y - 1) {
                        m_blocks[getIndex(x, y, z)] = 1; // "grass"
                    }
                    // Next 3 layers (dirt)
                    else if (worldY >= height + m_position.y - 4) {
                        m_blocks[getIndex(x, y, z)] = 2; // "dirt"
                    }
                    // Everything else (stone)
                    else {
                        m_blocks[getIndex(x, y, z)] = 3; // "stone"
                    }
                }
            }
        }
    }
    
    // Generate mesh from blocks
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    
    // Add faces for each solid block
    for (int x = 0; x < CHUNK_SIZE; x++) {
        for (int y = 0; y < CHUNK_SIZE; y++) {
            for (int z = 0; z < CHUNK_SIZE; z++) {
                if (blockExists(x, y, z)) {
                    glm::vec3 blockPos(
                        static_cast<float>(x), 
                        static_cast<float>(y), 
                        static_cast<float>(z)
                    );
                    
                    // Get block type for coloring
                    unsigned char blockType = m_blocks[getIndex(x, y, z)];
                    
                    // Only add faces that are adjacent to air or chunk boundary
                    
                    // Top face (Y+)
                    if (!blockExists(x, y + 1, z)) {
                        addFace(vertices, indices, blockPos, 0, blockType);
                    }
                    
                    // Bottom face (Y-)
                    if (!blockExists(x, y - 1, z)) {
                        addFace(vertices, indices, blockPos, 1, blockType);
                    }
                    
                    // Front face (Z+)
                    if (!blockExists(x, y, z + 1)) {
                        addFace(vertices, indices, blockPos, 2, blockType);
                    }
                    
                    // Back face (Z-)
                    if (!blockExists(x, y, z - 1)) {
                        addFace(vertices, indices, blockPos, 3, blockType);
                    }
                    
                    // Right face (X+)
                    if (!blockExists(x + 1, y, z)) {
                        addFace(vertices, indices, blockPos, 4, blockType);
                    }
                    
                    // Left face (X-)
                    if (!blockExists(x - 1, y, z)) {
                        addFace(vertices, indices, blockPos, 5, blockType);
                    }
                }
            }
        }
    }
    
    // Create the mesh if we have vertices
    if (!vertices.empty()) {
        m_mesh = std::make_unique<Mesh>(vertices, indices);
        Logger::getInstance().debug("Chunk mesh created with " + 
            std::to_string(vertices.size()) + " vertices and " + 
            std::to_string(indices.size()) + " indices");
    } else {
        Logger::getInstance().warning("Generated empty chunk at position (" + 
            std::to_string(m_position.x) + ", " + 
            std::to_string(m_position.y) + ", " + 
            std::to_string(m_position.z) + ")");
    }
}

void Chunk::update(float deltaTime) {
    // Currently no update logic needed for static blocks
}

void Chunk::render(const glm::mat4& projection, const glm::mat4& view) {
    if (!m_mesh) return;
    
    // Use shader
    m_shader->use();
    
    // Set up light properties
    m_shader->setVec3("lightPos", glm::vec3(1.2f, 20.0f, 2.0f));
    m_shader->setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
    // objectColor is not needed since we're using per-vertex colors
    
    // Set transformation matrices
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, m_position);
    
    m_shader->setMat4("model", model);
    m_shader->setMat4("view", view);
    m_shader->setMat4("projection", projection);
    
    // Draw chunk
    m_mesh->Draw();
}

void Chunk::addFace(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, 
                   const glm::vec3& position, int faceType, unsigned char blockType) 
{
    // Store current number of vertices
    unsigned int indexOffset = static_cast<unsigned int>(vertices.size());
    
    // Define colors based on block type
    glm::vec3 blockColor;
    
    // Color based on block type
    switch (blockType) {
        case 1: // Grass
            blockColor = (faceType == 0) ? glm::vec3(0.3f, 0.8f, 0.2f) : // Top: green
                         (faceType == 1) ? glm::vec3(0.5f, 0.3f, 0.1f) : // Bottom: brown
                         glm::vec3(0.4f, 0.6f, 0.2f);                     // Sides: grass-dirt mix
            break;
        case 2: // Dirt
            blockColor = glm::vec3(0.5f, 0.3f, 0.1f); // Brown
            break;
        case 3: // Stone
            blockColor = glm::vec3(0.6f, 0.6f, 0.6f); // Gray
            // Add some variation to stone
            blockColor += glm::vec3(0.05f * (position.x / CHUNK_SIZE),
                                   0.05f * (position.y / CHUNK_SIZE),
                                   0.05f * (position.z / CHUNK_SIZE));
            break;
        case 4: // Special floating blocks
            blockColor = glm::vec3(0.9f, 0.6f, 0.2f); // Gold/yellow
            break;
        default: // Unknown
            blockColor = glm::vec3(1.0f, 0.0f, 1.0f); // Magenta for errors
    }
    
    // Add slight variation based on face direction for better visuals
    if (faceType == 2 || faceType == 3) { // Front/back faces
        blockColor *= 0.9f;
    } else if (faceType == 4 || faceType == 5) { // Left/right faces
        blockColor *= 0.8f;
    }
    
    // Define vertices for the face - exact positions with no offsets
    switch (faceType) {
        case 0: // Top face (Y+)
        {
            vertices.push_back({ position + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f), blockColor });
            vertices.push_back({ position + glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 0.0f), blockColor });
            vertices.push_back({ position + glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 1.0f), blockColor });
            vertices.push_back({ position + glm::vec3(0.0f, 1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 1.0f), blockColor });
            
            // Counter-clockwise winding order (when viewed from outside/above)
            indices.push_back(indexOffset + 0);
            indices.push_back(indexOffset + 1);
            indices.push_back(indexOffset + 2);
            indices.push_back(indexOffset + 2);
            indices.push_back(indexOffset + 3);
            indices.push_back(indexOffset + 0);
            break;
        }
        case 1: // Bottom face (Y-)
        {
            vertices.push_back({ position + glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.0f, 0.0f), blockColor });
            vertices.push_back({ position + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.0f, 1.0f), blockColor });
            vertices.push_back({ position + glm::vec3(1.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(1.0f, 1.0f), blockColor });
            vertices.push_back({ position + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(1.0f, 0.0f), blockColor });
            
            // Counter-clockwise winding order (when viewed from outside/below)
            indices.push_back(indexOffset + 0);
            indices.push_back(indexOffset + 3);
            indices.push_back(indexOffset + 2);
            indices.push_back(indexOffset + 2);
            indices.push_back(indexOffset + 1);
            indices.push_back(indexOffset + 0);
            break;
        }
        case 2: // Front face (Z+)
        {
            vertices.push_back({ position + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f), blockColor });
            vertices.push_back({ position + glm::vec3(0.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 1.0f), blockColor });
            vertices.push_back({ position + glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 1.0f), blockColor });
            vertices.push_back({ position + glm::vec3(1.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f), blockColor });
            
            // Counter-clockwise winding order (when viewed from outside/front)
            indices.push_back(indexOffset + 0);
            indices.push_back(indexOffset + 1);
            indices.push_back(indexOffset + 2);
            indices.push_back(indexOffset + 2);
            indices.push_back(indexOffset + 3);
            indices.push_back(indexOffset + 0);
            break;
        }
        case 3: // Back face (Z-)
        {
            vertices.push_back({ position + glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 0.0f), blockColor });
            vertices.push_back({ position + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 0.0f), blockColor });
            vertices.push_back({ position + glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 1.0f), blockColor });
            vertices.push_back({ position + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 1.0f), blockColor });
            
            // Counter-clockwise winding order (when viewed from outside/back)
            indices.push_back(indexOffset + 0);
            indices.push_back(indexOffset + 3);
            indices.push_back(indexOffset + 2);
            indices.push_back(indexOffset + 2);
            indices.push_back(indexOffset + 1);
            indices.push_back(indexOffset + 0);
            break;
        }
        case 4: // Right face (X+)
        {
            vertices.push_back({ position + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f), blockColor });
            vertices.push_back({ position + glm::vec3(1.0f, 0.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f), blockColor });
            vertices.push_back({ position + glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f), blockColor });
            vertices.push_back({ position + glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f), blockColor });
            
            // Counter-clockwise winding order (when viewed from outside/right)
            indices.push_back(indexOffset + 0);
            indices.push_back(indexOffset + 3);
            indices.push_back(indexOffset + 2);
            indices.push_back(indexOffset + 2);
            indices.push_back(indexOffset + 1);
            indices.push_back(indexOffset + 0);
            break;
        }
        case 5: // Left face (X-)
        {
            vertices.push_back({ position + glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f), blockColor });
            vertices.push_back({ position + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f), blockColor });
            vertices.push_back({ position + glm::vec3(0.0f, 1.0f, 1.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f), blockColor });
            vertices.push_back({ position + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f), blockColor });
            
            // Counter-clockwise winding order (when viewed from outside/left)
            indices.push_back(indexOffset + 0);
            indices.push_back(indexOffset + 1);
            indices.push_back(indexOffset + 2);
            indices.push_back(indexOffset + 2);
            indices.push_back(indexOffset + 3);
            indices.push_back(indexOffset + 0);
            break;
        }
    }
}

bool Chunk::blockExists(int x, int y, int z) const {
    // If coordinates are outside this chunk, we need to return false
    // to ensure edges get rendered properly
    if (x < 0 || x >= CHUNK_SIZE || 
        y < 0 || y >= CHUNK_SIZE || 
        z < 0 || z >= CHUNK_SIZE) {
        return false;
    }
    
    // Check if the block at this position is solid (not 0)
    return m_blocks[getIndex(x, y, z)] != 0;
}

int Chunk::getIndex(int x, int y, int z) const {
    return (z * CHUNK_SIZE * CHUNK_SIZE) + (y * CHUNK_SIZE) + x;
} 