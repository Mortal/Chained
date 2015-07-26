/* vim:set ts=4 sw=4 sts=4 noet: */
#include "textures.h"
#include "constants.h"

#include <map>
#include <vector>
#include <fstream>
#include <sstream>

#include <GL/glu.h>

namespace {

std::map<int, GLuint> tex_normal;
std::map<int, GLuint> tex_blink;
std::map<int, GLuint> tex_clear;

static void read_texture(int face, int facestate) {
	std::stringstream ss;
	ss << "res/block" << face << '-';
	if (facestate == FACESTATE_NORMAL) {
		ss << '0';
	} else if (facestate == FACESTATE_BLINK) {
		ss << 'a';
	} else if (facestate == FACESTATE_CLEAR) {
		ss << 'b';
	}
	ss << "-00.raw";
	std::string image;
	size_t size = BLOCK_SRCWIDTH * BLOCK_SRCHEIGHT * 3;
	image.resize(size);
	{
		std::ifstream fin(ss.str(), std::ios::binary);
		if (!fin.is_open()) throw std::runtime_error("Failed to read texture");
		fin.read(&image[0], image.size());
	}

	bind_texture(face, facestate);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, BLOCK_SRCWIDTH, BLOCK_SRCHEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, &image[0]);
}

}

void init_textures() {
	size_t faces = 6;
	size_t states = 3;
	size_t ntextures = faces * states;
	std::vector<GLuint> tex(ntextures);
	glEnable(GL_TEXTURE_2D);
	glGenTextures(tex.size(), &tex[0]);
	size_t i = 0;
	for (int face = 1; face <= 6; ++face) {
		tex_normal[face] = tex[i++];
		tex_blink[face] = tex[i++];
		tex_clear[face] = tex[i++];
		read_texture(face, FACESTATE_NORMAL);
		read_texture(face, FACESTATE_BLINK);
		read_texture(face, FACESTATE_CLEAR);
	}
}

void bind_texture(int face, int facestate) {
	int imgidx;
	if (facestate == FACESTATE_NORMAL)
		imgidx = tex_normal[face];
	else if (facestate == FACESTATE_BLINK)
		imgidx = tex_blink[face];
	else if (facestate == FACESTATE_CLEAR)
		imgidx = tex_clear[face];
	glBindTexture(GL_TEXTURE_2D, imgidx);
}
