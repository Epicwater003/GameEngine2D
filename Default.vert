#version 330 core


layout (location = 0) in vec3 vPos;         // Vertex position
layout (location = 1) in vec2 vTexCoords;   // Vertex texture coordinate
layout (location = 2) in vec3 vColor;       // Vertex color
layout (location = 3) in vec3 vNormal;      // Vertex normal



// Data from engine
uniform mat4 eModel;
uniform mat4 eView;
uniform mat4 eProjection;
uniform vec3 eViewPos;
uniform float eTime;

// To fragment shader
out struct_vertOut
{
    vec3 FragPos;
    vec2 TexCoords;
    vec3 Color;
    vec3 Normal;
} vertOut;

void main(){ 
    vertOut.FragPos    = vec3(eModel * vec4(vPos, 1.0));
    vertOut.Normal     = normalize(mat3(transpose(inverse(eModel))) * vNormal);
    vertOut.TexCoords  = vTexCoords;
    vertOut.Color      = vec3(vColor.x, vColor.y, vColor.z);
    gl_Position        = eProjection * eView * vec4(vertOut.FragPos, 1.0);
}