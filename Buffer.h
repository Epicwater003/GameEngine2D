#ifndef BUFFER_H
#define BUFFER_H

#include <glad/glad.h>

#include "Vertex.h"
#include <vector>


class BaseBuffer{		                    // ======== ������� ����������� ����� ========
public:
	virtual ~BaseBuffer() = default;
	virtual void Bind() = 0;
	virtual void Unbind() = 0;
	
protected:
	GLuint ID;	 // ID-����
};

class VBO : public BaseBuffer{	            // ======== VBO �����. ����� ������ ========
public:
	VBO(GLfloat* vertices, GLsizeiptr size);
	VBO(std::vector<Vertex>& vertices);
	~VBO();

	void Bind();
	void Unbind();
};

class VAO : public BaseBuffer{	            // ======== VAO �����. ����� ��������� ========
public:
	VAO();
	~VAO();

	void LinkAttrib(VBO& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset);
	void Bind();
	void Unbind();
};

class EBO : public BaseBuffer{		        // ======== EBO �����. ����� ��������� ========
public:
	EBO(GLuint* indices, GLsizeiptr size);
	EBO(std::vector<GLuint> indices);
	~EBO();

	void Bind();
	void Unbind();
};


#endif // !BUFFER_CLASS_H