#include "rockets.h"

#include <iostream> // @TMP

extern float map(float src, float src_start, float src_end, float des_start, float des_end);

Rockets::Rockets(GLint program, unsigned int num_rockets, glm::vec2 dims, glm::vec4 initial_color, glm::vec2 initial_position) {
	this->num_rockets = num_rockets;
	rects = new Rects(program, num_rockets, dims, initial_color, initial_position);
}

SmartRockets::SmartRockets(GLint program, unsigned int num_rockets, glm::vec2 dims, glm::vec4 initial_color, glm::vec2 initial_position, unsigned int dna_lifespan) {
	rockets = new Rockets(program, num_rockets, dims, initial_color, initial_position);

	this->initial_position = initial_position;
	this->dna_lifespan = dna_lifespan;
	current_dna_index = 0;
	dnas.reserve(num_rockets);
	stop_moving.reserve(num_rockets);
	fitness.reserve(num_rockets);
	iteration_number = 0;

	for(unsigned int i = 0; i < num_rockets; ++i) {
		Dna dna;
		dna.speeds.reserve(dna_lifespan);
		dna.angles.reserve(dna_lifespan);
		dnas.push_back(dna);
		fitness.push_back(0);
		stop_moving.push_back(false);
	}

	for(unsigned int i = 0; i < num_rockets; ++i) {
		for(unsigned int j = 0; j < dna_lifespan; ++j) {
			float vel = (float) rand() / (float) RAND_MAX;
			float angle = (float) rand() / (float) RAND_MAX;
			vel = map(vel, 0, 1, 0.01f, +0.03f);
			angle = map(angle, 0, 1, -110.0f, +110.0f);
			for(int x = 0; x < 10; ++x) {
				dnas[i].speeds.push_back(vel);
				dnas[i].angles.push_back(angle);
			}
			j += 10;
		}
	}
}

void SmartRockets::update(ObstaclesAndTarget obt) {
	if(current_dna_index >= dna_lifespan) {
		update_to_new_generation(obt);
		return;
	}

	for(unsigned int i = 0; i < rockets->num_rockets; ++i) {
		if(stop_moving[i])
			continue;

		float angle = dnas[i].angles[current_dna_index];
		float vel = dnas[i].speeds[current_dna_index];
		float new_x = rockets->rects->positions[i].x + (vel * cos(glm::radians(angle + 90)));
		float new_y = rockets->rects->positions[i].y + (vel * sin(glm::radians(angle + 90)));
		
		rockets->rects->positions[i] = glm::vec2(new_x, new_y);
		rockets->rects->angles[i] = angle;

		if(has_collided(obt, rockets->rects->positions[i])) {
			stop_moving[i] = true;
		}
	}

	current_dna_index += 1;
}

bool SmartRockets::has_collided(ObstaclesAndTarget obt, glm::vec2 position) {
	for(unsigned int i = 0; i < obt.positions.size(); ++i) {
		if((position.x > obt.positions[i].x - obt.dims[i].x) && (position.x < obt.positions[i].x + obt.dims[i].x) && (position.y > obt.positions[i].y - obt.dims[i].y) && (position.y < obt.positions[i].y + obt.dims[i].y)) {
			return true;
		}
	}

	return false;
}

void SmartRockets::update_to_new_generation(ObstaclesAndTarget obt) {
	calculate_fitness(obt);

	std::vector<int> mating_pool;
	for(unsigned int i = 0; i < rockets->num_rockets; ++i) {
		int n = fitness[i] * 100;
		for(int j = 0; j < n; ++j) {
			mating_pool.push_back(i);
		}
	}

	std::vector<Dna> new_dnas;
	for(unsigned int i = 0; i < rockets->num_rockets; ++i) {
		float index_1 = (float) rand() / (float) RAND_MAX;
		float index_2 = (float) rand() / (float) RAND_MAX;
		index_1 = (int) map(index_1, 0, 1, 0, mating_pool.size() - 1);
		index_2 = (int) map(index_2, 0, 1, 0, mating_pool.size() - 1);

		Dna new_dna = cross_over(mating_pool[index_1], mating_pool[index_2]);
		new_dnas.push_back(new_dna);
	}

	dnas = new_dnas;

	// Resetting Positions to Initial Position
	for(unsigned int i = 0; i < rockets->num_rockets; ++i) {
		rockets->rects->positions[i] = initial_position;
		stop_moving[i] = false;
	}
	current_dna_index = 0;

	iteration_number += 1;
	std::cout << "Iteration " << iteration_number << " ended." << std::endl;
}

Dna SmartRockets::cross_over(int dna_index_1, int dna_index_2) {
	Dna new_dna;
	new_dna.speeds.reserve(dna_lifespan);
	new_dna.angles.reserve(dna_lifespan);

	int mid = dna_lifespan / 2;
	for(unsigned int i = 0; i < dna_lifespan; ++i) {
		if(i > mid) {
			new_dna.speeds.push_back(dnas[dna_index_1].speeds[i]);
			new_dna.angles.push_back(dnas[dna_index_1].angles[i]);
		}
		else {
			new_dna.speeds.push_back(dnas[dna_index_2].speeds[i]);
			new_dna.angles.push_back(dnas[dna_index_2].angles[i]);
		}
	}

	return new_dna;
}

void SmartRockets::calculate_fitness(ObstaclesAndTarget obt) {
	float max_fitness = 0;
	for(unsigned int i = 0; i < rockets->num_rockets; ++i) {
		fitness[i] = 1 / (glm::distance(obt.positions[0], rockets->rects->positions[i])) + 0.00001f;

		if(fitness[i] > max_fitness)
			max_fitness = fitness[i];
	}

	for(unsigned int i = 0; i < rockets->num_rockets; ++i) {
		fitness[i] /= max_fitness;
	}
}

void SmartRockets::draw() {
	rockets->rects->draw();
}
