#version 330

layout (location = 0) in vec2 aPos;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

uniform vec3 inColor;
out vec3 oColor;

void main() {
	gl_Position = u_projection * u_view * u_model * vec4(aPos, 1, 1);
	oColor = inColor;
}
