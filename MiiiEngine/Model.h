#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include <string>
#include "tiny_obj_loader.h"  // Ensure tinyobjloader is correctly included
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "ShaderProgram.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>



class Model {
public:
    Model(const std::string& filename);
    ~Model();
    
    void draw(bool drawWireframe, ShaderProgram& shader) const;
    void setupMesh();
    void processNode(aiNode* node, const aiScene* scene);
    void processMesh(aiMesh* mesh, const aiScene* scene);
    const glm::mat4& getModelMatrix() const;
    void rotate(float angle, const glm::vec3& axis);
    void translate(const glm::vec3& offset);
    void scale(const glm::vec3& scaleFactor);
   

    glm::mat4 aiMatrix4x4ToGlm(const aiMatrix4x4& from)
{
    glm::mat4 to;

    to[0][0] = from.a1; to[1][0] = from.a2;
    to[2][0] = from.a3; to[3][0] = from.a4;
    to[0][1] = from.b1; to[1][1] = from.b2;
    to[2][1] = from.b3; to[3][1] = from.b4;
    to[0][2] = from.c1; to[1][2] = from.c2;
    to[2][2] = from.c3; to[3][2] = from.c4;
    to[0][3] = from.d1; to[1][3] = from.d2;
    to[2][3] = from.d3; to[3][3] = from.d4;

    return to;
}
private:
    unsigned int VAO, VBO;
    std::vector<float> vertices;
  
    glm::mat4 modelMatrix;
    std::vector<unsigned int> indices;
   

   
    void loadModel(const std::string& filename);
};

#endif // MODEL_H
