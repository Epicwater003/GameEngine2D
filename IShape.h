#ifndef ISHAPE_H
#define ISHAPE_H

#include <glm/glm.hpp>

#include "Shader.h"
#include "Camera.h"
#include "Mesh.h"


class IShape
{
public:
	
	virtual IShape*   GetShape()             = 0;
	virtual Mesh      GetMesh()              = 0;
	virtual glm::mat4 GetModelMatrix()       = 0;
	virtual glm::vec3 GetColor()             = 0;

	// TODO: �������� ����������� �� ����������� ������
	virtual void SetShape(IShape& shape)             = 0;
	virtual void SetMesh(Mesh mesh)                  = 0;
	virtual void SetModelMatrix(glm::mat4 mat)       = 0;
	virtual void SetColor(glm::vec3 col)             = 0;

	virtual void Draw(Shader& s, Camera& c) = 0;	 // ���������
	virtual void Update()     = 0;                   // ���������� ����������													 
    virtual Mesh CreateMesh() = 0;		             // ������� �����	 | TODO: ���� �����-�� ������� ����������������� ������� ��� ���� ����� �����
	virtual IShape* CreateShape() = 0;
	
protected:

private:

};

#endif // !GAMEOBJECT_H
