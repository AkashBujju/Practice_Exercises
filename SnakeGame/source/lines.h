#pragma once

#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>

struct Rect {
	GLuint vao;
	GLuint vbo;
	glm::vec3 position;
	/* @Note: DIRECTION: 1=LEFT, 2=RIGHT, 3=DOWN, 4=UP */
	int last_dir, next_dir;
	glm::vec2 virtual_grid_pos;
};

class SnakeGame {
	private:
		GLint program;
		float width;
		glm::vec3 snake_color;
		glm::vec3 food_color;
		std::vector<Rect> snake;
		int head_direction;
		Rect food;

		int virtual_grid_row_count;
		int virtual_grid_col_count;
		float extreme_left_x_pos;
		float extreme_down_y_pos;
		float block_dist;
	public:
		SnakeGame(GLint program, float width, int direction, glm::vec3 snake_color, glm::vec3 food_color);
		void set_snake_head_direction(int direction);
		void move_snake();
		void draw();
		void update();
	private:
		void extend_body();
		void reset_food_position();
};
