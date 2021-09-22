#include "circles.h"
#define GLM_FORCE_RADIANS
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

extern float map(float src, float src_start, float src_end, float des_start, float des_end);
extern float to_radians(float degree);

Circles::Circles(GLint program) {
	this->program = program;
}

void Circles::add(glm::vec3 position, glm::vec3 color, float radius) {
	unsigned int current_index = positions.size();
	unsigned int num_iterations = 100;
	float step_angle = 360.0f / num_iterations;
	float num_floats = (1 + num_iterations) * 3 + 3;
	this->num_vertices = num_iterations + 2;

	float *vertices = (float*)malloc(sizeof(float) * num_floats);
	vertices[0] = 0; vertices[1] = 0; vertices[2] = 0;
	vertices[3] = radius; vertices[4] = 0; vertices[5] = 0;

	unsigned int index = 6;
	float current_angle = step_angle;
	for(unsigned int i = 0; i < num_iterations; ++i) {
		float x = radius * cos(to_radians(current_angle));
		float y = radius * sin(to_radians(current_angle));
		vertices[index++] = x;
		vertices[index++] = y;
		vertices[index++] = 0;
		current_angle += step_angle;
	}

	positions.push_back(position);
	colors.push_back(color);
	radii.push_back(radius);
	vaos.push_back(0);

	glGenVertexArrays(1, &vaos[current_index]);
	glGenBuffers(1, &vbo);
	glBindVertexArray(vaos[current_index]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * num_floats, vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	free(vertices);
}

void Circles::draw() {
	for(unsigned int i = 0; i < positions.size(); ++i) {
		glUseProgram(program);

		int position_loc = glGetUniformLocation(program, "u_pos");
		glUniform3fv(position_loc, 1, glm::value_ptr(positions[i]));

		int color_loc = glGetUniformLocation(program, "u_color");
		glUniform3fv(color_loc, 1, glm::value_ptr(colors[i]));

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, positions[i]);
		int model_loc = glGetUniformLocation(program, "u_model");
		glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model));

		glBindVertexArray(vaos[i]);
		glDrawArrays(GL_TRIANGLE_FAN, 0, num_vertices);
	}
}
