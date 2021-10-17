#pragma once

#define GLM_FORCE_RADIANS
#include <glad/glad.h>
#include <glm/glm.hpp>

#define MAX_LINES 25

struct Line {
	glm::vec3 positions[2];
	glm::vec2 bone_indices[2];
	glm::vec2 weights[2];
};

struct Bone {
	glm::mat4 model;
	glm::vec3 orientation;
	float angle;
};

struct Mesh {
	GLuint vao, vbo, program;
	uint32_t num_lines;
	Bone bones[MAX_LINES];
	Line lines[MAX_LINES];
	glm::mat4 mesh_model;
	glm::vec3 mesh_position;
	glm::vec3 mesh_orientation;
	float mesh_angle;
};

void add_vertex_to_mesh(Mesh *mesh, uint32_t line_index, uint32_t vertex_index, float x1, float y1, float z1, float b0, float b1, float w0, float w1);
void compile_mesh(Mesh *mesh, GLuint program, uint32_t num_vertices);
void rotate_bone_in_mesh(Mesh *mesh, uint32_t bone_index, float x, float y, float z, float degree);
void translate_bone_in_mesh(Mesh *mesh, uint32_t bone_index, float x, float y, float z);
void rotate_mesh(Mesh *mesh, float x, float y, float z, float degree);
void translate_mesh(Mesh *mesh, float x, float y, float z);
void draw_mesh(Mesh *mesh);