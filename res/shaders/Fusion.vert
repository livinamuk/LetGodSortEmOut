#version 400

layout (location = 0) in vec3 vp;
out vec2 texCoord;

void main() {
	texCoord = vec2(0.5) + 0.5 * vp.xy;
	gl_Position = vec4(vp, 1.0);
}