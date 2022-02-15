#version 450

layout(location = 0) in vec3 tex;
layout(location = 0) out vec4 col;

uniform samplerCube cubemap;

void main()
{
	col = texture(cubemap, tex);
}