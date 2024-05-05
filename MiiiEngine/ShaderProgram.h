#pragma once
#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <unordered_map> 

class ShaderProgram {
public:
    // Constructor and Destructor
    ShaderProgram(const char* vertexSource, const char* fragmentSource);
    ~ShaderProgram();

    // Methods to use and manage the shader program
    void use() const;
    GLuint getProgram() const;

    // Utility methods for setting uniforms
    void setUniform(const std::string& name, const glm::mat4& mat) const;
    void setUniform(const std::string& name, const glm::vec3& vec) const;
    void setUniform(const std::string& name, float val) const;
    void setUniform(const std::string& name, int val) const;
    void fetchUniformLocations();
private:
    GLuint programID;
    static GLuint compileShader(const char* source, GLenum type);
    std::unordered_map<std::string, GLint> uniformLocations;
};

#endif // SHADER_PROGRAM_H
