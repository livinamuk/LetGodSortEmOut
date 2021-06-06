// fragment shader
#version 440 core
out vec4 FragColor;

layout (binding = 0) uniform sampler2D inputTexture;

uniform int screenWidth;
uniform int screenHeight;
uniform vec3 color;
uniform float strength;

uniform vec2 lightScreenSpacePixelPosition;
uniform float spriteWidth;
uniform float spriteHeight;
uniform float lightScale;
uniform int rotate;
  
float Map(float value, float min1, float max1, float min2, float max2)
{
     float perc = (value - min1) / (max1 - min1);
     float val = perc * (max2 - min2) + min2;
     return val;
}

void main()
{  
    float worldRangeLowerX = lightScreenSpacePixelPosition.x - spriteWidth*lightScale/2;
    float worldRangeLowerY = lightScreenSpacePixelPosition.y - spriteHeight*lightScale/2;

    float worldRangeUpperX = worldRangeLowerX + spriteWidth * lightScale;
    float worldRangeUpperY = worldRangeLowerY + spriteHeight * lightScale;

    float maskTexCoordX = Map(gl_FragCoord.x, worldRangeLowerX, worldRangeUpperX, 0, 1);
    float maskTexCoordY = Map(screenHeight-gl_FragCoord.y, worldRangeLowerY, worldRangeUpperY, 0, 1);

    vec2 rotatedTex = vec2(maskTexCoordX, maskTexCoordY);    
    if (rotate == 1) 
        rotatedTex = vec2(maskTexCoordY, maskTexCoordX);
    if (rotate == 2) 
        rotatedTex = vec2(-maskTexCoordX, maskTexCoordY);
    if (rotate == 3) 
        rotatedTex = vec2(-maskTexCoordY, -maskTexCoordX);

    vec4 spriteColor = texture(inputTexture, rotatedTex);
    FragColor = spriteColor * vec4(color * strength, 1);
} 