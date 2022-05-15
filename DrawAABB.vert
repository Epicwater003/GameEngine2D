#version 330 core


layout (location = 0) in vec3 vPos;         // Позиция вершины
layout (location = 2) in vec3 vColor;       // Цвет вершины



// Данные из движка
uniform mat4 eModel;
uniform mat4 eView;
uniform mat4 eProjection;

// Передаем в фрагментный шейдер

out  vec3 Color;


void main(){ 
    vec3 FragPos = vPos;
    Color = vColor;
    gl_Position = eProjection * eView * vec4(FragPos, 1.0);
}