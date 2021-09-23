#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <cmath>
#include <ctime>
#include <list>
#include <random>
#include <map>
#include <chrono>
#include <easy/profiler.h>
#include "shader.h"
#include "rects.h"
#include "plot.h"

void keyboard(GLFWwindow* window, int key, int scancode, int action, int mods);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_cursor(GLFWwindow* window, double x, double y);
void mouse_button(GLFWwindow* window, int button, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void reload_shader();
float map(float src, float src_start, float src_end, float des_start, float des_end);
float to_radians(float degree);
float magnitude(glm::vec2 vec);
void test_random_number_distributions();

Shader* shader;
Shader* text_shader;

int main(void) {
	cout << "Started Main...\n\n";

	srand(time(0));
	EASY_PROFILER_ENABLE;

	/* Init OpenGL */
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
	glfwWindowHint(GLFW_SAMPLES, 16);
	/* Init OpenGL */

	/* Make Window */
	unsigned int window_width = 1440;
	unsigned int window_height = 900;
	const char* window_name = "BreadthFirstTraversl_Viz";
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
	glEnable(GL_MULTISAMPLE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  
	glfwSwapInterval(1);
	glfwMaximizeWindow(window);
	/* Init OpenGL */

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

	/* Init Shaders */
	shader = new Shader("..\\shaders\\vertex_shader.glsl", "..\\shaders\\fragment_shader.glsl");
	text_shader = new Shader("..\\shaders\\v_text.shader", "..\\shaders\\f_text.shader");
	shader->reload_shader();
	text_shader->reload_shader();
	/* Init Shaders */

	/* Init Others */
	glm::mat4 view, projection;

	Plot u_plot("uniform", shader->program, glm::vec2(1, 1), glm::vec2(-3.5f, +0.5f), glm::vec2(window_width, window_height));
	Plot n_plot("normal", shader->program, glm::vec2(1, 1), glm::vec2(-3.5f, -1.5f), glm::vec2(window_width, window_height));
	Plot b_plot("bernoulli", shader->program, glm::vec2(1, 1), glm::vec2(+1.5f, +0.5f), glm::vec2(window_width, window_height));
	Plot e_plot("exponential", shader->program, glm::vec2(1, 1), glm::vec2(+1.5f, -1.5f), glm::vec2(window_width, window_height));

	test_random_number_distributions();
	/* Init Others */

	float timer = static_cast<float>(glfwGetTime());

	/* Start Loop */
	while (!glfwWindowShouldClose(window)) {
		glClearColor(0, 0, 0, 1.0f);
		float time_sec = static_cast<float>(glfwGetTime());
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if((time_sec - timer) >= 0.5f) {
			timer = time_sec;
		}

		view = glm::mat4(1.0f);
		projection = glm::mat4(1.0f);
		view  = glm::translate(view, glm::vec3(0.0f, 0.0f, -6.0f));
		projection = glm::perspective(glm::radians(45.0f), 1440.0f / 800.0f, 0.1f, 100.0f);

		glUseProgram(shader->program);
		int view_loc = glGetUniformLocation(shader->program, "u_view");
		int projection_loc = glGetUniformLocation(shader->program, "u_projection");
		glUniformMatrix4fv(view_loc, 1, false, glm::value_ptr(view));
		glUniformMatrix4fv(projection_loc, 1, false, glm::value_ptr(projection));

		glUseProgram(text_shader->program);
		int text_projection_loc = glGetUniformLocation(text_shader->program, "projection");
		int text_view_loc = glGetUniformLocation(text_shader->program, "view");
		glUniformMatrix4fv(text_projection_loc, 1, false, glm::value_ptr(projection));
		glUniformMatrix4fv(text_view_loc, 1, false, glm::value_ptr(view));

		u_plot.draw(text_shader->program);
		n_plot.draw(text_shader->program);
		b_plot.draw(text_shader->program);
		e_plot.draw(text_shader->program);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	profiler::dumpBlocksToFile("test_profile.prof");

	cout << "\nEnded Main..\n";
	return 0;
}

void test_random_number_distributions() {
	/*
	EASY_FUNCTION(profiler::colors::Magenta);
	
	std::cout << "EXPONENTIAL DISTRIBUTION" << std::endl;
	std::exponential_distribution<> exponential_dist(1);
	std::map<int, int> info_1;
	for(unsigned int i = 0; i < 100; ++i) {
		int result = exponential_dist(rand_eng);	
		info_1[result] += 1;
	}
	for(auto p: info_1) {
		std::cout << std::string(p.second, '*') << std::endl;
	}
	*/
}

float magnitude(glm::vec2 vec) {
	return sqrt((vec.x * vec.x) + (vec.y * vec.y));
}

float to_radians(float degree) {
	return degree * 3.14159265358f / 180.0f;
}

float map(float src, float src_start, float src_end, float des_start, float des_end) {
	float t1 = src / (src_end - src_start);
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
}
