#pragma once

#include "lines.h"
#include "text.h"
#include "circles.h"
#include <map>

struct Node {
	int data;
	int id;
	std::vector<int> connections;
};

struct GraphViz {
	Lines *lines;
	Texts *texts;
	Circles *circles;
	std::vector<int> ids;
	std::map<std::string, bool> connected;

	GLint text_shader;
	GLint normal_shader;
	float radius;

	std::vector<Node> nodes;
	glm::vec3 highlight_color;
	glm::vec3 color_copy;

	GraphViz(GLint text_shader, GLint normal_shader, std::string font_path, int font_size, glm::vec2 window_dims, float circle_radius, glm::vec3 highlight_color);
	void draw();
	void highlight_node(int node_id);
	void unhighlight_node(int node_id);
	void print_all_connections();
	void compute_lines();
	void add_node(int id, int data, std::vector<int> connections, glm::vec3 position, glm::vec3 circle_color, glm::vec3 text_color);
};
