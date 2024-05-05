#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>
class Camera {
public:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    float speed;
    float yaw;
    float pitch;

    Camera(glm::vec3 pos, glm::vec3 frontDir, glm::vec3 upVector, float moveSpeed)
        : position(pos), front(glm::normalize(frontDir)), up(upVector), speed(moveSpeed),
        yaw(-90.0f), pitch(0.0f) {
        updateVectors();
    }

    glm::mat4 getViewMatrix() {
        return glm::lookAt(position, position + front, up);
    }

    void updateVectors() {
        // Calculate the new front vector
        front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        front.y = sin(glm::radians(pitch));
        front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        front = glm::normalize(front);
        // Re-calculate the right and up vector
        right = glm::normalize(glm::cross(front, glm::vec3(0.0f, 1.0f, 0.0f)));
        up = glm::normalize(glm::cross(right, front));
    }

    void moveForward(float deltaTime) {
        position += speed * front * deltaTime;
    }

    void moveBackward(float deltaTime) {
        position -= speed * front * deltaTime;
    }

    void strafeRight(float deltaTime) {
        position += speed * right * deltaTime;
    }

    void strafeLeft(float deltaTime) {
        position -= speed * right * deltaTime;
    }

    void rotate(float offsetX, float offsetY, bool constrainPitch = true) {
        yaw += offsetX;
        pitch += offsetY;

        // Constrain pitch to prevent flipping
        if (constrainPitch) {
            if (pitch > 89.0f) pitch = 89.0f;
            if (pitch < -89.0f) pitch = -89.0f;
        }

        // Update front, right, and up vectors
        updateVectors();
    }
};

