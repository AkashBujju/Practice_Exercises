#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>
#include <cmath>
#include <ctime>
#include "shader.h"
#include "rects.h"

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
std::vector<glm::vec2> swap_indices;

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
	unsigned int window_width = 800;
	unsigned int window_height = 800;
	const char* window_name = "QuickSort_Viz";
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
	glfwSwapInterval(1);
	/* Init OpenGL */

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	/* Init Others */
	shader = new Shader("..\\shaders\\vertex_shader.glsl", "..\\shaders\\fragment_shader.glsl");
	reload_shader();

	std::vector<glm::vec3> colors;
	std::vector<float> numbers;

	for(unsigned int i = 0; i < 100; ++i) {
		float r = rand() % 255;
		float g = rand() % 255;
		float b = rand() % 255;
		r = map(r, 0, 255, 0, 1);
		g = map(g, 0, 255, 0, 1);
		b = map(b, 0, 255, 0, 1);
		colors.push_back(glm::vec3(r, g, b));

		float number = rand() % 100;
		numbers.push_back(number);
	}
	
	std::vector<float> numbers_copy(numbers);
	quick_sort(numbers, 0, numbers.size() - 1);

	Rects lines;
	int current_index = 0;
	lines.init(shader->program, colors, numbers_copy);
	/* Init Others */

	float timer = static_cast<float>(glfwGetTime());

	/* Start Loop */
	while (!glfwWindowShouldClose(window)) {
		float time_sec = static_cast<float>(glfwGetTime());
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if((time_sec - timer) >= 0.2f) {
			timer = time_sec;
			if(current_index < swap_indices.size())
				lines.swap_positions(swap_indices[current_index].x, swap_indices[current_index].y);
			current_index += 1;
		}

		lines.draw();

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

	swap_indices.push_back(glm::vec2(index_0, index_1));
}

int partition (std::vector<float> &arr, int low, int high) {
	float pivot = arr[high];
	int i = (low - 1);

	for (int j = low; j <= high- 1; j++) {
		if (arr[j] <= pivot) {
			i++;
			swap(arr, i, j);
		}
	}

	swap(arr, i + 1, high);
	return (i + 1);
}

void quick_sort(std::vector<float> &arr, int low, int high) {
	if (low < high) {
		int pi = partition(arr, low, high);
		quick_sort(arr, low, pi - 1);
		quick_sort(arr, pi + 1, high);
	}
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
