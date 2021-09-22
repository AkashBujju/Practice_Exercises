#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <stdio.h>
#include <string.h>
#include "text.h"
#include "shader.h"

extern float map(float src, float src_start, float src_end, float des_start, float des_end);

void Texts::init(std::string font_path, int font_size, glm::vec2 window_dims) {
	this->font_size = font_size;
	this->window_dims = window_dims;
	init_freetype(&ft);
	init_font(&font, font_path.c_str(), &ft);
}

void Texts::set_text(unsigned int index, std::string text) {
	texts[index] = text;
}

void Texts::set_position(unsigned int index, glm::vec3 position) {
	positions[index] = position;
}

void Texts::set_color(unsigned int index, glm::vec3 color) {
	colors[index] = color;
}

void Texts::add_text(std::string text, glm::vec3 position, glm::vec3 color) {
	texts.push_back(text);
	colors.push_back(color);
	positions.push_back(position);
}

void Texts::draw(GLint program) {
	for(unsigned int i = 0; i < texts.size(); ++i) {
		render_text(&font, program, texts[i].c_str(), 1, positions[i], colors[i], window_dims);
	}
}

void init_freetype(FT_Library *ft) {
	if(FT_Init_FreeType(ft))
		printf("Could not init FreeType library\n");
}

void init_font(Font *font, const char* filepath, FT_Library *ft) {
	FT_Face face;
	if(FT_New_Face(*ft, filepath, 0, &face)) {
		printf("Could not load font %s\n", filepath);
		return;
	}

	FT_Set_Pixel_Sizes(face, 0, 64);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	for(unsigned int i = 0; i < 128; ++i) {
		if(FT_Load_Char(face, i, FT_LOAD_RENDER)) {
			printf("Failed to load glyph for %d\n", i);
			continue;
		}

		unsigned int texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D, 0, GL_RED,
			face->glyph->bitmap.width, face->glyph->bitmap.rows,
			0, GL_RED,
			GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer
		);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		font->characters[i].texture_id = texture;
		font->characters[i].size = glm::vec2(face->glyph->bitmap.width, face->glyph->bitmap.rows);
		font->characters[i].bearing = glm::vec2(face->glyph->bitmap_left, face->glyph->bitmap_top);
		font->characters[i].advance = face->glyph->advance.x;
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	FT_Done_Face(face);
	FT_Done_FreeType(*ft);

	glGenVertexArrays(1, &font->vao);
	glGenBuffers(1, &font->vbo);
	glBindVertexArray(font->vao);
	glBindBuffer(GL_ARRAY_BUFFER, font->vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void render_text(Font *font, unsigned int program, const char* text, float scale, glm::vec3 position, glm::vec3 color, glm::vec2 window_dims) {
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);

	int text_length = -1;
	while(text[++text_length] != '\0'){}

	glUseProgram(program);
	int textColor_loc = glGetUniformLocation(program, "textColor");
	glUniform3fv(textColor_loc, 1, glm::value_ptr(color));

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, position);
	int model_loc = glGetUniformLocation(program, "model");
	glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model));

	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(font->vao);

	float x = window_dims.x / 2.0f;
	float y = window_dims.y / 2.0f;

	float width_to_sub = 0, height_to_sub = 0;
	float tmp_x = x;
	for(int i = 0; i < text_length; ++i) {
		int ascii = (int)text[i];
		Character *character = &font->characters[ascii];
		GLfloat h = character->size.y * scale;
		height_to_sub += h;
		tmp_x += (character->advance >> 6) * scale;
	}
	width_to_sub = tmp_x - x;
	height_to_sub /= text_length;
	width_to_sub /= 2.0f;
	height_to_sub /= 2.0f;

	tmp_x = x;
	for(int i = 0; i < text_length; ++i) {
		int ascii = (int)text[i];
		Character *character = &font->characters[ascii];

		GLfloat xpos = tmp_x + character->bearing.x * scale;
		GLfloat ypos = y - (character->size.y - character->bearing.y) * scale;
		GLfloat w = character->size.x * scale;
		GLfloat h = character->size.y * scale;

		xpos -= width_to_sub;
		ypos -= height_to_sub;

		xpos = map(xpos, 0, window_dims.x, -1, +1);
		ypos = map(ypos, 0, window_dims.y, -1, +1);
		w = map(w, 0, window_dims.x, 0, +2);
		h = map(h, 0, window_dims.y, 0, +2);

		GLfloat vertices[6][4] = {
			{ xpos, ypos + h, 0.0, 0.0 },          
			{ xpos, ypos, 0.0, 1.0 },
			{ xpos + w, ypos, 1.0, 1.0 },
			{ xpos, ypos + h, 0.0, 0.0 },
			{ xpos + w, ypos, 1.0, 1.0 },
			{ xpos + w, ypos + h, 1.0, 0.0 }           
		};

		glBindTexture(GL_TEXTURE_2D, character->texture_id);
		glBindBuffer(GL_ARRAY_BUFFER, font->vbo);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		tmp_x += (character->advance >> 6) * scale;
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);
}
