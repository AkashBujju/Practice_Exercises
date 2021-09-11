#include "particles.h"
#include <cstdlib>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

extern float map(float src, float src_start, float src_end, float des_start, float des_end);

ParticleSystem::ParticleSystem(GLint program, unsigned int num_particles, glm::vec3 color, glm::vec2 initial_pos) {
	this->program = program;

	positions.reserve(num_particles);
	initial_positions.reserve(num_particles);
	velocities.reserve(num_particles);
	accelerations.reserve(num_particles);
	colors.reserve(num_particles);
	bursted.reserve(num_particles);
	vaos.reserve(num_particles);
	
	float vertices[] = {
		0, 0
	};

	for(unsigned int i = 0; i < num_particles; ++i) {
		initial_positions[i] = initial_pos;
		positions.push_back(glm::vec2(initial_pos.x, initial_pos.y));
		velocities.push_back(glm::vec2(0, 0.015f));
		accelerations.push_back(glm::vec2(0, -0.0001f));
		colors.push_back(glm::vec3(color.x, color.y, color.z));
		bursted.push_back(false);
		vaos.push_back(0);

		glGenVertexArrays(1, &vaos[i]);
		glGenBuffers(1, &vbo);
		glBindVertexArray(vaos[i]);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
	}
}

void ParticleSystem::update() {
	check_and_reset();

	for(unsigned int i = 0; i < positions.size(); ++i) {
		positions[i].x += velocities[i].x;
		positions[i].y += velocities[i].y;

		velocities[i].y += accelerations[i].y;
		velocities[i].x += accelerations[i].x;

		if(velocities[i].y < 0.00001f && !bursted[i]) {
			float vel_x = (float) rand() / (float) RAND_MAX;
			float vel_y = (float) rand() / (float) RAND_MAX;
			vel_x = map(vel_x, 0, 1, -0.0015f, 0.0020f);
			vel_y = map(vel_y, 0, 1, -0.002f, 0.004f);
			velocities[i].x = vel_x;
			velocities[i].y = vel_y;

			bursted[i] = true;
		}
	}
}

void ParticleSystem::check_and_reset() {
	unsigned int count = 0;
	for(unsigned int i = 0; i < positions.size(); ++i) {
		if(positions[i].y < -1.0f)
			count += 1;
	}

	if(count > (positions.size() - 5))
		reset();
}

void ParticleSystem::reset() {
	for(unsigned int i = 0; i < positions.size(); ++i) {
		positions[i] = glm::vec2(initial_positions[i].x, initial_positions[i].y);
		velocities[i] = glm::vec2(0, 0.015f);
		accelerations[i] = glm::vec2(0, -0.0001f);
		colors[i] = glm::vec3(colors[i].x, colors[i].y, colors[i].z);
		bursted[i] = false;
	}
}

void ParticleSystem::draw() {
	for(unsigned int i = 0; i < positions.size(); ++i) {
		glUseProgram(program);

		int position_loc = glGetUniformLocation(program, "u_pos");
		if (position_loc != -1) {
			glUniform2fv(position_loc, 1, glm::value_ptr(positions[i]));
		}

		int color_loc = glGetUniformLocation(program, "u_color");
		if (color_loc != -1) {
			glUniform3fv(color_loc, 1, glm::value_ptr(colors[i]));
		}

		glBindVertexArray(vaos[i]);
		glDrawArrays(GL_POINTS, 0, 2);
	}
}
