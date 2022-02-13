#version 450
layout(location = 0) in vec4 pos;
layout(location = 1) in vec2 tex;

layout(location = 0) out float posY;
layout(location = 1) out vec2 texPos;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main()
{
    posY = 2*pos.y/256;
    texPos = tex;
    gl_Position = projectionMatrix * viewMatrix * pos;
}