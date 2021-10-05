#include <glad/glad.h>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include "shader.h"

Shader::Shader(const string vertex_filename, const string fragment_filename) {
	this->vertex_filename = vertex_filename;
	this->fragment_filename = fragment_filename;
	this->program = -1;
}

bool Shader::reload_shader() {
	int success;
	char info_log[512];

	const size_t sz = 8192;
	char *tmp1 = (char*)calloc(sz, sizeof(char));
	const GLchar* vertex_str = (GLchar*)tmp1;
	if(!read_file(vertex_filename.c_str(), tmp1)) {
		cout << "File " << vertex_filename << " not found.\n";
		free(tmp1);
		return false;
	}

	char *tmp2 = (char*)calloc(sz, sizeof(char));
	const GLchar* fragment_str = (GLchar*)tmp2;
	if(!read_file(fragment_filename.c_str(), tmp2)) {
		cout << "File " << fragment_filename << " not found.\n";
		free(tmp2);
		return false;
	}

	bool no_errors = true;

	unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, &vertex_str, NULL);
	glCompileShader(vertex_shader);
	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertex_shader, 512, NULL, info_log);
		cout << "Vertex Shader Error: " << info_log << "\n";
		no_errors = false;
	}

	unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &fragment_str, NULL);
	glCompileShader(fragment_shader);
	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragment_shader, 512, NULL, info_log);
		cout << "Fragment Shader Error: " << info_log << "\n";
		no_errors = false;
	}

	unsigned int shader_program = glCreateProgram();
	glAttachShader(shader_program, vertex_shader);
	glAttachShader(shader_program, fragment_shader);
	glLinkProgram(shader_program);
	glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shader_program, 512, NULL, info_log);
		cout << "Shader Link Error: " << info_log << "\n";
		no_errors = false;
	}

	free(tmp1);
	free(tmp2);

	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	if(!no_errors) {
		return false;
	}

	this->program = shader_program;
	glUseProgram(this->program);

	return true;
}

bool Shader::read_file(const char* filename, char *buffer) {
	FILE *file = NULL;
	file = fopen(filename, "r");
	if(file == NULL) {
		cout << "Could not open file " << filename << "\n";
		return false;
	}

	fseek(file, 0, SEEK_END);
	const size_t filesize = ftell(file);
	fseek(file, 0, SEEK_SET);

	fread(buffer, 1, filesize, file);
	buffer[filesize] = 0;
	
	fclose(file);
	return true;
}
