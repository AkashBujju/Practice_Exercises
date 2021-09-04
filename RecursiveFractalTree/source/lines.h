#pragma once

#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>

struct Line {
	glm::vec2 p1;
	glm::vec2 p2;
	glm::vec3 color;

	Line(glm::vec2 p1, glm::vec2 p2, glm::vec3 color);
};

class Lines {
	private:
		GLuint vao;
		GLuint vbo;
		GLint program;
		GLuint total_floats;
	public:
		Lines(std::vector<Line> lines, GLint program);
		void draw();
};
