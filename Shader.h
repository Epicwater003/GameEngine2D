/*
* ��, ������ ����� ��������� ���������, ����� ��� ��������
* ����� �������������� ��������� ���������, ������� ����� ���� ������
* ����� �������� ���������� ������ ����� � ��������� ���������
*/

#ifndef SHADER_H
#define SHADER_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include <glad/glad.h>
#include <glm/glm.hpp>


class Shader
{
public:
	
    Shader(std::string vertexShaderFile, std::string fragmentShaderFile);
	~Shader();
public:
    void Activate();                                                                   // ����������� ��������� �� ���� ������
    void setBool (const std::string& name, bool value)                         const;  // ������������� uniform ���������� ���� bool ��������� ��������� �� �����
    void setInt  (const std::string& name, int value)                          const;  // � ���, ��������� ��� ������?
    void setFloat(const std::string& name, float value)                        const;
    void setVec2 (const std::string& name, const glm::vec2& value)             const;
    void setVec2 (const std::string& name, float x, float y)                   const;
    void setVec3 (const std::string& name, const glm::vec3& value)             const;
    void setVec3 (const std::string& name, float x, float y, float z)          const;
    void setVec4 (const std::string& name, const glm::vec4& value)             const;
    void setVec4 (const std::string& name, float x, float y, float z, float w) const;
    void setMat2 (const std::string& name, const glm::mat2& mat)               const;
    void setMat3 (const std::string& name, const glm::mat3& mat)               const;
    void setMat4 (const std::string& name, const glm::mat4& mat)               const;
	GLuint getID() { return ID; };
private:
	GLuint ID;
	bool checkCompileErrors(unsigned int shader, const char* type);
	std::string GetFileContent(std::string const& filename);
};



#endif // !SHADER_H

