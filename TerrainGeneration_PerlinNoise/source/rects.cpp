#include "rects.h"
#define GLM_FORCE_RADIANS
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <map>
#include <string>
#include "PerlinNoise.hpp"

extern float map(float src, float src_start, float src_end, float des_start, float des_end);
extern float to_radians(float degree);

Terrain::Terrain(GLint program, glm::vec2 scale, int num_rows, int num_cols) {
	this->program = program;
	this->scale = scale;
	this->num_rows = num_rows;
	this->num_cols = num_cols;

	int num_floats = num_rows * num_cols * 6 * 6;
	this->num_vertices = num_floats / 3.0f;
	float *vertices = (float*)malloc(sizeof(float) * num_floats);

	int seed = 54321;
	float x = 0.01f;
	int octaves = 16;
	float z_scale = 2.0f;
	const siv::PerlinNoise perlin(seed);

	std::map<std::string, float> z_values;
	glm::vec3 start_pos = glm::vec3(-((num_cols / 2.0f) * scale.x), +((num_rows / 2.0f) * scale.y), 0);
	for(unsigned i = 1; i <= num_rows; ++i) {
		for(unsigned int j = 1; j <= num_cols; ++j) {
			glm::vec2 v0 = glm::vec2(start_pos.x, start_pos.y);
			glm::vec2 v1 = glm::vec2(start_pos.x + scale.x, start_pos.y);
			glm::vec2 v2 = glm::vec2(start_pos.x, start_pos.y - scale.y);
			glm::vec2 v3 = glm::vec2(start_pos.x, start_pos.y - scale.y);
			glm::vec2 v4 = glm::vec2(start_pos.x + scale.x, start_pos.y);
			glm::vec2 v5 = glm::vec2(start_pos.x + scale.x, start_pos.y - scale.y);

			std::string s1 = std::to_string(v0.x) + std::to_string(v0.y);
			std::string s2 = std::to_string(v1.x) + std::to_string(v1.y);
			std::string s3 = std::to_string(v2.x) + std::to_string(v2.y);
			std::string s4 = std::to_string(v3.x) + std::to_string(v3.y);
			std::string s5 = std::to_string(v4.x) + std::to_string(v4.y);
			std::string s6 = std::to_string(v5.x) + std::to_string(v5.y);

			float by = 0.01f;
			float z1 = map(perlin.accumulatedOctaveNoise1D_0_1(x, octaves), 0, 1, -z_scale, +z_scale * (0.5f));
			x += by + 0.01f;
			float z2 = map(perlin.accumulatedOctaveNoise1D_0_1(x, octaves), 0, 1, -z_scale, +z_scale * (0.5f));
			x += by + 0.02f;
			float z3 = map(perlin.accumulatedOctaveNoise1D_0_1(x, octaves), 0, 1, -z_scale, +z_scale * (0.5f));
			x += by + 0.03f;
			float z4 = map(perlin.accumulatedOctaveNoise1D_0_1(x, octaves), 0, 1, -z_scale, +z_scale * (0.5f));
			x += by + 0.02f;
			float z5 = map(perlin.accumulatedOctaveNoise1D_0_1(x, octaves), 0, 1, -z_scale, +z_scale * (0.5f));
			x += by + 0.01f;
			float z6 = map(perlin.accumulatedOctaveNoise1D_0_1(x, octaves), 0, 1, -z_scale, +z_scale * (0.5f));
			x += by;

			z_values[s1] = z1;
			z_values[s2] = z2;
			z_values[s3] = z3;
			z_values[s4] = z4;
			z_values[s5] = z5;
			z_values[s6] = z6;

			start_pos.x += scale.x;
		}

		start_pos.x = -((num_cols / 2.0f) * scale.x);
		start_pos.y -= scale.y;
	}

	unsigned int index = 0;
	start_pos = glm::vec3(-((num_cols / 2.0f) * scale.x), +((num_rows / 2.0f) * scale.y), 0);
	for(unsigned i = 1; i <= num_rows; ++i) {
		for(unsigned int j = 1; j <= num_cols; ++j) {
			glm::vec2 v0 = glm::vec2(start_pos.x, start_pos.y);
			glm::vec2 v1 = glm::vec2(start_pos.x + scale.x, start_pos.y);
			glm::vec2 v2 = glm::vec2(start_pos.x, start_pos.y - scale.y);
			glm::vec2 v3 = glm::vec2(start_pos.x, start_pos.y - scale.y);
			glm::vec2 v4 = glm::vec2(start_pos.x + scale.x, start_pos.y);
			glm::vec2 v5 = glm::vec2(start_pos.x + scale.x, start_pos.y - scale.y);
			std::string s1 = std::to_string(v0.x) + std::to_string(v0.y);
			std::string s2 = std::to_string(v1.x) + std::to_string(v1.y);
			std::string s3 = std::to_string(v2.x) + std::to_string(v2.y);
			std::string s4 = std::to_string(v3.x) + std::to_string(v3.y);
			std::string s5 = std::to_string(v4.x) + std::to_string(v4.y);
			std::string s6 = std::to_string(v5.x) + std::to_string(v5.y);

			// Triangle 1
			vertices[index++] = v0.x;
			vertices[index++] = v0.y;
			vertices[index++] = z_values[s1];
			float c11 = perlin.accumulatedOctaveNoise1D_0_1(0.01f, octaves);
			float c12 = perlin.accumulatedOctaveNoise1D_0_1(0.02f, octaves);
			float c13 = perlin.accumulatedOctaveNoise1D_0_1(0.04f, octaves);
			vertices[index++] = c11;
			vertices[index++] = c12;
			vertices[index++] = c13;

			vertices[index++] = v1.x;
			vertices[index++] = v1.y;
			vertices[index++] = z_values[s2];
			float c21 = perlin.accumulatedOctaveNoise1D_0_1(0.1f, octaves);
			float c22 = perlin.accumulatedOctaveNoise1D_0_1(0.2f, octaves);
			float c23 = perlin.accumulatedOctaveNoise1D_0_1(0.3f, octaves);
			vertices[index++] = c21;
			vertices[index++] = c22;
			vertices[index++] = c23;

			vertices[index++] = v2.x;
			vertices[index++] = v2.y;
			vertices[index++] = z_values[s3];
			float c31 = perlin.accumulatedOctaveNoise1D_0_1(0.2f, octaves);
			float c32 = perlin.accumulatedOctaveNoise1D_0_1(0.1f, octaves);
			float c33 = perlin.accumulatedOctaveNoise1D_0_1(0.01f, octaves);
			vertices[index++] = c31;
			vertices[index++] = c32;
			vertices[index++] = c33;

			// Triangle 2
			vertices[index++] = v3.x;
			vertices[index++] = v3.y;
			vertices[index++] = z_values[s4];
			float c41 = perlin.accumulatedOctaveNoise1D_0_1(0.01f, octaves);
			float c42 = perlin.accumulatedOctaveNoise1D_0_1(0.02f, octaves);
			float c43 = perlin.accumulatedOctaveNoise1D_0_1(0.03f, octaves);
			vertices[index++] = c41;
			vertices[index++] = c42;
			vertices[index++] = c43;

			vertices[index++] = v4.x;
			vertices[index++] = v4.y;
			vertices[index++] = z_values[s5];
			float c51 = perlin.accumulatedOctaveNoise1D_0_1(0.1f, octaves);
			float c52 = perlin.accumulatedOctaveNoise1D_0_1(0.01f, octaves);
			float c53 = perlin.accumulatedOctaveNoise1D_0_1(0.2f, octaves);
			vertices[index++] = c51;
			vertices[index++] = c52;
			vertices[index++] = c53;

			vertices[index++] = v5.x;
			vertices[index++] = v5.y;
			vertices[index++] = z_values[s6];
			float c61 = perlin.accumulatedOctaveNoise1D_0_1(0.01f, octaves);
			float c62 = perlin.accumulatedOctaveNoise1D_0_1(0.01f, octaves);
			float c63 = perlin.accumulatedOctaveNoise1D_0_1(0.01f, octaves);
			vertices[index++] = c61;
			vertices[index++] = c62;
			vertices[index++] = c63;

			start_pos.x += scale.x;
		}

		start_pos.x = -((num_cols / 2.0f) * scale.x);
		start_pos.y -= scale.y;
	}

	printf("num_floats: %d\n", num_floats);

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * num_floats, vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	free(vertices);
}

void Terrain::draw() {
	glUseProgram(program);

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::rotate(model, (float) glm::radians(90.0f), glm::vec3(1, 0, 0));
	int model_loc = glGetUniformLocation(program, "u_model");
	glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model));

	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, num_vertices);
}
