#version 330

layout (location = 0) in vec3 aPos;

uniform vec3 position;
uniform vec3 color;

out vec3 oColor;

void main() {
	gl_Position = vec4(aPos + position, 1);
	oColor = color;
}
