#include "Model.h"
#include <glad/glad.h>
#include <iostream>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp> // Ensure this header is included for transformation functions
#include <glm/gtc/type_ptr.hpp>

Model::Model(const std::string& filename) : VAO(0), VBO(0), shaderProgram(0), modelMatrix(glm::mat4(1.0f)) {
	initShaders();
    loadModel(filename);
    setupMesh();
}

Model::~Model() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
}

void Model::loadModel(const std::string& filename) {
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filename.c_str())) {
        throw std::runtime_error(warn + err);
    }

    // Loop over shapes
    for (const auto& shape : shapes) {
        // Loop over faces(polygon)
        size_t index_offset = 0;
        for (size_t f = 0; f < shape.mesh.num_face_vertices.size(); f++) {
            int fv = shape.mesh.num_face_vertices[f];

            // Loop over vertices in the face
            for (size_t v = 0; v < fv; v++) {
                // Access to vertex
                tinyobj::index_t idx = shape.mesh.indices[index_offset + v];
                tinyobj::real_t vx = attrib.vertices[3 * idx.vertex_index + 0];
                tinyobj::real_t vy = attrib.vertices[3 * idx.vertex_index + 1];
                tinyobj::real_t vz = attrib.vertices[3 * idx.vertex_index + 2];

                vertices.push_back(vx);
                vertices.push_back(vy);
                vertices.push_back(vz);
            }
            index_offset += fv;
        }
    }
}

void Model::initShaders() {
    const char* vertexShaderSource = R"glsl(
       #version 330 core
        layout (location = 0) in vec3 aPos;
        uniform mat4 model;
        uniform mat4 view;
        uniform mat4 projection;

        void main() {
            gl_Position = projection * view * model * vec4(aPos, 1.0);
        }
    )glsl";
    const char* fragmentShaderSource = R"glsl(
        #version 330 core
        out vec4 FragColor;
        void main() {
            FragColor = vec4(1.0, 0.5, 0.2, 1.0);  // Customize the color as needed
        }
    )glsl";

    // Compile vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // Compile fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // Link shaders to a shader program
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    int success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    // Delete shaders as they're linked into our program now and no longer necessery
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}


void Model::setupMesh() {

    /*vertices.clear();

    // Define the vertices for a cube
    float cubeVertices[] = {
        -0.5f, -0.5f, -0.5f,  0.5f, -0.5f, -0.5f,  0.5f,  0.5f, -0.5f,  0.5f,  0.5f, -0.5f, -0.5f,  0.5f, -0.5f, -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,  0.5f, -0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  0.5f, -0.5f,  0.5f,  0.5f, -0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f, -0.5f,  0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f,  0.5f, -0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,  0.5f,  0.5f, -0.5f,  0.5f, -0.5f, -0.5f,  0.5f, -0.5f, -0.5f,  0.5f, -0.5f,  0.5f,  0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,  0.5f, -0.5f, -0.5f,  0.5f, -0.5f,  0.5f,  0.5f, -0.5f,  0.5f, -0.5f, -0.5f,  0.5f, -0.5f, -0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,  0.5f,  0.5f, -0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  0.5f, -0.5f,  0.5f,  0.5f, -0.5f,  0.5f, -0.5f
    };

    for (int i = 0; i < sizeof(cubeVertices) / sizeof(cubeVertices[0]); ++i) {
        vertices.push_back(cubeVertices[i]);
    }*/


    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Assuming 'vertices' is a std::vector<float> populated from the OBJ file
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    // Setup vertex attribute pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Model::draw(bool drawWireframe) const {
    if (drawWireframe) {
        // Set polygon mode to render lines instead of filled polygons
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    // Bind the VAO (Vertex Array Object) for the model
    glBindVertexArray(VAO);

    // Draw the mesh using glDrawArrays or glDrawElements depending on your setup
    glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vertices.size() / 3)); // or use glDrawElements if you have indices

    // Revert to the default polygon mode to render filled polygons
    if (drawWireframe) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    // Unbind the VAO to clean up
    glBindVertexArray(0);
}
void Model::rotate(float angle, const glm::vec3& axis) {
    modelMatrix = glm::rotate(modelMatrix, glm::radians(angle), axis);
}

const glm::mat4& Model::getModelMatrix() const {
    return modelMatrix;
}

void Model::scale(const glm::vec3& scaleFactor) {
    modelMatrix = glm::scale(modelMatrix, scaleFactor);
}

void Model::translate(const glm::vec3& offset) {
    modelMatrix = glm::translate(modelMatrix, offset);
}
