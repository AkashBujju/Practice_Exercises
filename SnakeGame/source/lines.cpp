#include "lines.h"
#include <cstdlib>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <ctime>

SnakeGame::SnakeGame(GLint program, float width, int head_direction, glm::vec3 snake_color, glm::vec3 food_color) {
	srand((unsigned) time(0));
	
	this->program = program;
	this->width = width;
	this->head_direction = head_direction;
	this->snake_color = snake_color;
	this->food_color = food_color;
	virtual_grid_row_count = 2 / width;
	virtual_grid_col_count = 2 / width;

	block_dist = width + (width * 0.05f);
	extreme_left_x_pos = - (block_dist * floor(virtual_grid_row_count / 2));
	extreme_down_y_pos = - (block_dist * floor(virtual_grid_col_count / 2));

	{
		if(((int)floor(virtual_grid_row_count)) % 2 == 0) {
			std::cout << "-- ERROR: Please change the 'width'. grid row count cannot be even." << std::endl;
		}
	}

	extend_body();
	extend_body();
	extend_body();
	extend_body();

	/* FOOD */
	float vertices[] = {
		-width/2, +width/2, 0, /* TL */
		+width/2, +width/2, 0, /* TR */
		+width/2, -width/2, 0, /* BR */
		-width/2, +width/2, 0, /* TL */
		-width/2, -width/2, 0, /* BL */
		+width/2, -width/2, 0  /* BR */
	};
	glGenVertexArrays(1, &food.vao);
	glGenBuffers(1, &food.vbo);
	glBindVertexArray(food.vao);
	glBindBuffer(GL_ARRAY_BUFFER, food.vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	reset_food_position();
	/* FOOD */
}

void SnakeGame::extend_body() {
	float vertices[] = {
		-width/2, +width/2, 0, /* TL */
		+width/2, +width/2, 0, /* TR */
		+width/2, -width/2, 0, /* BR */
		-width/2, +width/2, 0, /* TL */
		-width/2, -width/2, 0, /* BL */
		+width/2, -width/2, 0  /* BR */
	};

	unsigned int snake_size = snake.size();

	Rect rect;
	if(snake_size == 0) {
		rect.position = glm::vec3(0, 0, 0);
		rect.last_dir = head_direction;
		rect.next_dir = head_direction;
		rect.virtual_grid_pos.x = virtual_grid_row_count / 2 + 1;
		rect.virtual_grid_pos.y = virtual_grid_col_count / 2 + 1;
	}
	else {
		glm::vec3 last_body_pos = snake[snake_size - 1].position;
		int last_last_dir = snake[snake_size - 1].last_dir;
		int last_next_dir = snake[snake_size - 1].next_dir;
		block_dist = width + (width * 0.05f);
		if(last_last_dir == 4) {
			rect.position = glm::vec3(last_body_pos.x, last_body_pos.y - block_dist, 0);
			rect.virtual_grid_pos.x = snake[snake_size - 1].virtual_grid_pos.x;
			rect.virtual_grid_pos.y = snake[snake_size - 1].virtual_grid_pos.y - 1;
		}
		else if(last_last_dir == 3) {
			rect.position = glm::vec3(last_body_pos.x, last_body_pos.y + block_dist, 0);
			rect.virtual_grid_pos.x = snake[snake_size - 1].virtual_grid_pos.x;
			rect.virtual_grid_pos.y = snake[snake_size - 1].virtual_grid_pos.y + 1;
		}
		else if(last_last_dir == 2) {
			rect.position = glm::vec3(last_body_pos.x - block_dist, last_body_pos.y, 0);
			rect.virtual_grid_pos.x = snake[snake_size - 1].virtual_grid_pos.x - 1;
			rect.virtual_grid_pos.y = snake[snake_size - 1].virtual_grid_pos.y;
		}
		else if(last_last_dir == 1) {
			rect.position = glm::vec3(last_body_pos.x + block_dist, last_body_pos.y, 0);
			rect.virtual_grid_pos.x = snake[snake_size - 1].virtual_grid_pos.x + 1;
			rect.virtual_grid_pos.y = snake[snake_size - 1].virtual_grid_pos.y;
		}

		rect.last_dir = last_last_dir;
		rect.next_dir = last_next_dir;
	}

	snake.push_back(rect);

	glGenVertexArrays(1, &snake[snake_size].vao);
	glGenBuffers(1, &snake[snake_size].vbo);
	glBindVertexArray(snake[snake_size].vao);
	glBindBuffer(GL_ARRAY_BUFFER, snake[snake_size].vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}

void SnakeGame::reset_food_position() {
	int row = rand() % (virtual_grid_row_count);	
	int col = rand() % (virtual_grid_col_count);	

	row += 1;
	col += 1;
	
	int updated_row = row - (virtual_grid_row_count / 2 + 1);
	int updated_col = col - (virtual_grid_col_count / 2 + 1);

	float to_add = width + (width * 0.05f);
	float x_pos = updated_col * (to_add);
	float y_pos = updated_row * (to_add);

	food.virtual_grid_pos.x = col;
	food.virtual_grid_pos.y = row;
	food.position.x = x_pos;
	food.position.y = y_pos;
}

void SnakeGame::update() {
	if((snake[0].virtual_grid_pos.x == food.virtual_grid_pos.x) && (snake[0].virtual_grid_pos.y == food.virtual_grid_pos.y)) {
		reset_food_position();
		extend_body();
	}
}

void SnakeGame::set_snake_head_direction(int direction) {
	if((head_direction == 4 && direction == 3) || (direction == 4 && head_direction == 3))
		return;
	else if((head_direction == 2 && direction == 1) || (direction == 2 && head_direction == 1))
		return;

	head_direction = direction;
}

void SnakeGame::move_snake() {
	block_dist = width + (width * 0.05f);
	snake[0].next_dir = head_direction;
	snake[0].last_dir = head_direction;

	for(unsigned int i = 1; i < snake.size(); ++i)
		snake[i].last_dir = snake[i].next_dir;

	for(unsigned int i = 0; i < snake.size(); ++i) {
		if(snake[i].last_dir == 4)
			snake[i].position.y += block_dist;
		else if(snake[i].last_dir == 3)
			snake[i].position.y -= block_dist;
		else if(snake[i].last_dir == 2)
			snake[i].position.x += block_dist;
		else if(snake[i].last_dir == 1)
			snake[i].position.x -= block_dist;
	}

	for(unsigned int i = 1; i < snake.size(); ++i)
		snake[i].next_dir = snake[i - 1].last_dir;

	/* Updating virtual_grid_pos */
	for(unsigned int i = 0; i < snake.size(); ++i) {
		if(snake[i].last_dir == 1)
			snake[i].virtual_grid_pos.x -= 1;
		else if(snake[i].last_dir == 2)
			snake[i].virtual_grid_pos.x += 1;
		else if(snake[i].last_dir == 3)
			snake[i].virtual_grid_pos.y -= 1;
		else if(snake[i].last_dir == 4)
			snake[i].virtual_grid_pos.y += 1;
	}
	/* Updating virtual_grid_pos */
}

void SnakeGame::draw() {
	for(unsigned int i = 0; i < snake.size(); ++i) {
		glUseProgram(program);

		int position_loc = glGetUniformLocation(program, "position");
		if (position_loc != -1)
			glUniform3fv(position_loc, 1, glm::value_ptr(snake[i].position));

		int color_loc = glGetUniformLocation(program, "color");
		if (color_loc != -1)
			glUniform3fv(color_loc, 1, glm::value_ptr(snake_color));

		glBindVertexArray(snake[i].vao);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	glUseProgram(program);
	int position_loc = glGetUniformLocation(program, "position");
	int color_loc = glGetUniformLocation(program, "color");
	if (position_loc != -1)
		glUniform3fv(position_loc, 1, glm::value_ptr(food.position));
	if (color_loc != -1)
		glUniform3fv(color_loc, 1, glm::value_ptr(food_color));
	glBindVertexArray(food.vao);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}
