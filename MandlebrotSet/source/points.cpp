#include "points.h"
#include <cstdlib>

Points::Points(std::vector<glm::vec2> points, std::vector<glm::vec3> colors, GLint program) {
	this->program = program;

	unsigned int array_size = (points.size() * 2) + (colors.size() * 3);
	unsigned int total_points = points.size();
	total_floats = array_size;

	float *vertices = (float*)malloc(sizeof(float) * array_size);
	unsigned int index = 0;
	for(unsigned int i = 0; i < total_points; ++i) {
		vertices[index++] = points[i].x;	
		vertices[index++] = points[i].y;
		vertices[index++] = colors[i].x;
		vertices[index++] = colors[i].y;
		vertices[index++] = colors[i].z;
	}

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * array_size, vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(sizeof(float) * 2));
	glEnableVertexAttribArray(1);
}

void Points::draw() {
	glUseProgram(program);

	glBindVertexArray(vao);
	glDrawArrays(GL_POINTS, 0, total_floats);
}
