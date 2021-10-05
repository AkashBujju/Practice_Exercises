#version 330

layout (location = 0) in vec2 aPos;

uniform vec2 u_pos;
uniform vec4 u_color;

out vec4 oColor;

void main() {
	gl_Position = vec4(aPos + u_pos, 0, 1);
	oColor = u_color;
}
