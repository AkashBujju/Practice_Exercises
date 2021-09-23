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
	unsigned int window_width = 1920;
	unsigned int window_height = 1080;
	const char* window_name = "Some Particles - Akash S";
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
	glfwMaximizeWindow(window);
	/* Init OpenGL */

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

	/* Init Others */
	shader = new Shader("..\\shaders\\vertex_shader.glsl", "..\\shaders\\fragment_shader.glsl");
	reload_shader();

	float radius = 0.030f;

	ParticleSystem ps1(shader->program, 500, radius, glm::vec4(1, 1, 1, 1), glm::vec2(-1.1f, +0.9f), 3);
	ParticleSystem ps2(shader->program, 500, radius, glm::vec4(1, 1, 1, 1), glm::vec2(-1.1f, +0.6f), 3);
	ParticleSystem ps3(shader->program, 500, radius, glm::vec4(1, 1, 1, 1), glm::vec2(-1.1f, +0.3f), 3);
	ParticleSystem ps4(shader->program, 500, radius, glm::vec4(1, 1, 1, 1), glm::vec2(-1.1f, +0.0f), 3);
	ParticleSystem ps5(shader->program, 500, radius, glm::vec4(1, 1, 1, 1), glm::vec2(-1.1f, -0.3f), 3);
	ParticleSystem ps6(shader->program, 500, radius, glm::vec4(1, 1, 1, 1), glm::vec2(-1.1f, -0.6f), 3);

	ParticleSystem ps7(shader->program, 500, radius, glm::vec4(1, 1, 1, 1), glm::vec2(+1.1f, +0.6f), 4);
	ParticleSystem ps8(shader->program, 500, radius, glm::vec4(1, 1, 1, 1), glm::vec2(+1.1f, +0.3f), 4);
	ParticleSystem ps9(shader->program, 500, radius, glm::vec4(1, 1, 1, 1), glm::vec2(+1.1f, +0.0f), 4);
	ParticleSystem ps10(shader->program, 500, radius, glm::vec4(1, 1, 1, 1), glm::vec2(+1.1f, -0.3f), 4);
	ParticleSystem ps11(shader->program, 500, radius, glm::vec4(1, 1, 1, 1), glm::vec2(+1.1f, -0.6f), 4);
	ParticleSystem ps12(shader->program, 500, radius, glm::vec4(1, 1, 1, 1), glm::vec2(+1.1f, -0.9f), 4);

	ParticleSystem ps13(shader->program, 500, radius, glm::vec4(1, 1, 1, 1), glm::vec2(-0.9f, -1.1f), 1);
	ParticleSystem ps14(shader->program, 500, radius, glm::vec4(1, 1, 1, 1), glm::vec2(-0.6f, -1.1f), 1);
	ParticleSystem ps15(shader->program, 500, radius, glm::vec4(1, 1, 1, 1), glm::vec2(-0.3f, -1.1f), 1);
	ParticleSystem ps16(shader->program, 500, radius, glm::vec4(1, 1, 1, 1), glm::vec2(-0.0f, -1.1f), 1);
	ParticleSystem ps17(shader->program, 500, radius, glm::vec4(1, 1, 1, 1), glm::vec2(+0.3f, -1.1f), 1);
	ParticleSystem ps18(shader->program, 500, radius, glm::vec4(1, 1, 1, 1), glm::vec2(+0.6f, -1.1f), 1);
	ParticleSystem ps19(shader->program, 500, radius, glm::vec4(1, 1, 1, 1), glm::vec2(+0.9f, -1.1f), 1);

	ParticleSystem ps20(shader->program, 500, radius, glm::vec4(1, 1, 1, 1), glm::vec2(-0.9f, +1.1f), 2);
	ParticleSystem ps21(shader->program, 500, radius, glm::vec4(1, 1, 1, 1), glm::vec2(-0.6f, +1.1f), 2);
	ParticleSystem ps22(shader->program, 500, radius, glm::vec4(1, 1, 1, 1), glm::vec2(-0.3f, +1.1f), 2);
	ParticleSystem ps23(shader->program, 500, radius, glm::vec4(1, 1, 1, 1), glm::vec2(-0.0f, +1.1f), 2);
	ParticleSystem ps24(shader->program, 500, radius, glm::vec4(1, 1, 1, 1), glm::vec2(+0.3f, +1.1f), 2);
	ParticleSystem ps25(shader->program, 500, radius, glm::vec4(1, 1, 1, 1), glm::vec2(+0.6f, +1.1f), 2);
	ParticleSystem ps26(shader->program, 500, radius, glm::vec4(1, 1, 1, 1), glm::vec2(+0.9f, +1.1f), 2);
	/* Init Others */

	float timer = static_cast<float>(glfwGetTime());
	float start_timer = static_cast<float>(glfwGetTime());
	bool should_start = false;

	/* Start Loop */
	while (!glfwWindowShouldClose(window)) {
		glClearColor(0, 0, 0, 1.0f);
		float time_sec = static_cast<float>(glfwGetTime());
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if((time_sec - start_timer) >= 3.0f) {
			should_start = true;
		}

		if((time_sec - timer) >= 0.2f && should_start) {
			timer = time_sec;
			ps1.add_new_particle();
			ps2.add_new_particle();
			ps3.add_new_particle();
			ps4.add_new_particle();
			ps5.add_new_particle();
			ps6.add_new_particle();
			ps7.add_new_particle();
			ps8.add_new_particle();
			ps9.add_new_particle();
			ps10.add_new_particle();
			ps11.add_new_particle();
			ps12.add_new_particle();
			ps13.add_new_particle();
			ps14.add_new_particle();
			ps15.add_new_particle();
			ps16.add_new_particle();
			ps17.add_new_particle();
			ps18.add_new_particle();
			ps19.add_new_particle();
			ps20.add_new_particle();
			ps21.add_new_particle();
			ps22.add_new_particle();
			ps23.add_new_particle();
			ps24.add_new_particle();
			ps25.add_new_particle();
			ps26.add_new_particle();
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
		ps8.update();
		ps8.draw();
		ps9.update();
		ps9.draw();
		ps10.update();
		ps10.draw();
		ps11.update();
		ps11.draw();
		ps12.update();
		ps12.draw();
		ps13.update();
		ps13.draw();
		ps14.update();
		ps14.draw();
		ps15.update();
		ps15.draw();
		ps16.update();
		ps16.draw();
		ps17.update();
		ps17.draw();
		ps18.update();
		ps18.draw();
		ps19.update();
		ps19.draw();
		ps20.update();
		ps20.draw();
		ps21.update();
		ps21.draw();
		ps22.update();
		ps22.draw();
		ps23.update();
		ps23.draw();
		ps24.update();
		ps24.draw();
		ps25.update();
		ps25.draw();
		ps26.update();
		ps26.draw();

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
