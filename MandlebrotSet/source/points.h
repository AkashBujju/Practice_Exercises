#pragma once

#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>

class Points {
	private:
		GLuint vao;
		GLuint vbo;
		GLint program;
		GLuint total_floats;
	public:
		Points(std::vector<glm::vec2> points, std::vector<glm::vec3> colors, GLint program);
		void draw();
};
