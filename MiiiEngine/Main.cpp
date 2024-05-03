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
    FragColor = vec4(1.0, 0.0, 0.0, 1.0);  // Red color
}
)glsl";

GLuint CompileShader(const char* source, GLenum type) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);
    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    return shader;
}

GLuint CreateShaderProgram(const char* vertexSource, const char* fragmentSource) {
    GLuint vertexShader = CompileShader(vertexSource, GL_VERTEX_SHADER);
    GLuint fragmentShader = CompileShader(fragmentSource, GL_FRAGMENT_SHADER);
    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    int success;
    char infoLog[512];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDetachShader(program, vertexShader);
    glDetachShader(program, fragmentShader);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return program;
}


Camera camera(glm::vec3(0.0f, 1.0f, 1.0f), glm::vec3(0.0f, -1.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), 12.5f);

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

    GLFWwindow* window = glfwCreateWindow(800, 600, "HELLO FIRST MODEL", NULL, NULL);
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
    GLuint shaderProgram = CreateShaderProgram(vertexShaderSource, fragmentShaderSource);
   
    Model myModel("C:/Users/13667/Downloads/models/house1.obj");
    ShaderProgram shader("vertex_shader.glsl", "fragment_shader.glsl");
    float lastFrame = 0.0f;
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    float fov = glm::radians(102.0f);  // Field of view (45 degrees is commonly used)
    float aspectRatio = 800.0f / 600.0f;  // Depends on your window size
    float nearPlane = 0.1f;  // Closest distance that can be rendered
    float farPlane = 100.0f;  // Farthest distance that can be rendered
    glm::mat4 projectionMatrix = glm::perspective(fov, aspectRatio, nearPlane, farPlane);
    GLuint shaderProgram1 = CreateShaderProgram(vertexShaderSource, fragmentShaderSource);
    GLuint shaderProgram2 = CreateShaderProgram(vertexShaderSource, fragmentShaderSource);
    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        processInput(window, deltaTime);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Use your shader program
        glUseProgram(myModel.shaderProgram);
        GLint modelLoc = glGetUniformLocation(shaderProgram, "model");
        GLint viewLoc = glGetUniformLocation(myModel.shaderProgram, "view");
        GLint projLoc = glGetUniformLocation(myModel.shaderProgram, "projection");
        glm::mat4 modelMatrix = glm::mat4(1.0f); 
        // Set matrices
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(camera.getViewMatrix()));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

        // Pass the view and projection matrices to the shader
		

       
       
        // Draw your model
        myModel.draw();

       


        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    glfwTerminate();
    return 0;
}