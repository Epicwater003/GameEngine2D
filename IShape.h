#ifndef ISHAPE_H
#define ISHAPE_H

#include <memory>

#include <glm/glm.hpp>

#include "Shader.h"
#include "Camera.h"
#include "Mesh.h"


class IShape
{
public:
	
	//virtual IShape   GetShape()             = 0;

	virtual Mesh      GetMesh()              = 0;
	virtual glm::mat4 GetModelMatrix()       = 0;
	virtual glm::vec3 GetColor()             = 0;

	// TODO: �������� ����������� �� ����������� ������
	virtual void SetShape(IShape& shape)             = 0;
	virtual void SetMesh(Mesh& mesh)                 = 0;

	virtual void SetVertices(std::vector<Vertex>& v) = 0;
	virtual void SetIndices(std::vector<GLuint>& i) = 0;
	virtual void SetTextures(std::vector<Texture>& t) = 0;
	virtual std::vector<Vertex>  GetVertices() = 0;
	virtual std::vector<GLuint>  GetIndices()  = 0;
	virtual std::vector<Texture> GetTextures() = 0;
		
	virtual void SetModelMatrix(glm::mat4 mat)       = 0;
	virtual void SetColor(glm::vec3 col)             = 0;

	virtual void Draw(Shader& s, Camera& c) = 0;	 // ���������
	virtual void Update(float dt)     = 0;                   // ���������� ����������													 
    virtual std::unique_ptr<Mesh> CreateMesh() = 0;		             // ������� �����	 | TODO: ���� �����-�� ������� ����������������� ������� ��� ���� ����� �����
	virtual std::unique_ptr<IShape> CreateShape() = 0;
	
protected:

private:

};

#endif // !GAMEOBJECT_H
