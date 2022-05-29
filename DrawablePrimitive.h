#ifndef DRAWABLEPRIMITIVE_H
#define DRAWABLEPRIMITIVE_H

#include <vector>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include "Shader.h"
#include "Buffer.h"
#include "Camera.h"


namespace DrawablePrimitive
{

class Line {
    std::unique_ptr<Shader> shaderProgram;
    std::unique_ptr<VAO>    Vao = std::make_unique<VAO>();
    

    unsigned int VBO_, VAO_;
    std::vector<GLfloat> vertices;
    glm::vec3 startPoint = glm::vec3(0., 0., 0.);
    glm::vec3 endPoint = glm::vec3(1., 0., 0.);
    glm::mat4 MWP = glm::mat4(1.0f);
    glm::vec3 Color = glm::vec3(1., 0., 0.);

public:
    Line()
    {

        const char* vertexShaderSource = "#version 330 core\n"
            "layout (location = 0) in vec3 aPos;\n"
            "uniform mat4 MWP;\n"
            "void main()\n"
            "{\n"
            "   gl_Position = MWP * vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
            "}\0";
        const char* fragmentShaderSource = "#version 330 core\n"
            "out vec4 FragColor;\n"
            "uniform vec3 Color;\n"
            "void main()\n"
            "{\n"
            "   FragColor = vec4(Color, 1.0f);\n"
            "}\n\0";
        shaderProgram = std::make_unique<Shader>(vertexShaderSource, fragmentShaderSource, 666);
        vertices = {
             startPoint.x, startPoint.y, startPoint.z,
             endPoint.x, endPoint.y, endPoint.z,
        };    
       
        Vao->Bind();
        VBO Vbo(vertices.data(), sizeof(vertices));
        Vao->LinkAttrib(Vbo, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);
        
        Vao->Unbind();
        Vbo.Unbind();
    }
    Line(glm::vec3 start, glm::vec3 end) :
        startPoint(start),
        endPoint(end)

    {

        const char* vertexShaderSource = "#version 330 core\n"
            "layout (location = 0) in vec3 aPos;\n"
            "uniform mat4 MWP;\n"
            "void main()\n"
            "{\n"
            "   gl_Position = MWP * vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
            "}\0";
        const char* fragmentShaderSource = "#version 330 core\n"
            "out vec4 FragColor;\n"
            "uniform vec3 Color;\n"
            "void main()\n"
            "{\n"
            "   FragColor = vec4(Color, 1.0f);\n"
            "}\n\0";
        shaderProgram = std::make_unique<Shader>(vertexShaderSource, fragmentShaderSource, 666);
        vertices = {
             start.x, start.y, start.z,
             end.x, end.y, end.z,
        };

        Vao->Bind();
        VBO Vbo(vertices.data(), sizeof(vertices));
        Vao->LinkAttrib(Vbo, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);

        Vao->Unbind();
        Vbo.Unbind();
    }

    void setMWP(glm::mat4 mwp) {
        MWP = glm::mat4(mwp);
    }

    void setColor(glm::vec3 color) {
        Color = color;
    }
    void setPoints(glm::vec3 start, glm::vec3 end) {
        startPoint = start;
        endPoint = end;
        vertices = {
             startPoint.x, startPoint.y, startPoint.z,
             endPoint.x, endPoint.y, endPoint.z,
        };

        Vao->Bind();
        VBO Vbo(vertices.data(), sizeof(vertices));
        Vao->LinkAttrib(Vbo, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);

        Vao->Unbind();
        Vbo.Unbind();
    }
    void setPoints(glm::vec2 start, glm::vec2 end) {
        startPoint = glm::vec3(start,0.f);
        endPoint = glm::vec3(end,0.f);
        vertices = {
             startPoint.x, startPoint.y, startPoint.z,
             endPoint.x, endPoint.y, endPoint.z,
        };

        Vao->Bind();
        VBO Vbo(vertices.data(), sizeof(vertices));
        Vao->LinkAttrib(Vbo, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);

        Vao->Unbind();
        Vbo.Unbind();
    }

