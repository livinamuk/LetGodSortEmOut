#version 400
out vec4 fragColor;
in vec2 texCoord;

uniform sampler2D mainTexture;
uniform sampler2D backgroundTexture;

uniform vec3 color;

void main() 
{
	// The background surface buffer is flipped vertically
	//vec4 background = texture2D(backgroundTexture, vec2(texCoord.x, 1.0 - texCoord.y));
//	fragColor = vec4(texCoord, 1.0, 1.0) * texture2D(mainTexture, texCoord) * background;

	fragColor = vec4(color, 1);
}