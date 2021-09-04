#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>
#include <cmath>
#include "shader.h"
#include "lines.h"

void keyboard(GLFWwindow* window, int key, int scancode, int action, int mods);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_cursor(GLFWwindow* window, double x, double y);
void mouse_button(GLFWwindow* window, int button, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void reload_shader();
float map(float src, float src_start, float src_end, float des_start, float des_end);
float to_radians(float degree);
glm::vec2 get_second_point(glm::vec2 p1, float len, float degree);
float magnitude(glm::vec2 vec);
void branch(glm::vec2 line_end, float current_length, float current_angle);

Shader* shader;
std::vector<Line> all_lines;

int main(void) {
	cout << "Started Main...\n\n";

	/* Init OpenGL */
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
	glfwWindowHint(GLFW_SAMPLES, 1);
	/* Init OpenGL */

	/* Make Window */
	unsigned int window_width = 800;
	unsigned int window_height = 800;
	const char* window_name = "Mandlebrot Set";
	GLFWwindow* window = glfwCreateWindow(window_width, window_height, window_name, NULL, NULL);
	/* Make Window */

	/* Init OpenGL */
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	glfwSetKeyCallback(window, keyboard);
	glfwSetCursorPosCallback(window, mouse_cursor);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetMouseButtonCallback(window, mouse_button);
	glEnable(GL_DEPTH_TEST);
	/* Init OpenGL */

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	/* Init Others */
	shader = new Shader("..\\shaders\\vertex_shader.glsl", "..\\shaders\\fragment_shader.glsl");
	reload_shader();

	Line line1 (
		glm::vec2(0, -1.0f),
		glm::vec2(0.0f, -0.5f),
		glm::vec3(0, 1, 1)
	);
	all_lines.push_back(line1);

	branch(line1.p2, 0.5f, 45);
	Lines lines(all_lines, shader->program);

	/* Init Others */

	/* Start Loop */
	while (!glfwWindowShouldClose(window)) {
		float time_sec = static_cast<float>(glfwGetTime());
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		lines.draw();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	cout << "\nEnded Main..\n";
	return 0;
}

void branch(glm::vec2 line_end, float current_length, float current_angle) {
	if(current_length > 0.005f) {
		Line l1(
			line_end,
			get_second_point(line_end, current_length * 0.67f, current_angle),
			glm::vec3(1, 0, 1)
		);
		all_lines.push_back(l1);
		branch(l1.p2, current_length * 0.67f, current_angle - 30);

		Line l2(
			line_end,
			get_second_point(line_end, current_length * 0.67f, current_angle + 90),
			glm::vec3(1, 1, 0)
		);
		all_lines.push_back(l2);
		branch(l2.p2, current_length * 0.67f, current_angle + 30);
	}
}

float magnitude(glm::vec2 vec) {
	return sqrt((vec.x * vec.x) + (vec.y * vec.y));
}

glm::vec2 get_second_point(glm::vec2 p1, float len, float degree) {
	float x = p1.x + len * cos(to_radians(degree));
	float y = p1.y + len * sin(to_radians(degree));
	return glm::vec2(x, y);
}

float to_radians(float degree) {
	return degree * 3.14159265358f / 180.0f;
}

float map(float src, float src_start, float src_end, float des_start, float des_end) {
	float t1 = src / (src_start + src_end);
	float ans = (t1 * (des_end - des_start)) + des_start;

	return ans;
}

void keyboard(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS) {
		if (key == GLFW_KEY_ESCAPE)
			glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void mouse_cursor(GLFWwindow* window, double x, double y) {
}

void mouse_button(GLFWwindow* window, int button, int action, int mods) {
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
}

void reload_shader() {
	if (shader->reload_shader()) {
		glClearColor(0, 0, 0, 0);
	}
	else {
		glClearColor(1, 0, 0, 0);
	}
}
