#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>
#include "shader.h"
#include "points.h"

void keyboard(GLFWwindow* window, int key, int scancode, int action, int mods);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_cursor(GLFWwindow* window, double x, double y);
void mouse_button(GLFWwindow* window, int button, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void reload_shader();
float map(float src, float src_start, float src_end, float des_start, float des_end);

glm::vec3 pos(0.0f, 0.0f, 0.0f);
Shader* shader;

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

	std::vector<glm::vec2> positions;
	std::vector<glm::vec3> colors;

	unsigned int max_iterations = 100;
	for(float i = -2.0f; i < +2.0f; i += 0.001f) {
		for(float j = -2.0f; j < +2.0f; j += 0.001f) {
			float a = i;
			float b = j;
			float ca = a;
			float cb = b;

			unsigned int current_iteration = 0;
			for(current_iteration = 0; current_iteration < max_iterations; ++current_iteration) {
				float aa = (a * a) - (b * b);
				float bb = 2 * a * b;
				a = aa + ca;
				b = bb + cb;

				if((a + b) > 1)
					break;
			}

			float color = sqrt(map(current_iteration, 0, max_iterations, 0, 1));
			float red = color;
			float blue = color;
			float green = color;

			if(current_iteration == max_iterations) {
				red = blue = green = 0;
			}

			positions.push_back(glm::vec2(i + 0.5f, j));
			colors.push_back(glm::vec3(red, 0, 0));
		}
	}

	Points points(positions, colors, shader->program);
	/* Init Others */

	/* Start Loop */
	while (!glfwWindowShouldClose(window)) {
		float time_sec = static_cast<float>(glfwGetTime());
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		points.draw();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	cout << "\nEnded Main..\n";
	return 0;
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
