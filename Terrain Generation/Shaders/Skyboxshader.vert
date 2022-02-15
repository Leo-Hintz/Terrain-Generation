#version 450
layout(location = 0) in vec4 pos;

layout(location = 0) out vec3 texPos;

uniform mat3 viewMatrix;
uniform mat4 projectionMatrix;

void main()
{
	gl_Position = projectionMatrix * vec4(viewMatrix * vec3(pos).xyz,1);
	texPos = vec3(pos.x, pos.y, -pos.z);
}