#ifndef TEXT_H
#define TEXT_H

#include <ft2build.h>
#include FT_FREETYPE_H
#include <glm/glm.hpp>
#include <vector>
#include <string>

struct Character {
	unsigned int texture_id;
	unsigned int advance;
	glm::vec2 size;
	glm::vec2 bearing;
};

struct Font {
	Character characters[128];
	GLuint vao;
	GLuint vbo;
};

struct Texts {
	std::vector<std::string> texts;
	std::vector<glm::vec3> colors;
	std::vector<glm::vec3> positions;
	glm::vec2 window_dims;

	FT_Library ft;
	Font font;
	int font_size;

	void init(std::string font_path, int font_size, glm::vec2 window_dims);
	void add_text(std::string text, glm::vec3 position, glm::vec3 color);
	void set_text(unsigned int index, std::string text);
	void set_position(unsigned int index, glm::vec3 position);
	void set_color(unsigned int index, glm::vec3 colorposition);
	void draw(GLint program);
};

void init_freetype(FT_Library *ft);
void init_font(Font *font, const char* filepath, FT_Library *ft);
void render_text(Font *font, unsigned int program, const char* text, float scale, glm::vec3 position, glm::vec3 color, glm::vec2 window_dims);

#endif
