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
void quick_sort(std::vector<float> &arr, int low, int high);
int partition (std::vector<float> &arr, int low, int high);
void swap(std::vector<float> &arr, int index_0, int index_1);

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
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glfwSwapInterval(1);
	/* Init OpenGL */

	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

	/* Init Others */
	shader = new Shader("..\\shaders\\vertex_shader.glsl", "..\\shaders\\fragment_shader.glsl");
	reload_shader();

	ParticleSystem ps1(shader->program, 300, glm::vec3(1, 1, 1), glm::vec2(0, -0.80f));
	ParticleSystem ps2(shader->program, 300, glm::vec3(1, 1, 0), glm::vec2(-0.5, -0.95f));
	ParticleSystem ps3(shader->program, 300, glm::vec3(0, 1, 0), glm::vec2(+0.5, -0.95f));
	ParticleSystem ps4(shader->program, 300, glm::vec3(0, 1, 1), glm::vec2(+0.3, -0.90f));
	ParticleSystem ps5(shader->program, 300, glm::vec3(1, 0, 1), glm::vec2(-0.3, -0.90f));
	ParticleSystem ps6(shader->program, 300, glm::vec3(0, 0, 1), glm::vec2(-0.7, -0.7f));
	ParticleSystem ps7(shader->program, 300, glm::vec3(1, 0.5f, 0.3f), glm::vec2(+0.7, -0.7f));
	/* Init Others */

	float timer = static_cast<float>(glfwGetTime());

	/* Start Loop */
	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.1f, 0.1f, 0.1f, 0.5f);
		float time_sec = static_cast<float>(glfwGetTime());
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if((time_sec - timer) >= 0.2f) {
			timer = time_sec;
		}

		ps1.update();
		ps1.draw();
		ps2.update();
		ps2.draw();
		ps3.update();
		ps3.draw();
		ps4.update();
		ps4.draw();
		ps5.update();
		ps5.draw();
		ps6.update();
		ps6.draw();
		ps7.update();
		ps7.draw();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	cout << "\nEnded Main..\n";
	return 0;
}

void swap(std::vector<float> &arr, int index_0, int index_1) {
	float tmp = arr[index_0];
	arr[index_0] = arr[index_1];
	arr[index_1] = tmp;
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
