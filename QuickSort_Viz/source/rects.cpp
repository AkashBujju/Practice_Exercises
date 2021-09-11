#include "rects.h"
#include <cstdlib>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

extern float map(float src, float src_start, float src_end, float des_start, float des_end);

void Rects::init(GLint program, std::vector<glm::vec3> colors, std::vector<float> numbers) {
	this->program = program;
	this->per_width = (1.98f / numbers.size()) - (0.05f * (1.98f / numbers.size()));

	float max_number = -99999999;
	float min_number = +99999999;
	for(unsigned int i = 0; i < numbers.size(); ++i) {
		if(numbers[i] < min_number)
			min_number = numbers[i];
		if(numbers[i] > max_number)
			max_number = numbers[i];
	}

	float start_x_pos = -0.98 + per_width / 2.0f;
	for(unsigned int i = 0; i < numbers.size(); ++i) {
		float height = map(numbers[i], min_number, max_number, -1, +1);
		add_number(numbers[i], start_x_pos, height, colors[i]);
		start_x_pos += per_width + (per_width * 0.05f);
	}
}

void Rects::swap_positions(int index_0, int index_1) {
	float index_0_pos = numbers[index_0].x_position;
	float index_1_pos = numbers[index_1].x_position;

	numbers[index_0].x_position = index_1_pos;
	numbers[index_1].x_position = index_0_pos;

	Number tmp = numbers[index_0];
	numbers[index_0] = numbers[index_1];
	numbers[index_1] = tmp;
}

void Rects::add_number(float num, float x_pos, float height, glm::vec3 color) {
	Number number;
	number.number = num;
	number.x_position = x_pos;

	float vertices[] = {
		-per_width / 2, height, color.x, color.y, color.z,
		+per_width / 2, height, color.x, color.y, color.z,
		+per_width / 2, -0.98f, color.x, color.y, color.z,
		-per_width / 2, height, color.x, color.y, color.z,
		-per_width / 2, -0.98f, color.x, color.y, color.z,
		+per_width / 2, -0.98f, color.x, color.y, color.z
	};

	glGenVertexArrays(1, &number.vao);
	glGenBuffers(1, &vbo);
	glBindVertexArray(number.vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);

	numbers.push_back(number);
}

void Rects::draw() {
	for(unsigned int i = 0; i < numbers.size(); ++i) {
		glUseProgram(program);

		int position_loc = glGetUniformLocation(program, "x_pos");
		if (position_loc != -1) {
			glUniform1f(position_loc, numbers[i].x_position);
		}

		glBindVertexArray(numbers[i].vao);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
}
