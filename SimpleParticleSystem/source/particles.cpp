#include "particles.h"
#include <cstdlib>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

extern float map(float src, float src_start, float src_end, float des_start, float des_end);
extern float to_radians(float degree);

ParticleSystem::ParticleSystem(GLint program, unsigned int num_particles, float radius, glm::vec4 color, glm::vec2 initial_pos, int direction) {
	this->program = program;
	this->direction = direction;
	initial_position = initial_pos;
	max_particles = num_particles;
	initial_color = color;

	positions.reserve(num_particles);
	velocities.reserve(num_particles);
	accelerations.reserve(num_particles);
	colors.reserve(num_particles);
	vaos.reserve(num_particles);

	unsigned int num_iterations = 100;
	float step_angle = 360.0f / num_iterations;
	this->num_floats = (1 + num_iterations) * 2 + 2;
	this->num_vertices = num_iterations + 2;
	float *vertices = (float*)malloc(sizeof(float) * num_floats);
	this->vertices = vertices;

	vertices[0] = 0;
	vertices[1] = 0;
	vertices[2] = radius;
	vertices[3] = 0;

	unsigned int index = 4;
	float current_angle = step_angle;
	for(unsigned int i = 0; i < num_iterations; ++i) {
		float x = radius * cos(to_radians(current_angle));
		float y = radius * sin(to_radians(current_angle));

		vertices[index++] = x;
		vertices[index++] = y;
		current_angle += step_angle;	
	}

	for(unsigned int i = 0; i < max_particles; ++i) {
		colors.push_back(glm::vec4(color.x, color.y, color.z, color.a));
		if(direction == 1)
			accelerations.push_back(glm::vec2(0, -0.000005f));
		if(direction == 2)
			accelerations.push_back(glm::vec2(0, +0.000005f));
		if(direction == 3)
			accelerations.push_back(glm::vec2(-0.000005f, 0));
		if(direction == 4)
			accelerations.push_back(glm::vec2(+0.000005f, 0));
	}
}

void ParticleSystem::add_new_particle() {
	unsigned int current_index = positions.size();
	positions.push_back(glm::vec2(initial_position.x, initial_position.y));
	float vel_x = (float) rand() / (float) RAND_MAX;
	float vel_y = (float) rand() / (float) RAND_MAX;
	if(direction == 1) {
		vel_x = map(vel_x, 0, 1, -0.0002f, +0.0002f);
		vel_y = map(vel_y, 0, 1, 0.002f, +0.0035f);
	}
	else if(direction == 2) {
		vel_x = map(vel_x, 0, 1, -0.0002f, +0.0002f);
		vel_y = map(vel_y, 0, 1, -0.002f, -0.0035f);
	}
	else if(direction == 3) {
		vel_x = map(vel_x, 0, 1, +0.002f, +0.0035f);
		vel_y = map(vel_y, 0, 1, -0.0002f, +0.0002f);
	}
	else if(direction == 4) {
		vel_x = map(vel_x, 0, 1, -0.002f, -0.0035f);
		vel_y = map(vel_y, 0, 1, -0.0002f, +0.0002f);
	}
	velocities.push_back(glm::vec2(vel_x, vel_y));
	vaos.push_back(0);

	glGenVertexArrays(1, &vaos[current_index]);
	glGenBuffers(1, &vbo);
	glBindVertexArray(vaos[current_index]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * num_floats, vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}

void ParticleSystem::update() {
	check_and_reset();

	for(unsigned int i = 0; i < positions.size(); ++i) {
		positions[i].x += velocities[i].x;
		positions[i].y += velocities[i].y;

		colors[i].a -= 0.001f;

		velocities[i].y += accelerations[i].y;
		velocities[i].x += accelerations[i].x;
	}
}

void ParticleSystem::check_and_reset() {
	if(positions.size() < max_particles)
		return;

	for(unsigned int i = 0; i < positions.size(); ++i) {
		positions[i] = glm::vec2(initial_position.x, initial_position.y);
		float vel_x = (float) rand() / (float) RAND_MAX;
		float vel_y = (float) rand() / (float) RAND_MAX;
		vel_x = map(vel_x, 0, 1, -0.0002f, +0.0002f);
		vel_y = map(vel_y, 0, 1, 0.002f, +0.0035f);
		velocities[i] = glm::vec2(0, 0.015f);
		accelerations[i] = glm::vec2(0, -0.0001f);
		colors[i] = glm::vec4(initial_color.x, initial_color.y, initial_color.z, initial_color.a);
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
			glUniform4fv(color_loc, 1, glm::value_ptr(colors[i]));
		}

		glBindVertexArray(vaos[i]);
		glDrawArrays(GL_TRIANGLE_FAN, 0, num_vertices);
	}
}
