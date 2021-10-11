#include "rects.h"
#define GLM_FORCE_RADIANS
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <map>
#include <string>

extern float map(float src, float src_start, float src_end, float des_start, float des_end);
extern float to_radians(float degree);

WaveMotion::WaveMotion(GLint program, glm::vec3 scale) {
	this->program = program;
	this->num_rows = 10;
	this->num_cols = 16;

	int total_cubes = num_rows * num_cols;
	positions.reserve(total_cubes);
	scales.reserve(total_cubes);

	for(unsigned int i = 0; i < total_cubes; ++i) {
		scales.push_back(scale);
		scales_t.push_back(glm::vec3(0, 0, 0));
	}

	int num_floats = 36 * 6;
	this->num_vertices = 36;
	float vertices[] = {
		-0.5f, -0.5f, -0.5f, 0.0f, 0.9f, 0.0f,
		+0.5f, -0.5f, -0.5f, 0.0f, 0.9f, 0.0f,
		+0.5f, +0.5f, -0.5f, 0.0f, 0.9f, 0.0f,
		+0.5f, +0.5f, -0.5f, 0.0f, 0.9f, 0.0f,
		-0.5f, +0.5f, -0.5f, 0.0f, 0.9f, 0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 0.9f, 0.0f,

		-0.5f, -0.5f, +0.5f, 0.0f, 0.8f, 0.0f, 
		+0.5f, -0.5f, +0.5f, 0.0f, 0.8f, 0.0f,
		+0.5f, +0.5f, +0.5f, 0.0f, 0.8f, 0.0f,
		+0.5f, +0.5f, +0.5f, 0.0f, 0.8f, 0.0f,
		-0.5f, +0.5f, +0.5f, 0.0f, 0.8f, 0.0f,
		-0.5f, -0.5f, +0.5f, 0.0f, 0.8f, 0.0f,

		-0.5f, +0.5f,  0.5f, 0.0f, 0.70f, 0.0f, 
		-0.5f, +0.5f, -0.5f, 0.0f, 0.70f, 0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 0.70f, 0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 0.70f, 0.0f,
		-0.5f, -0.5f, +0.5f, 0.0f, 0.70f, 0.0f,
		-0.5f, +0.5f, +0.5f, 0.0f, 0.70f, 0.0f,

		+0.5f, +0.5f, +0.5f, 0.0f, 0.60f, 0.0f, 
		+0.5f, +0.5f, -0.5f, 0.0f, 0.60f, 0.0f,
		+0.5f, -0.5f, -0.5f, 0.0f, 0.60f, 0.0f,
		+0.5f, -0.5f, -0.5f, 0.0f, 0.60f, 0.0f,
		+0.5f, -0.5f, +0.5f, 0.0f, 0.60f, 0.0f,
		+0.5f, +0.5f, +0.5f, 0.0f, 0.60f, 0.0f,

		-0.5f, -0.5f, -0.5f, 1.0f, 0.5f, 0.0f, 
		+0.5f, -0.5f, -0.5f, 0.0f, 0.5f, 0.0f,
		+0.5f, -0.5f, +0.5f, 0.0f, 0.5f, 0.0f,
		+0.5f, -0.5f, +0.5f, 0.0f, 0.5f, 0.0f,
		-0.5f, -0.5f, +0.5f, 0.0f, 0.5f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 0.5f, 0.0f,

		-0.5f, +0.5f, -0.5f, 1.0f, 0.5f, 0.0f, 
		+0.5f, +0.5f, -0.5f, 0.0f, 0.5f, 0.0f,
		+0.5f, +0.5f, +0.5f, 0.0f, 0.5f, 0.0f,
		+0.5f, +0.5f, +0.5f, 0.0f, 0.5f, 0.0f,
		-0.5f, +0.5f, +0.5f, 0.0f, 0.5f, 1.0f,
		-0.5f, +0.5f, -0.5f, 0.0f, 0.5f, 0.0f
	};

	glm::vec3 start = glm::vec3(-(num_cols / 2.0f) * (scale.x + scale.x * 0.1f), 0, -(num_rows / 2.0f) * (scale.x + scale.x * 0.1f));
	glm::vec3 current = start;
	for(unsigned int i = 0; i < num_rows; ++i) {
		for(unsigned int j = 0; j < num_cols; ++j) {
			positions.push_back(current);
			current.x += scale.x + scale.x * 0.1f;
		}
		current.x = start.x;
		current.z += scale.x + scale.x * 0.1f;
	}

	int center_index = get_index(num_rows / 2, num_cols / 2);
	glm::vec3 center_pos = positions[center_index];
	glm::vec3 last_pos = positions[positions.size() - 1];
	float max_dist = glm::distance(center_pos, last_pos);
	for(unsigned int i = 0; i < num_rows; ++i) {
		for(unsigned int j = 0; j < num_cols; ++j) {
			int index = get_index(i, j);
			glm::vec3 current_pos = positions[index];
			float dist = glm::distance(center_pos, current_pos);
			float offset = map(dist, 0, max_dist, -1, 1);
			float scale = offset;
			scales[index].y = scale;
		}
	}

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * num_floats, vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
}

int WaveMotion::get_index(int row, int col) {
	return row * num_cols + col;
}

void WaveMotion::update(float t) {
	float s_value = (sin(t * 2.0f) + 1.0f) / 2.0f;

	int center_index = get_index(num_rows / 2, num_cols / 2);
	glm::vec3 center_pos = positions[center_index];
	glm::vec3 last_pos = positions[positions.size() - 1];
	float max_dist = glm::distance(center_pos, last_pos);
	for(unsigned int i = 0; i < num_rows; ++i) {
		for(unsigned int j = 0; j < num_cols; ++j) {
			int index = get_index(i, j);
			glm::vec3 current_pos = positions[index];
			float dist = glm::distance(center_pos, current_pos);
			float offset = map(dist, 0, max_dist, -s_value, 0);
			scales_t[index].y = offset;
		}
	}
}

void WaveMotion::draw() {
	for(unsigned int i = 0; i < positions.size(); ++i) {
		glUseProgram(program);

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, positions[i]);
		glm::vec3 tmp_scale = scales[i] + scales_t[i];
		model = glm::scale(model, tmp_scale);
		int model_loc = glGetUniformLocation(program, "u_model");
		glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model));

		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, num_vertices);
	}
}
