#version 330

layout (location = 0) in vec3 aPos;

uniform vec3 u_pos;
uniform vec3 u_color;
uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

out vec3 oColor;

void main() {
	gl_Position = u_projection * u_view * u_model * vec4(aPos, 1);
	oColor = u_color;
}
