#include "lines.h"
#include <cstdlib>

Line::Line(glm::vec2 p1, glm::vec2 p2, glm::vec3 color) {
	this->p1 = p1;
	this->p2 = p2;
	this->color = color;
}

Lines::Lines(std::vector<Line> lines, GLint program) {
	this->program = program;

	unsigned int total_lines = lines.size();

	/* @Note: '2' for p1, '2' for p2, and 6 for 2 'color' */
	unsigned int array_length = lines.size() * 10;
	total_floats = array_length;

	float *vertices = (float*)malloc(sizeof(float) * array_length);
	unsigned int index = 0;
	for(unsigned int i = 0; i < total_lines; ++i) {
		/* Vertex 1 */
		vertices[index++] = lines[i].p1.x;
		vertices[index++] = lines[i].p1.y;
		vertices[index++] = lines[i].color.x;
		vertices[index++] = lines[i].color.y;
		vertices[index++] = lines[i].color.z;

		/* Vertex 2 */
		vertices[index++] = lines[i].p2.x;
		vertices[index++] = lines[i].p2.y;
		vertices[index++] = lines[i].color.x;
		vertices[index++] = lines[i].color.y;
		vertices[index++] = lines[i].color.z;
	}

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * array_length, vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(sizeof(float) * 2));
	glEnableVertexAttribArray(1);
}

void Lines::draw() {
	glUseProgram(program);
	glBindVertexArray(vao);
	glDrawArrays(GL_LINES, 0, total_floats);
}
