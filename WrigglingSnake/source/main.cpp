#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <cmath>
#include <ctime>
#include "shader.h"
#include "bone.h"

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
	const char* window_name = "Wriggling Snake";
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

	/* Bones */
	Mesh mesh;
	add_vertex_to_mesh(&mesh, 0, 0, +0.00f, 0.0f, 0, 0, 0, 1, 0);
	add_vertex_to_mesh(&mesh, 0, 1, +0.20f, 0.0f, 0, 0, 1, 0.5f, 0.5f);
	add_vertex_to_mesh(&mesh, 1, 0, +0.21f, 0.0f, 0, 0, 1, 0.5f, 0.5f);
	add_vertex_to_mesh(&mesh, 1, 1, +0.41f, 0.0f, 0, 1, 2, 0.5f, 0.5f);
	add_vertex_to_mesh(&mesh, 2, 0, +0.42f, 0.0f, 0, 1, 2, 0.5f, 0.5f);
	add_vertex_to_mesh(&mesh, 2, 1, +0.62f, 0.0f, 0, 2, 3, 0.5f, 0.5f);
	add_vertex_to_mesh(&mesh, 3, 0, +0.63, 0.0f, 0, 2, 3, 0.5f, 0.5f);
	add_vertex_to_mesh(&mesh, 3, 1, +0.83f, 0.0f, 0, 3, 4, 0.5f, 0.5f);
	add_vertex_to_mesh(&mesh, 4, 0, +0.84f, 0.0f, 0, 3, 4, 0.5f, 0.5f);
	add_vertex_to_mesh(&mesh, 4, 1, +1.04f, 0.0f, 0, 4, 5, 0.5f, 0.5f);
	add_vertex_to_mesh(&mesh, 5, 0, +1.05f, 0.0f, 0, 4, 5, 0.5f, 0.5f);
	add_vertex_to_mesh(&mesh, 5, 1, +1.25f, 0.0f, 0, 5, 0, 1, 0);
	compile_mesh(&mesh, shader->program, 12);
	/* Bones */

	glm::mat4 view, projection;
	/* Init Others */

	float timer = static_cast<float>(glfwGetTime());

	/* Start Loop */
	while (!glfwWindowShouldClose(window)) {
		glClearColor(0, 0, 0, 1.0f);
		float time_sec = static_cast<float>(glfwGetTime());
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if((time_sec - timer) >= 0.001f) {
			timer = time_sec;
		}

		view = glm::mat4(1.0f);
		projection = glm::mat4(1.0f);
		view  = glm::translate(view, glm::vec3(0.0f, 0.0f, -5.0f));
		projection = glm::perspective(glm::radians(45.0f), 1440.0f / 800.0f, 0.1f, 100.0f);

		glUseProgram(shader->program);
		int view_loc = glGetUniformLocation(shader->program, "view");
		int projection_loc = glGetUniformLocation(shader->program, "projection");
		//printf("view_loc: %d\n", view_loc);
		//printf("projection_loc: %d\n", projection_loc);
		glUniformMatrix4fv(view_loc, 1, false, glm::value_ptr(view));
		glUniformMatrix4fv(projection_loc, 1, false, glm::value_ptr(projection));

		/* Bones */
		const float degree = 90;
		float d = sin(glfwGetTime()) * degree;
		// rotate_mesh(&mesh, 0, 0, 1, d);
		rotate_bone_in_mesh(&mesh, 0, 1, 0, 0, d * 0.75f);
		rotate_bone_in_mesh(&mesh, 1, 0, 0, 1, d * 0.65f);
		rotate_bone_in_mesh(&mesh, 2, 0, 0, 1, d * 0.55f);
		rotate_bone_in_mesh(&mesh, 3, 0, 0, 1, d * 0.45f);
		rotate_bone_in_mesh(&mesh, 4, 0, 0, 1, d * 0.35f);
		rotate_bone_in_mesh(&mesh, 5, 0, 0, 1, d * 0.25f);
		draw_mesh(&mesh);
		/* Bones */

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
	if (shader->reload_shader()) {
		glClearColor(0, 0, 0, 0);
	}
	else {
		glClearColor(1, 0, 0, 0);
	}
}
