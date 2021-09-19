#pragma once

#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>

struct Rects {
	GLint program;
	GLuint vbo;
	unsigned int num_rects;
	glm::vec2 dims;
	glm::vec2 initial_position;
	glm::vec4 initial_color;
	std::vector<GLuint> vaos;
	std::vector<glm::vec2> positions;
	std::vector<float> angles;
	std::vector<glm::mat4> models;

	Rects(GLint program, unsigned int num_rects, glm::vec2 dims, glm::vec4 initial_color, glm::vec2 initial_position);
	void move(std::vector<float> velocities);
	void change_angle(std::vector<float> angles);
	void draw();
};

struct ObstaclesAndTarget {
	GLint program;
	GLuint vbo;
	std::vector<GLuint> vaos;
	std::vector<glm::mat4> models;
	std::vector<glm::vec2> positions;
	std::vector<glm::vec4> colors;
	std::vector<glm::vec2> dims;

	ObstaclesAndTarget(GLint program, glm::vec2 target_position, glm::vec4 target_color, glm::vec2 target_dims);
	void draw();
	void add_obstacle(glm::vec2 position, glm::vec2 dims, glm::vec4 color);
};
