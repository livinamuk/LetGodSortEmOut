// fragment shader
#version 330 core

layout (location = 0) out vec4 FragColor;

uniform int xMouse;
uniform int yMouse;
uniform int screenWidth;
uniform int screenHeight;
uniform int camX;
uniform int camY;

void main()
{
	vec2 uv = gl_FragCoord.xy  / vec2(screenWidth/screenHeight);
	float camOffsetX = camX + screenWidth / 2;
	float dist = length(uv - vec2(xMouse - camX + screenWidth / 2, screenHeight-yMouse + (camY - screenHeight/2 )));
	vec3 brightness = vec3(1 - (dist * 0.0025));
	FragColor = vec4(brightness, 1);
	//FragColor = vec4(1,1,1,1);
} 