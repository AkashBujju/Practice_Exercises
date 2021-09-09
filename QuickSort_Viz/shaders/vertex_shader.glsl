#version 330

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec3 aColor;

uniform float x_pos;
out vec3 oColor;

void main() {
	gl_Position = vec4(aPos.x + x_pos, aPos.y, 0, 1);
	oColor = aColor;
}
