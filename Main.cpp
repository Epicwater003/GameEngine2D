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
#include <fstream>
#include <vector>
#include <string>

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
#include "ICollisionEngine.h"
#include "AABB2D.h"
#include "SAT2D.h"
#include "GameEngine.h"
#include "DrawablePrimitive.h"


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

GLfloat deltaTime = 0.0f;	    // Время, прошедшее между последним и текущим кадром
GLfloat lastFrameTime = 0.0f;  	// Время вывода последнего кадра

Camera mainCamera;


int main() {
	
	glfwInit();                                     // Инициализация библиотеки glfw
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); 	// Установка версии OpenGL
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	glfwWindowHint(GLFW_SAMPLES, 4);                  // Включаем MSAA сглаживание
	
	
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

	Texture textures[]
	{
		Texture("planks.png", "Diff", 0, GL_RGBA, GL_UNSIGNED_BYTE)
	};
	std::vector <Texture> tex(textures, textures + sizeof(textures) / sizeof(Texture));
	
	Shader shaderProgram("Default.vert", "Default.frag");
	Shader AABBShader("DrawAABB.vert", "DrawAABB.frag");
	
	DrawablePrimitive::Line l(glm::vec3(0., 0., 0.), glm::vec3(0.5, 0.5, 0.));

	Circle c;
	Circle cc;
	Circle ccc0;
	Circle ccc1;
	Circle ccc2;
	Circle ccc3;

	std::vector<GameObject> gameObjects;

	gameObjects.push_back(std::move(ccc0));
	gameObjects.push_back(std::move(ccc1));
	gameObjects.push_back(std::move(ccc2));
	gameObjects.push_back(std::move(ccc3));
	/*gameObjects.emplace_back(ccc0);
	gameObjects.emplace_back(ccc1);
	gameObjects.emplace_back(ccc2);
	gameObjects.emplace_back(ccc3);*/


	static int res = 2;
	static float rad = 1;
	static int oldres = -1;
	static float oldrad = 1;
	static float arrowUp = 0;
	static float arrowRight = 0;
	static float collide = 0;
	static float isCollide = 0;
	cc.SetTextures(tex);
	cc.MoveToPosition(glm::vec3(1.5, 0, 0));
	//c.SetTextures(tex);

	glm::mat4 m(1.);
	AABB2D* collision = new AABB2D();
	SAT2D* collision2 = new SAT2D();
	// ===================== TEST ZONE! NO ENTER! =========================

	while (!glfwWindowShouldClose(window)){	                // Игровой цикл
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Очищаем экран
		
		// ===================== TEST ZONE! NO ENTER! =========================
		arrowRight = 0;
		arrowUp = 0;
		if (::pressedKeys[GLFW_KEY_LEFT]) {
			arrowRight -= 0.01;
		}
		if (::pressedKeys[GLFW_KEY_RIGHT]) {
			arrowRight += 0.01;
		}
		if (::pressedKeys[GLFW_KEY_UP]) {
			arrowUp += 0.01;
		}
		if (::pressedKeys[GLFW_KEY_DOWN]) {
			arrowUp -= 0.01;
		}


		if(oldres!=res || oldrad !=rad){
			cc.resolution = res;
			c.resolution = res;
			c.radius = rad;
			cc.Reshape();
			c.Reshape();
			oldres = res;
			oldrad = rad;
		}

		l.setPoints(glm::vec3(-4, 4, 0), glm::vec3(4, 4, 0));
		l.Draw(::mainCamera.getPerspProjectionMatrix() * ::mainCamera.getViewMatrix());
		

		
		static float angle = 0;
		angle += 0.3;
		m = glm::translate(m, glm::vec3(0.001, 0., 0.));
		cc.Rotate(-angle);
		c.Rotate(angle);
		c.Move(glm::vec3(arrowRight, arrowUp,0));

		
		c.Update();
		cc.Update();
		c.Draw(shaderProgram, ::mainCamera);
		cc.Draw(shaderProgram, ::mainCamera);
		collide = 0;
		CollisionProperties2D CollisionProps = {false, 0};
		for (int i = 0, s = gameObjects.size(); i < s;i++) {
			if (i % 2)
				gameObjects[i].Rotate(-angle);
			gameObjects[i].MoveToPosition(glm::vec3(2 * i, 2 * i, 0));

			gameObjects[i].Update();
			gameObjects[i].Draw(shaderProgram, ::mainCamera);
			CollisionProps = collision2->isCollide(c, gameObjects[i], ::mainCamera.getPerspProjectionMatrix() * ::mainCamera.getViewMatrix());
			if (CollisionProps.isCollide){
				collide = CollisionProps.penetration;
				glm::vec2 axis = CollisionProps.Normal;

				c.Move(glm::vec3(CollisionProps.ReactionA, 0) * collide);
			}
			
		}

		
		CollisionProps = collision2->isCollide(c, cc, ::mainCamera.getPerspProjectionMatrix() * ::mainCamera.getViewMatrix());
		if (CollisionProps.isCollide){
			collide = CollisionProps.penetration;
			glm::vec2 axis = CollisionProps.Normal;

			c.Move(glm::vec3(CollisionProps.ReactionA, 0) * collide/2.f );
			cc.Move(glm::vec3(CollisionProps.ReactionB, 0) * collide/2.f);
			
			
			c.Update();
			cc.Update();

		}
	
		
		// ===================== TEST ZONE! NO ENTER! =========================

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		if (::imGuiShowDemoWindow)
			ImGui::ShowDemoWindow(&::imGuiShowDemoWindow);            // Показать окно с демонстрацией возможностей imgui

		ImGui::Begin("Hello, world!");                                // Создать окно с названием "Hello, world!"
		ImGui::Checkbox("Show demo window?", &::imGuiShowDemoWindow); // Здесь пишем логику окна
		ImGui::SliderInt("resolution ", &res, 2, 70);
		ImGui::SliderFloat("radius ", &rad, 0.5, 10);
		ImGui::End();                                                 // Завершить обработку окна с названием "Hello, world!"

		ImGui::Begin("Camera stats");
		std::string stat = std::to_string(mainCamera.position.x);
		stat += " " + std::to_string(mainCamera.position.y);
		stat += " " + std::to_string(mainCamera.position.z);
		ImGui::Text(stat.c_str());
		ImGui::Text((std::string("Is collide: ") + std::to_string(collide)).c_str());
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
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