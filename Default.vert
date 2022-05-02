#version 330 core


layout (location = 0) in vec3 vPos;         // ������� �������
layout (location = 1) in vec2 vTexCoords;   // ���������� ���������� �������
layout (location = 2) in vec3 vColor;       // ���� �������
layout (location = 3) in vec3 vNormal;      // ������� �������



// ������ �� ������
uniform mat4 eModel;
uniform mat4 eView;
uniform mat4 eProjection;
uniform vec3 eViewPos;
uniform float eTime;

// �������� � ����������� ������
out struct_vertOut
{
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec3 Color;
} vertOut;

void main(){ 
    vertOut.FragPos    = vec3(eModel * vec4(vPos, 1.0));
    vertOut.Normal     = normalize(mat3(transpose(inverse(eModel))) * vNormal);
    vertOut.TexCoords  = vTexCoords;
    vertOut.Color      = vColor;
    gl_Position        = eProjection * eView * vec4(vertOut.FragPos, 1.0);
}