    void Draw() {
        shaderProgram->Activate();

        Vao->Bind();

        shaderProgram->setMat4("MWP", MWP);
        shaderProgram->setVec3("Color", Color);
        glDrawArrays(GL_LINES, 0, 2);
        Vao->Unbind();
    }
    void Draw(glm::mat4 mwp) {
        setMWP(mwp);
        Draw();
    }
};

class Point {
    std::unique_ptr<Shader> shaderProgram;
    std::unique_ptr<VAO>    Vao = std::make_unique<VAO>();


    unsigned int VBO_, VAO_;
    std::vector<GLfloat> vertices;
    glm::vec3 position = glm::vec3(0., 0., 0.);
    glm::mat4 MWP = glm::mat4(1.0f);
    glm::vec3 Color = glm::vec3(1., 0., 0.);

public:
    Point()
    {

        const char* vertexShaderSource = "#version 330 core\n"
            "layout (location = 0) in vec3 aPos;\n"
            "uniform mat4 MWP;\n"
            "void main()\n"
            "{\n"
            "   gl_Position = MWP * vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
            "}\0";
        const char* fragmentShaderSource = "#version 330 core\n"
            "out vec4 FragColor;\n"
            "uniform vec3 Color;\n"
            "void main()\n"
            "{\n"
            "   FragColor = vec4(Color, 1.0f);\n"
            "}\n\0";
        shaderProgram = std::make_unique<Shader>(vertexShaderSource, fragmentShaderSource, 666);
        vertices = {
             position.x, position.y, position.z,
        };

        Vao->Bind();
        VBO Vbo(vertices.data(), sizeof(vertices));
        Vao->LinkAttrib(Vbo, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);

        Vao->Unbind();
        Vbo.Unbind();
    }
    Point(glm::vec3 position) :
        position(position)

    {

        const char* vertexShaderSource = "#version 330 core\n"
            "layout (location = 0) in vec3 aPos;\n"
            "uniform mat4 MWP;\n"
            "void main()\n"
            "{\n"
            "   gl_Position = MWP * vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
            "}\0";
        const char* fragmentShaderSource = "#version 330 core\n"
            "out vec4 FragColor;\n"
            "uniform vec3 Color;\n"
            "void main()\n"
            "{\n"
            "   FragColor = vec4(Color, 1.0f);\n"
            "}\n\0";
        shaderProgram = std::make_unique<Shader>(vertexShaderSource, fragmentShaderSource, 666);
        vertices = {
             position.x, position.y, position.z,
        };

        Vao->Bind();
        VBO Vbo(vertices.data(), sizeof(vertices));
        Vao->LinkAttrib(Vbo, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);

        Vao->Unbind();
        Vbo.Unbind();
    }

    void setMWP(glm::mat4 mwp) {
        MWP = glm::mat4(mwp);
    }

    void setColor(glm::vec3 color) {
        Color = color;
    }
    void setPoint(glm::vec3 position) {
        this->position = position;
        vertices = {
             position.x, position.y, position.z,
        };

        Vao->Bind();
        VBO Vbo(vertices.data(), sizeof(vertices));
        Vao->LinkAttrib(Vbo, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);

        Vao->Unbind();
        Vbo.Unbind();
    }
    void setPoint(glm::vec2 position) {
        this->position = glm::vec3(position, 0.f);
        vertices = {
             this->position.x, this->position.y, this->position.z,
        };

        Vao->Bind();
        VBO Vbo(vertices.data(), sizeof(vertices));
        Vao->LinkAttrib(Vbo, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);

        Vao->Unbind();
        Vbo.Unbind();
    }

    void Draw() {
        shaderProgram->Activate();

        Vao->Bind();

        shaderProgram->setMat4("MWP", MWP);
        shaderProgram->setVec3("Color", Color);
        glDrawArrays(GL_POINTS, 0, 1);
        Vao->Unbind();
    }
    void Draw(glm::mat4 mwp) {
        setMWP(mwp);
        Draw();
    }
};



 }
#endif // !DRAWABLEPRIMITIVE_H
