#include "ShaderProgram.h"
#include <iostream>

ShaderProgram::ShaderProgram(const char* vertexSource, const char* fragmentSource) {
    GLuint vertexShader = compileShader(vertexSource, GL_VERTEX_SHADER);
    GLuint fragmentShader = compileShader(fragmentSource, GL_FRAGMENT_SHADER);

    programID = glCreateProgram();
    glAttachShader(programID, vertexShader);
    glAttachShader(programID, fragmentShader);
    glLinkProgram(programID);

    // Check for linking errors
    int success;
    char infoLog[512];
    glGetProgramiv(programID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(programID, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glDetachShader(programID, vertexShader);
    glDetachShader(programID, fragmentShader);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

ShaderProgram::~ShaderProgram() {
    glDeleteProgram(programID);
}

void ShaderProgram::use() const {
    glUseProgram(programID);
}

GLuint ShaderProgram::getProgram() const {
    return programID;
}

GLuint ShaderProgram::compileShader(const char* source, GLenum type) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    // Check for compilation errors
    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
        glDeleteShader(shader);
        shader = 0;
    }

    return shader;
}

void ShaderProgram::setUniform(const std::string& name, const glm::mat4& mat) const {
    glUniformMatrix4fv(glGetUniformLocation(programID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
}

void ShaderProgram::setUniform(const std::string& name, const glm::vec3& vec) const {
    glUniform3fv(glGetUniformLocation(programID, name.c_str()), 1, glm::value_ptr(vec));
}

void ShaderProgram::setUniform(const std::string& name, float val) const {
    glUniform1f(glGetUniformLocation(programID, name.c_str()), val);
}

void ShaderProgram::setUniform(const std::string& name, int val) const {
    glUniform1i(glGetUniformLocation(programID, name.c_str()), val);
}
