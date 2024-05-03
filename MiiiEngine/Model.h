#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include <string>
#include "tiny_obj_loader.h"  // Ensure tinyobjloader is correctly included
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
class Model {
public:
    Model(const std::string& filename);
    ~Model();
    unsigned int shaderProgram;
    void draw() const;
    void setupMesh();
    const glm::mat4& getModelMatrix() const;
private:
    unsigned int VAO, VBO;
    std::vector<float> vertices;
    glm::mat4 modelMatrix;

    void initShaders();
    void loadModel(const std::string& filename);
};

#endif // MODEL_H
