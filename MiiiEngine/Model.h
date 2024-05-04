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
    void draw(bool drawWireframe) const;
    void setupMesh();
    const glm::mat4& getModelMatrix() const;
    void rotate(float angle, const glm::vec3& axis);
    void translate(const glm::vec3& offset);
    void scale(const glm::vec3& scaleFactor);
private:
    unsigned int VAO, VBO;
    std::vector<float> vertices;
    glm::mat4 modelMatrix;

    void initShaders();
    void loadModel(const std::string& filename);
};

#endif // MODEL_H
