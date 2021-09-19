#pragma once

#include <string>
using namespace std;

class Shader {
	public:
		Shader(const string vertex_filename, const string fragment_filename); 
		bool reload_shader();
		GLint program;
	private:
		string vertex_filename;
		string fragment_filename;

		char* read_shader_source(const string filename);
		bool read_file(const char* filename, char *buffer);
};
