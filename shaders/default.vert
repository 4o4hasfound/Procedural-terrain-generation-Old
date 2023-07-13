#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;  

out vec3 FragPos;
out vec3 worldPos;
out vec3 Normal;
out vec3 ourColor;
out vec2 TexCoord;
out mat3 TBN;
flat out float snowHeight;
flat out float grassHeight;
flat out float waterHeight;

uniform vec3 viewPos;
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform float height;
uniform float snow;
uniform float grass;
uniform float water;

uniform sampler2D heightMap;

void main()
{
    vec3 Pos = vec3(aPos.x, aPos.y*height, aPos.z);
    worldPos = Pos;
    FragPos = vec3(model * vec4(Pos, 1.0));
    Normal = aNormal;  
    gl_Position = projection * view * vec4(Pos, 1.0);
    TexCoord=aTexCoord;
    vec3 T = normalize(vec3(model * vec4(aTangent,   0.0)));
    vec3 B = normalize(vec3(model * vec4(aBitangent, 0.0)));
    vec3 N = normalize(vec3(model * vec4(aNormal,    0.0)));
    TBN = mat3(T, B, N);
    snowHeight = snow;
    grassHeight = grass;
    waterHeight = water;
}