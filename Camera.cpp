#include "Camera.h"

Camera::Camera(): position(POSITION), frontVector(FRONTVECTOR), worldUp(UPVECTOR), roll(ROLL), speed(SPEED), fov(FOV), aspect(ASPECT), zNear(ZNEAR), zFar(ZFAR) {
    updateVectors();
}

glm::mat4 Camera::getViewMatrix() {
    return glm::lookAt(this->position, this->position + this->frontVector, this->upVector);
}
glm::mat4 Camera::getPerspProjectionMatrix() {
    return glm::perspective(fov, aspect, zNear, zFar);
}
glm::mat4 Camera::getOrthoProjectionMatrix(GLfloat right, GLfloat top) {
    return glm::ortho(0.f, right, 0.f, top);
}
#include <iostream>
void Camera::processKeyboard(GLboolean keys[], GLfloat deltaTime) {  // Управление камерой с помощью клавиатуры
                                                                     // TODO: Переписать в виде каллбэка
    GLfloat velocity = speed * deltaTime;
    if (keys[GLFW_KEY_W])
        position += velocity * frontVector;
    if (keys[GLFW_KEY_S])
        position -= velocity * frontVector;
    if (keys[GLFW_KEY_A])
        position -= glm::normalize(glm::cross(frontVector, upVector)) * velocity;
    if (keys[GLFW_KEY_D])
        position += glm::normalize(glm::cross(frontVector, upVector)) * velocity;
    if (keys[GLFW_KEY_SPACE])
        position += velocity * upVector;
    if (keys[GLFW_KEY_LEFT_SHIFT])
        position -= velocity * upVector;
    if (keys[GLFW_KEY_Q])
        roll += 0.5f;
    if (keys[GLFW_KEY_E])
        roll -= 0.5f;
    updateVectors();
}

void Camera::processMouse(GLfloat xOffset, GLfloat yOffset) {        // Управление камерой с помощью мыши
}                                                                    // TODO: Переписать в виде каллбэка

void Camera::processWhell(GLfloat yOffset) {                         // Управление камерой с помощью колесика мыши
    if (this->fov >= 1.0f && this->fov <= 45.0f)                     // TODO: Переписать в виде каллбэка
        this->fov -= yOffset;
    if (fov <= 1.0f)
        this->fov = 1.0f;
    if (fov >= 45.0f)
        this->fov = 45.0f;
}

void Camera::updateVectors() {                                       // Пересчитываем вектора, если изменяется наклон камеры
    
                             
    this->rightVector = glm::normalize(glm::cross(this->frontVector, this->worldUp));
    this->upVector    = glm::rotateZ(glm::normalize(glm::cross(this->rightVector, this->frontVector)), roll);
}