#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <GLFW/glfw3.h>

#include <vector>

const GLfloat SENSIVITY = 0.3f;
const GLfloat SPEED     = 5.0f;
const GLfloat ROLL      = 0.0f;
const GLfloat FOV       = 45.0f;
const glm::vec3 POSITION   (0.0f, 0.0f, 5.0f);
const glm::vec3 UPVECTOR   (0.0f, 1.0f, 0.0f);
const glm::vec3 FRONTVECTOR(0.0f, 0.0f, -1.0f);
const GLfloat ASPECT    = 1.33f;
const GLfloat ZNEAR     = 0.001f;
const GLfloat ZFAR      = 1000.0f;

class Camera {
public:
                           // ��������� ������
    glm::vec3 position;    // ������� ������
    glm::vec3 frontVector; // ������ ������ ��������� ������
    glm::vec3 upVector;    // ������ ������ ��������� �����
    glm::vec3 rightVector; // ������ ������ ��������� ������
    glm::vec3 worldUp;     // ������� ������ ��������� ����� (����� ��� ���������� �����)
    
    GLfloat roll;          // ���� ������
    
    GLfloat speed;         // ��������� ������
    GLfloat sensitivity;   
    GLfloat fov;
    
    GLfloat aspect;        // ��������� �����������
    GLfloat zNear;
    GLfloat zFar;

    Camera();

    glm::mat4 getViewMatrix();           // ������� �������
    glm::mat4 getPerspProjectionMatrix();
    glm::mat4 getOrthoProjectionMatrix(GLfloat right, GLfloat top);
    
    void processKeyboard(GLboolean keys[], GLfloat deltaTime); // ��������� �����
    void processMouse(GLfloat xOffset, GLfloat yOffset);
    void processWhell(GLfloat yOffset);
private:
    void updateVectors();
};
#endif
