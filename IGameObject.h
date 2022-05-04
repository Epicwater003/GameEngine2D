/*
* Класс GameObject отвечает за:
*	* Общий интерфейс отрисовки
*	* Общий интерфейс создания
*   * Отличительные особенности объекта такие как:
*		* Цвет
*		* Тип
*       * Модель(сетка треугольников, меш)
*       * Матрица пространственного расположения
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
	//GameObject(); // Запрет прямого создания объекта
	//virtual IGameObject* GetObjectContract() = 0;	 TODO: Улучшить контракт на реализацию
	virtual Mesh GetMesh()                   = 0;
	virtual glm::mat4 GetModelMatrix()       = 0;
	virtual glm::vec3 GetColor()             = 0;

	//virtual void SetObjectContract(IGameObject cont) = 0;	TODO: Улучшить контракт на реализацию
	virtual void SetMesh(Mesh mesh)                  = 0;
	virtual void SetModelMatrix(glm::mat4 mat)       = 0;
	virtual void SetColor(glm::vec3 col)             = 0;

	
	virtual void Draw(Shader& s, Camera& c) = 0;	 // Отрисовка
	virtual void Update()     = 0;                   // Перерасчет параметров													 
    virtual Mesh CreateMesh() = 0;		             // Создает сетку	 | TODO: Надо каким-то образом стандартизировать функцию для всех типов сеток
	//virtual GameObject* Create() = 0;			     // Нужно для реализации абстрактной фабрики объектов
protected:

private:

};

#endif // !GAMEOBJECT_H

// Для простоты разработки реализаций контракта
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

// Для простоты разработки объекта использующего реализацию
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