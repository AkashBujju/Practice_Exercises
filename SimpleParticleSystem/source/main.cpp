#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>
#include <cmath>
#include <ctime>
#include "shader.h"
#include "particles.h"

void keyboard(GLFWwindow* window, int key, int scancode, int action, int mods);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_cursor(GLFWwindow* window, double x, double y);
void mouse_button(GLFWwindow* window, int button, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void reload_shader();
float map(float src, float src_start, float src_end, float des_start, float des_end);
float to_radians(float degree);
float magnitude(glm::vec2 vec);

Shader* shader;

int main(void) {
	cout << "Started Main...\n\n";

	srand(time(0));

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
	unsigned int window_width = 1440;
	unsigned int window_height = 800;
	const char* window_name = "Fireworks";
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
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  
	glfwSwapInterval(1);
	/* Init OpenGL */

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

	/* Init Others */
	shader = new Shader("..\\shaders\\vertex_shader.glsl", "..\\shaders\\fragment_shader.glsl");
	reload_shader();

	ParticleSystem ps1(shader->program, 1000, 0.009f, glm::vec4(1, 1, 1, 1), glm::vec2(-0.5f, -0.9f));
	ParticleSystem ps2(shader->program, 1000, 0.009f, glm::vec4(1, 1, 0, 1), glm::vec2(0, -0.9f));
	ParticleSystem ps3(shader->program, 1000, 0.009f, glm::vec4(1, 0, 1, 1), glm::vec2(+0.5f, -0.9f));
	/* Init Others */

	float timer = static_cast<float>(glfwGetTime());

	/* Start Loop */
	while (!glfwWindowShouldClose(window)) {
		glClearColor(0, 0, 0, 1.0f);
		float time_sec = static_cast<float>(glfwGetTime());
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if((time_sec - timer) >= 0.1f) {
			timer = time_sec;
			ps1.add_new_particle();
			ps2.add_new_particle();
			ps3.add_new_particle();
		}

		ps1.update();
		ps2.update();
		ps3.update();
		ps1.draw();
		ps2.draw();
		ps3.draw();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	cout << "\nEnded Main..\n";
	return 0;
}

float magnitude(glm::vec2 vec) {
	return sqrt((vec.x * vec.x) + (vec.y * vec.y));
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
