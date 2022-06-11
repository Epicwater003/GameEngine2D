/*
* Курсовая работа по ООП МИРЭА второй семестр
* << Симуляция движения физических объектов >>
* Подготовил Волобуев Илья Владимирович
*/

/*
* Правила стиля:
* Имена, представляющие типы, должны быть обязательно написаны в смешанном регистре, начиная с верхнего
* Имена переменных должны быть записаны в смешанном регистре, начиная с нижнего
* Именованные константы (включая значения перечислений) должны быть записаны в верхнем регистре с нижним подчёркиванием в качестве разделителя
* Аббревиатуры и сокращения в именах должны записываться в нижнем регистре
* Глобальные переменные всегда следует использовать с оператором разрешения области видимости (::)
* Имена полей класса, предназначенных для отправки в шейдерную программу, должны соответствовать именам переменных в шейдерной программе
* Разделы класса public, protected и private должны быть отсортированы. Все разделы должны быть явно указаны
* Следует инициализировать переменные в месте их объявления
* Логические блоки следует разделять пустой строкой
* Следует использовать выравнивание везде, где это улучшает читаемость
* Правила созданы, чтобы их нарушать :D (если очень нужно)
// Source: https://habr.com/ru/post/172091/
*/

#include <iostream>
#include <vector>
#include <string>
#define _USE_MATH_DEFINES
#include <math.h>
#include <memory>

#include <glm/glm.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include "Camera.h"
#include "Vertex.h"
#include "Texture.h"
#include "Mesh.h"
#include "Circle.h"
#include "Square.h"
#include "ICollisionEngine.h"
#include "SAT2D.h"
#include "GameEngine.h"
#include "DrawablePrimitive.h"
#include "ImpulseResponse.h"


void calculateGlobalDeltaTime();
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouseCallback(GLFWwindow* window, double xpos, double ypos);
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void framebufferSizeCallback(GLFWwindow* window, int width, int height);

const GLint SCREEN_WIDTH  = 1366;
const GLint SCREEN_HEIGHT = 768;

glm::vec2 lastMousePos(::SCREEN_WIDTH / 2, ::SCREEN_HEIGHT / 2);

bool imGuiShowDemoWindow = false;

bool processKeyboard    =  true;
bool processMouseMove   =  true;
bool processMouseClick  =  true;
bool processMouseScroll =  true;

GLboolean pressedKeys[1024];
GLboolean pressedMouse[512];

GLfloat deltaTime = 0.0f;	    // Время, прошедшее между последним и текущим кадром
GLfloat lastFrameTime = 0.0f;  	// Время вывода последнего кадра

Camera mainCamera;


const float fps = 100;
const float dt = 1 / fps;


