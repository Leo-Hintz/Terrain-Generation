#version 450
layout(location = 0) in float posY;
layout(location = 1) in vec2 tex;

layout(location = 0) out vec4 col;

uniform sampler2D colorTexture;

void main()
{
	if(posY <= 0)
		col = vec4(0.2,0.32,0.74,1);
	if(posY > 0)
			col = vec4(0.65,0.5,.1,1);
	if(posY > 0.1)
			col = vec4(0,1,0,1) * vec4(posY, posY, posY, 1);
	if(posY > 0.3)
		col = vec4(1, 0.6, 0.5, 1) * vec4(posY, posY, posY, 1);
		
	if (posY > 0.5)  col = vec4(1.0,1.0,1.0, 1) * vec4(posY, posY, posY, 1); 
	
	col = texture(colorTexture, tex) * vec4(posY, posY, posY, 1);
}