#include "graph.h"
#include <iostream>
#include <cmath>

GraphViz::GraphViz(GLint text_shader, GLint normal_shader, std::string font_path, int font_size, glm::vec2 window_dims, float circle_radius, glm::vec3 highlight_color) {
	this->text_shader = text_shader;
	this->normal_shader = normal_shader;
	this->radius = circle_radius;
	this->highlight_color = highlight_color;

	texts = new Texts();
	lines = new Lines(this->normal_shader);
	circles = new Circles(this->normal_shader);
	texts->init(font_path, font_size, window_dims);
}

void GraphViz::add_node(int id, int data, std::vector<int> connections, glm::vec3 position, glm::vec3 circle_color, glm::vec3 text_color) {
	Node node;
	node.data = data;
	node.id = id;
	node.connections = connections;
	nodes.push_back(node);

	for(unsigned int i = 0; i < connections.size(); ++i) {
		std::string str1 = std::to_string(connections[i]) + "" + std::to_string(id);
		std::string str2 = std::to_string(id) + "" + std::to_string(connections[i]);
		connected.insert(std::make_pair<>(str1, false));
		connected.insert(std::make_pair<>(str2, false));
	}

	glm::vec3 text_pos = position;
	text_pos.z += 0.01f;

	texts->add_text(std::to_string(data), text_pos, text_color);
	circles->add(position, circle_color, radius);

	ids.push_back(nodes.size() - 1);
}

void GraphViz::compute_lines() {
	for(unsigned int i = 0; i < nodes.size(); ++i) {
		int from = nodes[i].id;
		for(unsigned int j = 0; j < nodes[i].connections.size(); ++j) {
			int to = nodes[i].connections[j];

			std::string conn_1 = std::to_string(from) + "" + std::to_string(to);
			std::string conn_2 = std::to_string(to) + "" + std::to_string(from);
			if(connected[conn_1] == true || connected[conn_2] == true) {
				continue;
			}

			glm::vec3 from_pos = circles->positions[from];
			glm::vec3 to_pos = circles->positions[to];
			float dist = glm::distance(from_pos, to_pos) / 2.0f;
			glm::vec3 vec_1 = to_pos - from_pos;
			glm::vec3 vec_2 = glm::vec3(1, 0, 0);
			glm::vec3 direction = glm::normalize(vec_1);
			glm::vec3 new_pos = from_pos + (direction * dist);

			float angle = acos(glm::dot(vec_1, vec_2) / (glm::length(vec_1) * glm::length(vec_2)));
			angle = glm::degrees(angle);

			glm::vec3 tmp = to_pos - from_pos;
			if(tmp.x < 0 && tmp.y < 0) // Quadrant 3
				angle = 360 - angle;
			else if (tmp.x > 0 && tmp.y < 0) // Quadrant 4
				angle = 360 - angle;

			lines->add(new_pos, glm::vec3(1, 1, 1), dist * 1.5f, angle);
			connected[conn_1] = true;
			connected[conn_2] = true;
		}
	}
}

void GraphViz::highlight_node(int node_id) {
	if(node_id < 0 || node_id >= nodes.size())
		return;

	color_copy = circles->colors[node_id];
	circles->colors[node_id] = highlight_color;	
}

void GraphViz::unhighlight_node(int node_id) {
	if(node_id < 0 || node_id >= nodes.size())
		return;

	circles->colors[node_id] = color_copy;
}

void GraphViz::print_all_connections() {
	for(unsigned int i = 0; i < nodes.size(); ++i) {
		std::cout << i << ": ";
		for(unsigned int j = 0; j < nodes[i].connections.size(); ++j) {
			std::cout << nodes[i].connections[j] << " ";
		}
		std::cout << std::endl;
	}
}

void GraphViz::draw() {
	lines->draw();
	circles->draw();
	texts->draw(text_shader);
}
