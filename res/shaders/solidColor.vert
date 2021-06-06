// vertex shader
#version 330 core
layout (location = 0) in vec2 position;

uniform mat4 view;

void main()
{
    gl_Position = view * vec4(position, 0.0f, 1.0f);
}
 