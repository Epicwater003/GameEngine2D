/*
* ����� GameObject �������� ��:
*	* ����� ��������� ���������
*	* ����� ��������� ��������
*   * ������������� ����������� ������� ����� ���:
*		* ����
*		* ���
*       * ������(����� �������������, ���)
*       * ������� ����������������� ������������
*/


#ifndef IGAMEOBJECT_H
#define IGAMEOBJECT_H

#include <glm/glm.hpp>

#include "Shader.h"
#include "Camera.h"
#include "Mesh.h"


class IGameObject
{
public:
	//GameObject(); // ������ ������� �������� �������
	//virtual IGameObject* GetObjectContract() = 0;	 TODO: �������� �������� �� ����������
	virtual Mesh GetMesh()                   = 0;
	virtual glm::mat4 GetModelMatrix()       = 0;
	virtual glm::vec3 GetColor()             = 0;

	//virtual void SetObjectContract(IGameObject cont) = 0;	TODO: �������� �������� �� ����������
	virtual void SetMesh(Mesh mesh)                  = 0;
	virtual void SetModelMatrix(glm::mat4 mat)       = 0;
	virtual void SetColor(glm::vec3 col)             = 0;

	
	virtual void Draw(Shader& s, Camera& c) = 0;	 // ���������
	virtual void Update()     = 0;                   // ���������� ����������													 
    virtual Mesh CreateMesh() = 0;		             // ������� �����	 | TODO: ���� �����-�� ������� ����������������� ������� ��� ���� ����� �����
	//virtual GameObject* Create() = 0;			     // ����� ��� ���������� ����������� ������� ��������
protected:

private:

};

#endif // !GAMEOBJECT_H

// ��� �������� ���������� ���������� ���������
//Mesh GetMesh() { return mesh; }
//glm::mat4 GetModelMatrix() { return model; }
//glm::vec3 GetColor() { return color; }
//
//void SetMesh(Mesh mesh) { this->mesh = mesh; }
//void SetModelMatrix(glm::mat4 mat) { this->model = mat; }
//void SetColor(glm::vec3 col) { this->color = col; }
//
//void Draw(Shader& s, Camera& c);
//void Update();
//Mesh CreateMesh();

// ��� �������� ���������� ������� ������������� ����������
//Mesh GetMesh() { return ObjectContract->GetMesh(); }
//glm::mat4 GetModelMatrix() { return ObjectContract->GetModelMatrix(); }
//glm::vec3 GetColor() { return ObjectContract->GetColor(); }
//
//void SetMesh(Mesh mesh) { ObjectContract->SetMesh(mesh); }
//void SetModelMatrix(glm::mat4 mat) { ObjectContract->SetModelMatrix(mat); }
//void SetColor(glm::vec3 col) { ObjectContract->SetColor(col); }
//
//void Draw(Shader& s, Camera& c) { ObjectContract->Draw(s, c); }
//void Update();
//Mesh CreateMesh() { return CreateCircleMesh(resolution); }