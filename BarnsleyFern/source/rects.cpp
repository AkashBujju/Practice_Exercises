#include "rects.h"
#define GLM_FORCE_RADIANS
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

extern float map(float src, float src_start, float src_end, float des_start, float des_end);
extern float to_radians(float degree);

extern int window_width;
extern int window_height;

Points::Points(GLint program) {
	this->program = program;
	end_index = 0;
	max_index = 11000;

	positions.reserve(max_index - 1);
	colors.reserve(max_index - 1);
	vaos.reserve(max_index - 1);

	float vertices[3] = {
		0, 0, 0
	};

	glm::vec3 pos = glm::vec3(0, 0, 0);
	for(unsigned int i = 0; i < max_index; ++i) {
		float r = (float)rand() / (float)RAND_MAX;
		positions.push_back(pos);

		float gr = map(pos.x, -2.182f, +2.655f, 1.0f, 0.0f);
		colors.push_back(glm::vec3(0, gr, 0));

		if(r < 0.01f) {
			pos.x = 0;
			pos.y = 0.16f * pos.y;
		}
		else if(r < 0.86f) {
			pos.x = 0.85f * pos.x + 0.04f * pos.y;
			pos.y = -0.04f * pos.x + 0.85f * pos.y + 1.6f;
		}
		else if(r < 0.93f) {
			pos.x = 0.2f * pos.x - 0.26f * pos.y;
			pos.y = 0.23f * pos.x + 0.22f * pos.y + 1.6f;
		}
		else {
			pos.x = -0.15f * pos.x + 0.28f * pos.y;
			pos.y = 0.26f * pos.x + 0.24f * pos.y + 0.44f;
		}

	//	printf("%.3f, %.3f, %.3f\n", positions[i].x, positions[i].y, positions[i].z);
	}

	for(unsigned int i = 0; i < max_index; ++i) {
		glGenVertexArrays(1, &vaos[i]);
		glGenBuffers(1, &vbo);
		glBindVertexArray(vaos[i]);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3, vertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
	}
}

void Points::add() {
	if(end_index >= max_index) {
		return;
	}

	end_index += 1;
}

void Points::draw() {
	// printf("end_index: %d\n", end_index);
	for(unsigned int i = 0; i <= end_index; ++i) {
		glUseProgram(program);

		int position_loc = glGetUniformLocation(program, "u_pos");
		glUniform3fv(position_loc, 1, glm::value_ptr(positions[i]));

		int color_loc = glGetUniformLocation(program, "u_color");
		glUniform3fv(color_loc, 1, glm::value_ptr(colors[i]));

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, positions[i]);
		int model_loc = glGetUniformLocation(program, "u_model");
		glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model));

		// printf("HERE(%d)\n", i);

		glBindVertexArray(vaos[i]);
		glDrawArrays(GL_POINTS, 0, 1);
	}
}
