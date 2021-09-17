#include "rects.h"
#include <cstdlib>
#define GLM_FORCE_RADIANS
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

extern float map(float src, float src_start, float src_end, float des_start, float des_end);
extern float to_radians(float degree);

Rects::Rects(GLint program, unsigned int num_rects, glm::vec2 dims, glm::vec4 initial_color, glm::vec2 initial_position) {
	this->program = program;
	this->initial_position = initial_position;
	this->num_rects = num_rects;
	this->initial_color = initial_color;

	positions.reserve(num_rects);
	angles.reserve(num_rects);
	vaos.reserve(num_rects);
	models.reserve(num_rects);

	float w = dims.x;
	float h = dims.y;
	float vertices[] = {
		-w / 2, +h / 2,
		+w / 2, +h / 2,
		+w / 2, -h / 2,
		-w / 2, +h / 2,
		-w / 2, -h / 2,
		+w / 2, -h / 2 
	};

	for(unsigned int i = 0; i < num_rects; ++i) {
		positions.push_back(initial_position);
		angles.push_back(0);
		vaos.push_back(0);

		glGenVertexArrays(1, &vaos[i]);
		glGenBuffers(1, &vbo);
		glBindVertexArray(vaos[i]);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 12, vertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
	}
}

ObstaclesAndTarget::ObstaclesAndTarget(GLint program, glm::vec2 target_position, glm::vec4 target_color, glm::vec2 target_dims) {
	this->program = program;

	positions.reserve(10);
	vaos.reserve(10);
	positions.reserve(10);
	models.reserve(10);
	dims.reserve(10);

	positions.push_back(target_position);
	colors.push_back(target_color);
	dims.push_back(target_dims);
	vaos.push_back(0);
	models.push_back(glm::mat4(1.0f));

	float w = target_dims.x;
	float h = target_dims.y;
	float vertices[] = {
		-w / 2, +h / 2,
		+w / 2, +h / 2,
		+w / 2, -h / 2,
		-w / 2, +h / 2,
		-w / 2, -h / 2,
		+w / 2, -h / 2 
	};
	glGenVertexArrays(1, &vaos[0]);
	glGenBuffers(1, &vbo);
	glBindVertexArray(vaos[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 12, vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}

void ObstaclesAndTarget::add_obstacle(glm::vec2 position, glm::vec2 dims, glm::vec4 color) {
	unsigned int index = positions.size();
	this->positions.push_back(position);
	this->colors.push_back(color);
	this->dims.push_back(dims);
	this->vaos.push_back(0);
	this->models.push_back(glm::mat4(1.0f));

	float w = dims.x;
	float h = dims.y;

	float vertices[] = {
		-w / 2, +h / 2,
		+w / 2, +h / 2,
		+w / 2, -h / 2,
		-w / 2, +h / 2,
		-w / 2, -h / 2,
		+w / 2, -h / 2 
	};

	glGenVertexArrays(1, &vaos[index]);
	glGenBuffers(1, &vbo);
	glBindVertexArray(vaos[index]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 12, vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}

void ObstaclesAndTarget::draw() {
	for(unsigned int i = 0; i < positions.size(); ++i) {
		glUseProgram(program);

		int position_loc = glGetUniformLocation(program, "u_pos");
		glUniform2fv(position_loc, 1, glm::value_ptr(positions[i]));

		int color_loc = glGetUniformLocation(program, "u_color");
		glUniform4fv(color_loc, 1, glm::value_ptr(colors[i]));

		int model_loc = glGetUniformLocation(program, "u_model");
		models[i] = glm::mat4(1.0f);

		/* Translate */
		models[i] = glm::translate(models[i], glm::vec3(positions[i].x, positions[i].y, 0));
		/* Translate */

		glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(models[i]));
		glBindVertexArray(vaos[i]);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
}

void Rects::change_angle(std::vector<float> angles) {
	for(unsigned int i = 0; i < num_rects; ++i) {
		this->angles[i] = angles[i];
	}
}

void Rects::move(std::vector<float> velocities) {
	for(unsigned int i = 0; i < num_rects; ++i) {
		float new_x = positions[i].x + (velocities[i] * cos(glm::radians(angles[i] + 90)));
		float new_y = positions[i].y + (velocities[i] * sin(glm::radians(angles[i] + 90)));
		positions[i] = glm::vec2(new_x, new_y);
	}
}

void Rects::draw() {
	for(unsigned int i = 0; i < num_rects; ++i) {
		glUseProgram(program);

		int position_loc = glGetUniformLocation(program, "u_pos");
		glUniform2fv(position_loc, 1, glm::value_ptr(positions[i]));

		int color_loc = glGetUniformLocation(program, "u_color");
		glUniform4fv(color_loc, 1, glm::value_ptr(initial_color));

		int model_loc = glGetUniformLocation(program, "u_model");
		models[i] = glm::mat4(1.0f);

		/* Translate */
		models[i] = glm::translate(models[i], glm::vec3(positions[i].x, positions[i].y, 0));
		/* Translate */

		/* Rotate */
		models[i] = glm::rotate(models[i], glm::radians(angles[i]), glm::vec3(0, 0, 1));
		/* Rotate */

		glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(models[i]));
		glBindVertexArray(vaos[i]);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
}
