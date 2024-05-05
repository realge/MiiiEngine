#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <cmath>
#include "Model.h"
#include "Camera.h"
#include "ShaderProgram.h"
#include "ModelLoader.h"
// Vertex Shader
const char* vertexShaderSource = R"glsl(
   #version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 Normal;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;
    
    gl_Position = projection * view * vec4(FragPos, 1.0);
}

)glsl";

// Fragment Shader
const char* fragmentShaderSource = R"glsl(
   #version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 objectColor;

void main()
{
    // Ambient lighting
    float ambientStrength = 0.02;
    vec3 ambient = ambientStrength * lightColor;
    
    // Diffuse lighting
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    
    // Combine the result
    vec3 result = (ambient + diffuse) * objectColor;
    FragColor = vec4(result, 1.0);
}
)glsl";



Camera camera(glm::vec3(0.0f, 1.0f, 1.0f), glm::vec3(0.0f, -1.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), 20.5f);
glm::vec3 lightDirection(0.0f, -1.0f, -1.0f); // Example light direction
glm::vec3 lightColor(1.0f, 1.0f, 1.0f);        // White light


void processInput(GLFWwindow* window, float deltaTime) {
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.moveForward(deltaTime); // Move closer
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.moveBackward(deltaTime); // Move further away

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.strafeLeft(deltaTime); // Rotate left
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.strafeRight(deltaTime); // Move right
}

void updateLightWithCamera(const Camera& camera, ShaderProgram& shader) {
    glm::vec3 lightPosition = camera.position;  // Light follows the camera position
    glm::vec3 lightDirection = camera.front;    // Light direction is the same as camera front

    shader.use();
    shader.setUniform("lightPos", lightPosition);


    // Set other light properties if necessary
    glm::vec3 lightColor(1.0f, 1.0f, 1.0f);  // Example: white light
    shader.setUniform("lightColor", lightColor);
}

// Define some global variables to track mouse movement and button state
float lastX = 400, lastY = 300;
bool firstMouse = true;
bool rightMouseButtonPressed = false;
glm::vec2 initialCursorPosition(0.0f);

// Function to handle mouse button presses
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        if (action == GLFW_PRESS) {
            rightMouseButtonPressed = true;
            double xpos, ypos;
            glfwGetCursorPos(window, &xpos, &ypos);
            initialCursorPosition.x = static_cast<float>(xpos);
            initialCursorPosition.y = static_cast<float>(ypos);
        }
        else if (action == GLFW_RELEASE) {
            rightMouseButtonPressed = false;
        }
    }
}

// Function to handle mouse movement
// Function to handle mouse movement
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse) {
        lastX = static_cast<float>(xpos);
        lastY = static_cast<float>(ypos);
        firstMouse = false;
    }

    float xOffset = static_cast<float>(xpos) - lastX;
    float yOffset = lastY - static_cast<float>(ypos); // Reversed since y-coordinates go from bottom to top

    lastX = static_cast<float>(xpos);
    lastY = static_cast<float>(ypos);

    // Adjust sensitivity if needed
    float sensitivity = 0.3f;
    xOffset *= sensitivity;
    yOffset *= sensitivity;

    // Update camera orientation based on mouse movement
    if (rightMouseButtonPressed) {
        camera.rotate(xOffset, yOffset);
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(1200, 800, "HELLO FIRST MODEL", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    // Set up input callbacks
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    glEnable(GL_DEPTH_TEST);


    Model myModel("C:/Users/13667/Downloads/models/house1.obj");
    Model myModel2("C:/Users/13667/Downloads/models/Armature_001-(Wavefront OBJ).obj");
    ModelLoader model;
    model.loadFBX("C:/Users/13667/Downloads/models/2022_11_11_Robot3.fbx");

    ShaderProgram shader(vertexShaderSource, fragmentShaderSource);
    float lastFrame = 0.0f;

    float fov = glm::radians(102.0f);  // Field of view (45 degrees is commonly used)
    float aspectRatio = 1200.0f / 800.0f;  // Depends on your window size
    float nearPlane = 0.1f;  // Closest distance that can be rendered
    float farPlane = 300.0f;  // Farthest distance that can be rendered
    glm::mat4 modelMatrix = glm::mat4(0.3f);
    glm::mat4 projectionMatrix = glm::perspective(fov, aspectRatio, nearPlane, farPlane);
    float rotationAngle = 0.0f;
    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        processInput(window, deltaTime);
       
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Use your shader program
        shader.use(); // Use shader program
        shader.setUniform("projection", projectionMatrix);
        shader.setUniform("view", camera.getViewMatrix());

        glm::vec3 objectColor(0.5f, 0.5f, 0.5f); // Mid-tone gray
        updateLightWithCamera(camera, shader);
        shader.setUniform("objectColor", objectColor);
        //modelMatrix = glm::rotate(modelMatrix, glm::radians(0.2f), glm::vec3(1.0f, 0.0f, 0.0f));

        // myModel.rotate(0.2f, glm::vec3(0.0f, 1.0f, 0.0f));
         //myModel.translate(glm::vec3(0.0f, 0.01f, -0.0f));



         //myModel.draw(false,shader); // Draw the model
         // Pass the view and projection matrices to the shader

        shader.setUniform("model", modelMatrix);
        //myModel2.draw(true, shader);


        // Draw your model
        model.render();


        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    glfwTerminate();
    return 0;
}