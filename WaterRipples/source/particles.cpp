#include "particles.h"
#include <cstdlib>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

extern float map(float src, float src_start, float src_end, float des_start, float des_end);
extern float to_radians(float degree);

Pool::Pool(GLint program, glm::vec2 pool_dims, float particle_gap, glm::vec4 pool_color) {
	this->program = program;
	this->pool_color = pool_color;
	this->pool_dims = pool_dims;
	this->particle_gap = particle_gap;

	this->num_particles = ceil(((float) pool_dims.x / particle_gap) * ((float) pool_dims.y / particle_gap));

	positions.reserve(this->num_particles);
	current_colors.reserve(this->num_particles);
	previous_colors.reserve(this->num_particles);
	vaos.reserve(this->num_particles);

	num_particle_dims.x = pool_dims.x / particle_gap;
	num_particle_dims.y = pool_dims.y / particle_gap;

	glm::vec2 top_left = glm::vec2(-pool_dims.x / 2.0f, pool_dims.y / 2.0f);
	for(float y = top_left.y; y > (-pool_dims.y / 2.0f + particle_gap); y -= particle_gap) {
		for(float x = top_left.x; x < (pool_dims.x / 2.0f - particle_gap); x += particle_gap) {
			positions.push_back(glm::vec2(x, y));
		}
	}

	for(unsigned int i = 0; i < this->num_particles; ++i) {
		current_colors.push_back(pool_color);
		previous_colors.push_back(pool_color);
		vaos.push_back(0);
	}

	float vertices[] = { 0, 0 };
	for(unsigned int current_index = 0; current_index < this->num_particles; ++current_index) {
		glGenVertexArrays(1, &vaos[current_index]);
		glGenBuffers(1, &vbo);
		glBindVertexArray(vaos[current_index]);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2, vertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
	}
}

void Pool::swap_colors() {
	std::vector<glm::vec4> tmp = current_colors;
	current_colors = previous_colors;
	previous_colors = tmp;
}

void Pool::update() {
	float dampening = 0.99f;
	for(unsigned int x = 1; x < num_particle_dims.x - 1; ++x) {
		for(unsigned int y = 1; y < num_particle_dims.y - 1; ++y) {
			glm::vec4 current_color = get_current_colors(x, y);
			glm::vec4 color = (get_previous_colors(x - 1, y) +
				               get_previous_colors(x + 1, y) +
									get_previous_colors(x, y - 1) +
									get_previous_colors(x, y + 1)) / 2.0f -
									current_color;
			color *= dampening;
			set_current_colors(x, y, color);
		}
	}
}

void Pool::set_current_colors(int x, int y, glm::vec4 color)
{
	if(x > (num_particle_dims.x - 1) || y > (num_particle_dims.y - 1))
		return;

	unsigned int index = y * num_particle_dims.x + x;
	current_colors[index] = color;
}

glm::vec4 Pool::get_current_colors(int x, int y)
{
	if(x > (num_particle_dims.x - 1) || y > (num_particle_dims.y - 1))
		return pool_color;

	unsigned int index = y * num_particle_dims.x + x;
	return current_colors[index];
}

glm::vec4 Pool::get_previous_colors(int x, int y)
{
	if(x > (num_particle_dims.x - 1) || y > (num_particle_dims.y - 1))
		return pool_color;

	unsigned int index = y * num_particle_dims.x + x;
	return previous_colors[index];
}

void Pool::draw() {
	for(unsigned int i = 0; i < num_particles; ++i) {
		glUseProgram(program);

		int position_loc = glGetUniformLocation(program, "u_pos");
		if (position_loc != -1) {
			glUniform2fv(position_loc, 1, glm::value_ptr(positions[i]));
		}

		int color_loc = glGetUniformLocation(program, "u_color");
		if (color_loc != -1) {
			glUniform4fv(color_loc, 1, glm::value_ptr(current_colors[i]));
		}

		glBindVertexArray(vaos[i]);
		glDrawArrays(GL_POINTS, 0, 1);
	}
}
