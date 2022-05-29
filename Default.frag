#version 330 core

// Data from engine
uniform mat4 eModel;
uniform mat4 eView;
uniform mat4 eProjection;
uniform vec3 eViewPos;
uniform float eTime;
uniform bool  hasTextures;


vec3 gViewDir;
// Data from vertex shader
in struct_vertOut
{
    vec3 FragPos;
    vec2 TexCoords;
    vec3 Color;
    vec3 Normal; // Mb unnormalized, be carefull
} fragIn;

// Global variables, just for comfort
vec3 gFragPos;
vec2 gTexCoords;
vec3 gColor;
vec3 gNormal;
// Textures
uniform sampler2D DiffTex0;
//uniform sampler2D DiffTex1;
uniform sampler2D SpecTex0;
//uniform sampler2D SpecTex1;

vec4 gDiffuseMap;
vec4 gSpecularMap;

// Color out
out vec4 fragColor;


void main()
{
        gViewDir = normalize(eViewPos - fragIn.FragPos);
        gFragPos = fragIn.FragPos;
        gNormal  = fragIn.Normal;
        gTexCoords = fragIn.TexCoords;
        gColor = fragIn.Color; 
        //gColor = vec3(0.423,0.411,0.376);
        if(hasTextures){
            gDiffuseMap = texture(DiffTex0, fragIn.TexCoords.xy);
            fragColor = vec4(gDiffuseMap);
        }
        else{
            fragColor = vec4(gColor, 1);
        }
}