int main() {
	
	glfwInit();                                     // Инициализация библиотеки glfw
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); 	// Установка версии OpenGL
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	glfwWindowHint(GLFW_SAMPLES, 8);                  // Включаем MSAA сглаживание
	
	
	GLFWwindow* window = glfwCreateWindow(::SCREEN_WIDTH, ::SCREEN_HEIGHT, "GameEngine2D", NULL, NULL); // Создание окна
	if (!window)
	{
		std::cout << "Failed to create window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);                                  // Создаем контекст окна
	
	// glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);     // Включаем ввод мыши и отключаем курсор
	glfwSetCursorPos(window, ::lastMousePos.x, ::lastMousePos.y);    // Ставим курсор в начальную позицию

	glfwSetCursorPosCallback  (window, mouseCallback);               // Привязывем каллбэк функцию обработки положения мыши
	glfwSetMouseButtonCallback(window, mouseButtonCallback);         // Привязывем каллбэк функцию клавиш обработки  мыши
	glfwSetScrollCallback     (window, scrollCallback);              // Привязывем каллбэк функцию обработки колесика мыши
	glfwSetKeyCallback        (window, keyCallback);                 // Привязываем каллбэк функцию обработки клавиш
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback); // Привязывем каллбэк функцию обработки изменения размера окна
	
	

	gladLoadGL();									   // Загружаем функции OpenGL через библиотеку glad
	glViewport(0, 0, ::SCREEN_WIDTH, ::SCREEN_HEIGHT); // Передаем OpenGl размер окна
	//glEnable(GL_DEPTH_TEST);                           // Включаем использование буфера глубины
	glEnable(GL_MULTISAMPLE);                          // Включаем MSAA
	glEnable(GL_PROGRAM_POINT_SIZE);
	glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
	glPointSize(8.); // Размер отрисовки 1 пикселя GL_DRAW_POINTS
	glLineWidth(3.); // Ширина отрисовки линии GL_DRAW_LINES
	glClearColor(0.902, 0.894, 0.847, 1.);             // Цвет фона после очистки экрана
	
	

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Включает управление клавиатурой
	
	ImGui::StyleColorsDark();                   // Устанавливаем стиль интерфейса imgui
	ImGui_ImplGlfw_InitForOpenGL(window, true); // Настройка бэкэнда
	ImGui_ImplOpenGL3_Init("#version 330");     // Устанавливаем версию OpenGL
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	


	// ===================== TEST ZONE! NO ENTER! =========================

	

	mainCamera.aspect = (float)::SCREEN_WIDTH / (float)::SCREEN_HEIGHT;
	mainCamera.zNear = 0.01f;
	mainCamera.zFar = 1000.f;

	
	
	Shader shaderProgram("Default.vert", "Default.frag");
	
	// ===================== TEST ZONE! NO ENTER! =========================
	DrawablePrimitive::Line l;
	DrawablePrimitive::Point p;

	std::shared_ptr < ICollisionEngine > collisionNarrow = std::make_shared< SAT2D >();
	std::shared_ptr <IResponseEngine> responseEngine = std::make_shared< ImpulseResponseNFNR>();
	std::shared_ptr<IResponseEngine> responseEngineWithFriction = std::make_shared<ImpulseResponseWFNR>();
	std::shared_ptr<IResponseEngine> responseEngineWithRotation = std::make_shared<ImpulseResponseWFWR>();

	static float xCoord = 2;
	static float yCoord = 1;
	static float radius = 2;
	static float gunPower = 1;

	std::unique_ptr<Texture> texture = std::make_unique<Texture>("planks.png", "Diff", 0, GL_RGBA, GL_UNSIGNED_BYTE);
	std::vector <Texture> tex;
	tex.push_back(*texture);
	

	GameEngine ge;
	ge.doStep(1);
	ge.CreateSquareObject(1000, 2,glm::vec3(0, -4, 0), glm::vec3(0),glm::vec3(0.2,0.2,0.2), 0, FLT_MAX, true);
	
	ge.CreateSquareObject(15, 3, glm::vec3(25, 5.1, 0), glm::vec3(0), glm::vec3(0.2, 0.5, 0.5), 0, 50, false);
	ge.CreateSquareObject(8, 8, glm::vec3(25, 13.2, 0), glm::vec3(0), glm::vec3(0.2, 0.5, 0.8), 0, 50, false);
	
	ge.CreateSquareObject(4, 8, glm::vec3(30, -1, 0), glm::vec3(0), glm::vec3(0.1, 0.3, 0.3), 0, 50, false);
	ge.CreateSquareObject(4, 8, glm::vec3(20, -1, 0), glm::vec3(0), glm::vec3(0.3, 0.3, 0.3), 0, 50, false);
	ge.doStep(0);
	static float acum = 0;
	while (!glfwWindowShouldClose(window)){	                // Игровой цикл
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Очищаем экран
		
		// ===================== TEST ZONE! NO ENTER! =========================
		
		acum += deltaTime;
		if (acum >= 0.2) {
			acum = 0.2;
		}
		//if(acum >= dt){
			ge.doStep(dt);
		//	acum -= dt;
		//}
		ge.Render(shaderProgram, ::mainCamera);
		l.setColor(glm::vec3(1));
		l.setPoints(glm::vec3(0), glm::vec3(xCoord, yCoord, 0));
		l.Draw(::mainCamera.getPerspProjectionMatrix()* ::mainCamera.getViewMatrix());


		
		// ===================== TEST ZONE! NO ENTER! =========================
		
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::Begin("Hello, world!");                                // Создать окно с названием "Hello, world!"
		ImGui::Text(" %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate); // Счетчик кадров

		ImGui::SliderFloat("X", &xCoord, -6.f, 6.f);
		ImGui::SliderFloat("Y", &yCoord, -6.f, 6.f);
		ImGui::SliderFloat("Radius", &radius, 0.5, 10.f);
		ImGui::SliderFloat("Gun power", &gunPower, 1, 16.f);
		if (ImGui::Button("Fire!"))
			ge.CreateCircleObject(radius, 70,tex, glm::vec3(0), glm::vec3(xCoord*gunPower, yCoord*gunPower, 0), glm::vec3(0), 0, radius*20);
		if (ImGui::Button("Clear"))
			ge.Clear();

		ImGui::End();                                                 // Завершить обработку окна с названием "Hello, world!"
		ImGui::EndFrame();
		ImGui::Render();         // Рисуем интерфейс с помощью imgui
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		
		mainCamera.processKeyboard(::pressedKeys, ::deltaTime);
		calculateGlobalDeltaTime();
		glfwPollEvents();        // Обработка всех событий ввода
		glfwSwapBuffers(window); // Двойная буфферизация
	}

	// Освобождаем ресурсы
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}


void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) { // Обработка нажатых клавиш
	if (::processKeyboard) {
		if (action == GLFW_PRESS)
			::pressedKeys[key] = true;
		else if (action == GLFW_RELEASE)
			::pressedKeys[key] = false;
		
		if (key == GLFW_KEY_P && action == GLFW_PRESS) {
			::imGuiShowDemoWindow = !::imGuiShowDemoWindow;
		}
		
	}
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos) {                  // Обработка передвижения мыши
	if (::processMouseMove) {
		GLfloat xOffset = static_cast<GLfloat>(xpos) - lastMousePos.x;
		GLfloat yOffset = lastMousePos.y - static_cast<GLfloat>(ypos);              // Обратный порядок вычитания потому что оконные Y-координаты возрастают с верху вниз 
		lastMousePos = { static_cast<GLfloat>(xpos), static_cast<GLfloat>(ypos) };
	}
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {    // Обработка кнопок мыши
	if (::processMouseClick) {
		if (action == GLFW_PRESS)
		{
			::pressedMouse[button] = true;
		}
		else if (action == GLFW_RELEASE) {
			::pressedMouse[button] = false;
		}
		if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
			std::cout << "\a";
	}
}


void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {           // Обработка колесика мыши
	if (::processMouseScroll) {
		GLfloat wheelOffset = static_cast<GLfloat>(yoffset);
	}
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {           // Обработка изменения размера окна
	glViewport(0, 0, width, height);
}

void calculateGlobalDeltaTime() {
	GLfloat currentFrame = static_cast<GLfloat>(glfwGetTime());
	::deltaTime = currentFrame - ::lastFrameTime;
	::lastFrameTime = currentFrame;
}