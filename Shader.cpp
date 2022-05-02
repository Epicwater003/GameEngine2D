#include "Shader.h"

Shader::Shader(std::string vertexShaderFile, std::string fragmentShaderFile) {
	std::string str_vertShaderCode = GetFileContent(vertexShaderFile);
	std::string str_fragShaderCode = GetFileContent(fragmentShaderFile);
	const char* vertShaderCode     = str_vertShaderCode.c_str();
	const char* fragShaderCode     = str_fragShaderCode.c_str();
	
	GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);	 // ������� ������ ������� � ���������� ��� ID-����
	glShaderSource(vertShader, 1, &vertShaderCode , NULL);   // ��������� ��� ������� � ������ ������� �� ��� ID
	glCompileShader(vertShader);                             // �����������
	checkCompileErrors(vertShader, "VERTEX");		         // ���������

	GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);  // ������! �������
	glShaderSource(fragShader, 1, &fragShaderCode, NULL);
	glCompileShader(fragShader);
	checkCompileErrors(fragShader, "FRAGMENT");

	ID = glCreateProgram();             // ������� ������ ��������� ��������� � ���������� � ID-����
	glAttachShader(ID, vertShader);	    // ������� ��� ������� ������� � ��������� ���������
	glAttachShader(ID, fragShader);
	glLinkProgram(ID);				    // ��������� ������
	checkCompileErrors(ID, "PROGRAM");	// �� � ��������� �� ��������

}

Shader::~Shader() {
	//glDeleteProgram(ID); // ��-��-��! RAII � ����, ������ ���� ���� ���������� � ��������� ���������.
}
void Shader::Activate(){
	glUseProgram(ID);	 // ����������� ��������� �� ���� ������
}

void Shader::setBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(this->ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(this->ID, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(this->ID, name.c_str()), value);
}

void Shader::setVec2(const std::string& name, const glm::vec2& value) const
{
	glUniform2fv(glGetUniformLocation(this->ID, name.c_str()), 1, &value[0]);
}
void Shader::setVec2(const std::string& name, float x, float y) const
{
	glUniform2f(glGetUniformLocation(this->ID, name.c_str()), x, y);
}

void Shader::setVec3(const std::string& name, const glm::vec3& value) const
{
	glUniform3fv(glGetUniformLocation(this->ID, name.c_str()), 1, &value[0]);
}
void Shader::setVec3(const std::string& name, float x, float y, float z) const
{
	glUniform3f(glGetUniformLocation(this->ID, name.c_str()), x, y, z);
}

void Shader::setVec4(const std::string& name, const glm::vec4& value) const
{
	glUniform4fv(glGetUniformLocation(this->ID, name.c_str()), 1, &value[0]);
}
void Shader::setVec4(const std::string& name, float x, float y, float z, float w) const
{
	glUniform4f(glGetUniformLocation(this->ID, name.c_str()), x, y, z, w);
}

void Shader::setMat2(const std::string& name, const glm::mat2& mat) const
{
	glUniformMatrix2fv(glGetUniformLocation(this->ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setMat3(const std::string& name, const glm::mat3& mat) const
{
	glUniformMatrix3fv(glGetUniformLocation(this->ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setMat4(const std::string& name, const glm::mat4& mat) const
{
	glUniformMatrix4fv(glGetUniformLocation(this->ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}


bool Shader::checkCompileErrors(unsigned int shader, const char* type) {
	GLint isOk;
	char infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &isOk);
		if (isOk == GL_FALSE)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::SHADER_COMPILE" << std::endl;
			std::cout << "Information:"          << std::endl;
			std::cout << "Type: " << type        << std::endl;
			std::cout << "Log: "  << infoLog     << std::endl;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &isOk);
		if (isOk == GL_FALSE)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::SHADER_LINKING" << std::endl;
			std::cout << "Information:"          << std::endl;
			std::cout << "Type: " << type        << std::endl;
			std::cout << "Log: "  << infoLog     << std::endl;
		}
	}
	return (bool)isOk;
}
std::string Shader::GetFileContent(std::string const& filename) {
	std::string cont;
	std::ifstream file(filename, std::ios::binary);	 // ��������� ����
	if (file){
		file.seekg(0, std::ios::end);				 // ���� � �����  �����
		cont.resize(file.tellg());					 // ������ ������ ������
		file.seekg(0, std::ios::beg);				 // ���� � ������ �����
		file.read(&cont[0], cont.size());			 // ������ ���� � ������
		file.close();								 // ��������� ����
		return(cont);								 // �� �����������, ������������
	}
	std::cout << "ERROR::NO_FILE"                  << std::endl;		 // ���, ��������� ��������� ******
	std::cout << "Information:"                    << std::endl;
	std::cout << "File " << filename << " missing" << std::endl;
	file.close();
	return cont;
}