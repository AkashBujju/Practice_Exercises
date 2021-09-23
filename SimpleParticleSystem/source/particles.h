#pragma once

#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>

struct ParticleSystem {
	GLint program;
	GLuint vbo;
	std::vector<glm::vec2> positions;
	glm::vec2 initial_position;
	glm::vec4 initial_color;
	std::vector<glm::vec2> velocities;
	std::vector<glm::vec2> accelerations;
	std::vector<glm::vec4> colors;
	std::vector<GLuint> vaos;
	GLuint num_vertices;
	unsigned int max_particles;
	unsigned int num_floats;
	float *vertices;
	int direction;

	public:
	ParticleSystem(GLint program, unsigned int num_particles, float radius, glm::vec4 color, glm::vec2 initial_pos, int direction);
	void add_new_particle();
	void update();
	void draw();

	private:
	void check_and_reset();
};
