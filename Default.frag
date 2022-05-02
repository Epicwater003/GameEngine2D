#version 330 core

// Данные из движка
uniform mat4 eModel;
uniform mat4 eView;
uniform mat4 eProjection;
uniform vec3 eViewPos;
uniform float eTime;
uniform bool  hasTextures;

//
vec3 gViewDir;
// Получаем данные фрагмента из вершинного шейдера
in struct_vertOut
{
    vec3 FragPos;
    vec3 Normal; // Может быть ненормализованна!
    vec2 TexCoords;
    vec3 Color;
} fragIn;

// Глобальные(для удобства доступа) переменные подготовленные для использования 
vec3 gFragPos;
vec3 gNormal;
vec2 gTexCoords;
vec3 gColor;

// Текстурные блоки
uniform sampler2D DiffTex0;
//uniform sampler2D DiffTex1;
uniform sampler2D SpecTex0;
//uniform sampler2D SpecTex1;

vec4 gDiffuseMap;
vec4 gSpecularMap;

// Передаем цвет дальше по конвееру
out vec4 fragColor;


void main()
{
        gViewDir = normalize(eViewPos - fragIn.FragPos);
        gFragPos = fragIn.FragPos;
        gNormal  = fragIn.Normal;
        gTexCoords = fragIn.TexCoords;
        gColor = fragIn.Color; 
        if(hasTextures){
            gDiffuseMap = texture(DiffTex0, fragIn.TexCoords);
            fragColor = vec4(gDiffuseMap);
        }
        else{
            fragColor = vec4(1,gColor);
        }
}