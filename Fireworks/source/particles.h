#pragma once

#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>

struct ParticleSystem {
	GLint program;
	GLuint vbo;
	std::vector<glm::vec2> positions;
	std::vector<glm::vec2> initial_positions;
	std::vector<glm::vec2> velocities;
	std::vector<glm::vec2> accelerations;
	std::vector<bool> bursted;
	std::vector<glm::vec3> colors;
	std::vector<GLuint> vaos;

	public:
	ParticleSystem(GLint program, unsigned int num_particles, glm::vec3 color, glm::vec2 initial_pos);
	void update();
	void draw();

	private:
	void check_and_reset();
	void reset();
};
