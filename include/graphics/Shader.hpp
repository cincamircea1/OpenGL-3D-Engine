#pragma once

#include <string>
#include <unordered_map>
#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

class Shader {
public:
    // Program ID
    unsigned int ID;

    // Constructor
    Shader(const char* vertexPath, const char* fragmentPath);
    
    // Destructor
    ~Shader();

    // Use/activate the shader
    void use();
    
    // Utility uniform functions
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setVec2(const std::string& name, const glm::vec2& value) const;
    void setVec3(const std::string& name, const glm::vec3& value) const;
    void setVec4(const std::string& name, const glm::vec4& value) const;
    void setMat2(const std::string& name, const glm::mat2& mat) const;
    void setMat3(const std::string& name, const glm::mat3& mat) const;
    void setMat4(const std::string& name, const glm::mat4& mat) const;

private:
    // Utility function for checking shader compilation/linking errors
    void checkCompileErrors(GLuint shader, std::string type);
    
    // Cache for uniform locations
    mutable std::unordered_map<std::string, int> uniformLocationCache;
    
    // Get uniform location with caching
    int getUniformLocation(const std::string& name) const;
}; 