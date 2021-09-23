#pragma once
#include "rects.h"
#include "text.h"
#include <random>
#include <chrono>

struct Plot {
	Rects *rects;
	Texts texts;
	std::string distribution;
	glm::vec2 start_pos;

	Plot::Plot(std::string distribution, int shader, glm::vec2 scale, glm::vec2 start_pos, glm::vec2 window_dims) {
		this->distribution = distribution;
		this->start_pos = start_pos;
		rects = new Rects(shader, scale);

		texts.init("..\\data\\consolas.ttf", 64, window_dims);

		init();
	}

	void draw(unsigned int text_shader) {
		rects->draw();
		texts.draw(text_shader);
	}

	private:
	void init() {
		unsigned seed = std::chrono::steady_clock::now().time_since_epoch().count();
		std::default_random_engine rand_eng(seed);

		if(distribution == "normal") {
			std::normal_distribution<double> norm_dist(10, 3);
			std::vector<int> count(20);
			for(unsigned int i = 0; i < 1000; ++i) {
				int number = norm_dist(rand_eng);	
				if(number >= 0 && number <= 20) {
					count[number]++;
				}
			}

			unsigned int num_rects = count.size();
			float per_count_width = 0.005f;
			glm::vec3 current_pos = glm::vec3(start_pos.x, start_pos.y, 0.0f);
			for(unsigned int i = 0; i < num_rects; ++i) {
				float current_height = count[i] * per_count_width;
				glm::vec3 color = glm::vec3(1, 1, 1);
				if(count[i] == 0) {
					current_height = 0.002f;
					color = glm::vec3(1, 0, 0);
				}
				rects->add(glm::vec3(current_pos.x, current_pos.y + current_height / 2.0f, 0.0f), glm::vec2(0.1f, current_height), color);
				current_pos.x += 0.1f + (0.1f * 0.02f);
			}

			float total_width = current_pos.x - start_pos.x;
			glm::vec3 text_pos;
			text_pos.x = start_pos.x + total_width / 2.0f;
			text_pos.y = start_pos.y - 0.2f;
			text_pos.z = 0.0f;
			texts.add_text("normal distribution", text_pos, glm::vec3(1.0f, 0.0f, 0.0f));
		}
		else if(distribution == "uniform") {
			std::normal_distribution<double> uniform_dist(0, 30);
			std::vector<int> count(30);
			for(unsigned int i = 0; i < 10000; ++i) {
				int number = uniform_dist(rand_eng);
				if(number >= 0 && number <= 30) {
					count[number]++;
				}
			}

			unsigned int num_rects = count.size();
			float per_count_width = 0.005f;
			glm::vec3 current_pos = glm::vec3(start_pos.x, start_pos.y, 0.0f);
			for(unsigned int i = 0; i < num_rects; ++i) {
				float current_height = count[i] * per_count_width;
				glm::vec3 color = glm::vec3(1, 1, 1);
				if(count[i] == 0) {
					current_height = 0.002f;
					color = glm::vec3(1, 0, 0);
				}
				rects->add(glm::vec3(current_pos.x, current_pos.y + current_height / 2.0f, 0.0f), glm::vec2(0.1f, current_height), color);
				current_pos.x += 0.1f + (0.1f * 0.02f);
			}

			float total_width = current_pos.x - start_pos.x;
			glm::vec3 text_pos;
			text_pos.x = start_pos.x + total_width / 2.0f;
			text_pos.y = start_pos.y - 0.2f;
			text_pos.z = 0.0f;
			texts.add_text("uniform distribution", text_pos, glm::vec3(1.0f, 0.0f, 0.0f));
		}
		else if(distribution == "bernoulli") {
			std::bernoulli_distribution bernoulli_dist(0.2f); // Give true 4/10 of the times.
			std::vector<int> count(2);
			for(unsigned int i = 0; i < 500; ++i) {
				bool result = bernoulli_dist(rand_eng);
				if (result)
					count[0]++;
				else 
					count[1]++;
			}

			unsigned int num_rects = count.size();
			float per_count_width = 0.003f;
			glm::vec3 current_pos = glm::vec3(start_pos.x, start_pos.y, 0.0f);
			for(unsigned int i = 0; i < num_rects; ++i) {
				float current_height = count[i] * per_count_width;
				glm::vec3 color = glm::vec3(1, 1, 1);
				if(count[i] == 0) {
					current_height = 0.002f;
					color = glm::vec3(1, 0, 0);
				}
				rects->add(glm::vec3(current_pos.x, current_pos.y + current_height / 2.0f, 0.0f), glm::vec2(0.1f, current_height), color);
				current_pos.x += 0.1f + (0.1f * 0.02f);
			}

			float total_width = current_pos.x - start_pos.x;
			glm::vec3 text_pos;
			text_pos.x = start_pos.x + total_width / 2.0f;
			text_pos.y = start_pos.y - 0.2f;
			text_pos.z = 0.0f;
			texts.add_text("bernoulli distribution: 0.2", text_pos, glm::vec3(1.0f, 0.0f, 0.0f));
		}

		else if(distribution == "exponential") {
			std::exponential_distribution<> exponential_dist(1);
			std::map<int, int> info;
			for(unsigned int i = 0; i < 1000; ++i) {
				int result = exponential_dist(rand_eng);	
				info[result] += 1;
			}

			unsigned int num_rects = info.size();
			float per_count_width = 0.002f;
			glm::vec3 current_pos = glm::vec3(start_pos.x, start_pos.y, 0.0f);
			for(auto p: info) {
				float current_height = p.second * per_count_width;
				glm::vec3 color = glm::vec3(1, 1, 1);
				if(p.second == 0) {
					current_height = 0.002f;
					color = glm::vec3(1, 0, 0);
				}
				rects->add(glm::vec3(current_pos.x, current_pos.y + current_height / 2.0f, 0.0f), glm::vec2(0.1f, current_height), color);
				current_pos.x += 0.1f + (0.1f * 0.02f);
			}

			float total_width = current_pos.x - start_pos.x;
			glm::vec3 text_pos;
			text_pos.x = start_pos.x + total_width / 2.0f;
			text_pos.y = start_pos.y - 0.2f;
			text_pos.z = 0.0f;
			texts.add_text("exponential distribution", text_pos, glm::vec3(1.0f, 0.0f, 0.0f));

		}
	}
};
