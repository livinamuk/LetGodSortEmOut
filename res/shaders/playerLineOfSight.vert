// vertex shader
#version 330 core
layout (location = 0) in vec2 position;
layout (location = 1) in vec2 texCoords;

uniform mat4 view;

//out vec2 TexCoords;

void main()
{
    gl_Position = view * vec4(position, 0.0f, 1.0f);
   // TexCoords = texCoords;
}
 