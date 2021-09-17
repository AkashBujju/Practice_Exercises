#pragma once
#include "rects.h"
#include <cmath>

struct Rockets {
	Rects *rects;
	unsigned int num_rockets;

	Rockets(GLint program, unsigned int num_rockets, glm::vec2 dims, glm::vec4 initial_color, glm::vec2 initial_position);
};

struct Dna {
	std::vector<float> speeds;
	std::vector<float> angles;
};

struct SmartRockets {
	Rockets *rockets;
	unsigned int dna_lifespan;
	unsigned int current_dna_index;
	std::vector<Dna> dnas;
	std::vector<bool> stop_moving;
	std::vector<float> fitness;
	glm::vec2 initial_position;
	int iteration_number;

	SmartRockets(GLint program, unsigned int num_rockets, glm::vec2 dims, glm::vec4 initial_color, glm::vec2 initial_position, unsigned int dna_lifespan);
	void calculate_fitness(ObstaclesAndTarget obt);
	void update_to_new_generation(ObstaclesAndTarget obt);
	void update(ObstaclesAndTarget obt);
	Dna cross_over(int dna_index_1, int dna_index_2);
	bool has_collided(ObstaclesAndTarget obt, glm::vec2 position);
	void draw();
};
