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
#include "shader.h"
#include "text.h"
#include "circles.h"
#include "lines.h"
#include "graph.h"

void keyboard(GLFWwindow* window, int key, int scancode, int action, int mods);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_cursor(GLFWwindow* window, double x, double y);
void mouse_button(GLFWwindow* window, int button, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void reload_shader();
float map(float src, float src_start, float src_end, float des_start, float des_end);
float to_radians(float degree);
float magnitude(glm::vec2 vec);
void BFS(std::vector<Node> nodes, int s);

Shader* shader;
Shader* text_shader;
std::vector<int> bfs_traversal;

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
	glfwWindowHint(GLFW_SAMPLES, 16);
	/* Init OpenGL */

	/* Make Window */
	unsigned int window_width = 1440;
	unsigned int window_height = 800;
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
	/* Init OpenGL */

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

	/* Init Shaders */
	shader = new Shader("..\\shaders\\vertex_shader.glsl", "..\\shaders\\fragment_shader.glsl");
	text_shader = new Shader("..\\shaders\\v_text.shader", "..\\shaders\\f_text.shader");
	shader->reload_shader();
	text_shader->reload_shader();
	/* Init Shaders */

	/* Init Others */
	GraphViz gv(text_shader->program, shader->program, "..\\data\\FiraCode-Regular.ttf", 64, 
					glm::vec2(window_width, window_height), 0.2f, glm::vec3(0.3f, 0.5f, 0.4f));
	gv.add_node(0, 10, std::vector<int>{1, 2, 3}, glm::vec3(+0.0f, +1.4f, 0), glm::vec3(1, 1, 0), glm::vec3(0, 0, 0));
	gv.add_node(1, 20, std::vector<int>{0, 4, 5}, glm::vec3(-0.9f, +0.7f, 0), glm::vec3(1, 0, 0), glm::vec3(0, 0, 0));
	gv.add_node(2, 30, std::vector<int>{0, 6}, glm::vec3(0.0f, +0.7f, 0), glm::vec3(1, 0, 0), glm::vec3(0, 0, 0));
	gv.add_node(3, 40, std::vector<int>{0, 7, 8}, glm::vec3(+0.9f, +0.7f, 0), glm::vec3(1, 0, 0), glm::vec3(0, 0, 0));
	gv.add_node(4, 50, std::vector<int>{1}, glm::vec3(-1.8f, +0.0f, 0), glm::vec3(1, 0, 1), glm::vec3(0, 0, 0));
	gv.add_node(5, 60, std::vector<int>{1}, glm::vec3(-0.9f, +0.0f, 0), glm::vec3(1, 0, 1), glm::vec3(0, 0, 0));
	gv.add_node(6, 70, std::vector<int>{2}, glm::vec3(-0.0f, +0.0f, 0), glm::vec3(1, 0, 1), glm::vec3(0, 0, 0));
	gv.add_node(7, 80, std::vector<int>{3}, glm::vec3(+0.9f, -0.0f, 0), glm::vec3(1, 0, 1), glm::vec3(0, 0, 0));
	gv.add_node(8, 90, std::vector<int>{3}, glm::vec3(+1.8f, -0.0f, 0), glm::vec3(1, 0, 1), glm::vec3(0, 0, 0));
	gv.compute_lines();

	BFS(gv.nodes, 0);
	int bfs_traversal_index = 0;

	glm::mat4 view, projection;
	/* Init Others */

	float timer = static_cast<float>(glfwGetTime());

	/* Start Loop */
	while (!glfwWindowShouldClose(window)) {
		glClearColor(0, 0, 0, 1.0f);
		float time_sec = static_cast<float>(glfwGetTime());
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if((time_sec - timer) >= 0.5f) {
			if(bfs_traversal_index >= bfs_traversal.size())
				bfs_traversal_index = 0;

			gv.unhighlight_node(bfs_traversal_index - 1);
			gv.highlight_node(bfs_traversal_index);

			bfs_traversal_index += 1;
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

		gv.draw();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	cout << "\nEnded Main..\n";
	return 0;
}

void BFS(std::vector<Node> nodes, int s) {
    bool *visited = new bool[nodes.size()];
    for(int i = 0; i < nodes.size(); i++)
        visited[i] = false;
 
    list<int> queue;
    visited[s] = true;
    queue.push_back(s);
 
    list<int>::iterator i;
    while(!queue.empty()) {
        s = queue.front();
		  bfs_traversal.push_back(s);
        queue.pop_front();

		  for(int i = 0; i < nodes[s].connections.size(); ++i) {
            if (!visited[nodes[s].connections[i]]) {
                visited[nodes[s].connections[i]] = true;
                queue.push_back(nodes[s].connections[i]);
				}
		  }
    }
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
