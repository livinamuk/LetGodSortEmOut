// fragment shader
#version 330 core
out vec4 FragColor;
in vec2 TexCoords;
  
uniform vec3 color;
uniform float strength;
uniform sampler2D fboAttachment;
  
uniform int rotate;

void main()
{
    vec2 rotatedTex = TexCoords;
    
    if (rotate == 1) 
        rotatedTex = vec2(TexCoords.y, TexCoords.x);
    if (rotate == 2) 
        rotatedTex = vec2(-TexCoords.x, TexCoords.y);
    if (rotate == 3) 
        rotatedTex = vec2(-TexCoords.y, -TexCoords.x);
        
    FragColor = texture(fboAttachment, rotatedTex);
    FragColor.rgb *= color * strength;
} 