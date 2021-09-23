#version 330

in vec3 oColor;
out vec4 final_color;

void main() {
	final_color = vec4(oColor, 1.0);
}
