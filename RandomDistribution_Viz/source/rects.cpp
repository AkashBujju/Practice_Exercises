#include "rects.h"
#define GLM_FORCE_RADIANS
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

extern float map(float src, float src_start, float src_end, float des_start, float des_end);
extern float to_radians(float degree);

Rects::Rects(GLint program, glm::vec2 scale) {
	this->program = program;
	this->scale = scale;
}

void Rects::add(glm::vec3 position, glm::vec2 dims, glm::vec3 color) {
	unsigned int current_index = positions.size();

	float vertices[] = {
		-dims.x / 2, +dims.y / 2.0f, 0,
		+dims.x / 2, +dims.y / 2.0f, 0,
		+dims.x / 2, -dims.y / 2.0f, 0,
		-dims.x / 2, +dims.y / 2.0f, 0,
		-dims.x / 2, -dims.y / 2.0f, 0,
		+dims.x / 2, -dims.y / 2.0f, 0
	};

	positions.push_back(position);
	colors.push_back(color);
	vaos.push_back(0);

	glGenVertexArrays(1, &vaos[current_index]);
	glGenBuffers(1, &vbo);
	glBindVertexArray(vaos[current_index]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 18, vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	free(vertices);
}

void Rects::draw() {
	for(unsigned int i = 0; i < positions.size(); ++i) {
		glUseProgram(program);

		int position_loc = glGetUniformLocation(program, "u_pos");
		glUniform3fv(position_loc, 1, glm::value_ptr(positions[i]));

		int color_loc = glGetUniformLocation(program, "u_color");
		glUniform3fv(color_loc, 1, glm::value_ptr(colors[i]));

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, positions[i]);
		model = glm::scale(model, glm::vec3(scale.x, scale.y, 1));
		int model_loc = glGetUniformLocation(program, "u_model");
		glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model));

		glBindVertexArray(vaos[i]);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
}
