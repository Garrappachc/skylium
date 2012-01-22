/*
     _____        _                               
    |_   _|____ _| |_ _  _ _ _ ___   __ _ __ _ __ 
      | |/ -_) \ /  _| || | '_/ -_)_/ _| '_ \ '_ \
      |_|\___/_\_\\__|\_,_|_| \___(_)__| .__/ .__/
                                       |_|  |_|   
                                       
    Copyright (C) 2011  Michał Garapich garrappachc@gmail.com

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <iostream>
#include <cstring>

#include <sys/stat.h>

#include "../include/Texture.h"

#include "../include/TextureManager.h"
#include "../include/Skylium.h"
#include "../include/ShaderDataHandler.h"

#include "../include/stb_image.h"
#include "../include/imgUtils.h"

#include "../include/defines.h"
#include "../include/config.h"
#include "../include/utils.h"

using namespace std;

Texture::Texture(const string &_fileName) :
		name(""),
		__texture(0),
		__type(GL_TEXTURE_2D),
		__wrapping(GL_CLAMP_TO_BORDER),
		__file(_fileName),
		__channels(4),
		__boss(TextureManager::GetSingletonPtr()),
		__shaders(ShaderDataHandler::GetSingleton()) {
	if ((sGlobalConfig::DEBUGGING & D_CONSTRUCTORS) == D_CONSTRUCTORS)
		cout << LOG_INFO << "Loading texture: " << _fileName << "... ";
	
	if (!__fileExists(_fileName))
		throw "File " + _fileName + " not found!";
	
	unsigned lastDot = 0;
	lastDot = _fileName.rfind('.');
		name = (lastDot != string::npos) ? _fileName.substr(0, lastDot) : _fileName;
	
	__texture = __loadTexture(_fileName);
	
	if (!__texture) {
		if ((sGlobalConfig::DEBUGGING & D_WARNINGS) == D_WARNINGS)
			cout << LOG_WARN << "Texture loading failed!";
		throw "Texture loading failed!";
	}
	
	__boss -> pushBack(this);
	
	if ((sGlobalConfig::DEBUGGING & D_CONSTRUCTORS) == D_CONSTRUCTORS)
		cout << "Done.";
}

Texture::~Texture() {
	glDeleteTextures(1, &__texture);
	checkGLErrors(AT);
	if ((sGlobalConfig::DEBUGGING & D_DESTRUCTORS) == D_DESTRUCTORS)
		cout << LOG_INFO << "Texture (\"" << name << "\") destructed.";
}

void
Texture::setTexture() {
	glBindTexture(__type, __texture);
	checkGLErrors(AT);

	__shaders.updateSampler2D("colorMap", 0);
}

bool
Texture::__fileExists(const string &_fileName) {
	struct stat buf;
	if (stat(_fileName.c_str(), &buf) == 0)
		return 1;
	else
		return 0;
}

GLuint
Texture::__loadTexture(const string &_filename) {
	/* Based on SOIL library */
	unsigned char* img;
	int width, height, channels;
	
	img = stbi_load(_filename.c_str(), &width, &height, &channels, __channels);
	
	if (!img) {
		const char* result = stbi_failure_reason();
		if ((sGlobalConfig::DEBUGGING & D_ERRORS) == D_ERRORS)
			cout << LOG_ERROR << "Error while loading texture! Stb says:\n" << result;
		return 0;
	}
	
	GLint maxTextureSize;
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize);
	
	if (sGlobalConfig::CREATE_MIPMAPS || (width > maxTextureSize) || (height > maxTextureSize)) {
		int newWidth = 1, newHeight = 1;
		while (newWidth < width)
			newWidth *= 2;
		while (newHeight < height)
			newHeight *= 2;
		
		if ((newWidth != width) || (newHeight != height)) {
			unsigned char* resampled = new unsigned char[newWidth * newHeight * __channels];
			up_scale_image(img, width, height, __channels, resampled, newWidth, newHeight);
			
			delete [] img;
			img = resampled;
			width = newWidth;
			height = newHeight;
		}
	}
	
	if ((width > maxTextureSize) || (height > maxTextureSize)) {
		unsigned char* resampled;
		int reduceBlockX = 1, reduceBlockY = 1;
		int newWidth, newHeight;
		
		if (width > maxTextureSize)
			reduceBlockX = width / maxTextureSize;
		if (height > maxTextureSize)
			reduceBlockY = height / maxTextureSize;
		
		newWidth = width / reduceBlockX;
		newHeight = height / reduceBlockY;
		
		resampled = new unsigned char[newWidth * newHeight * __channels];
		mipmap_image(img, width, height, __channels, resampled, reduceBlockX, reduceBlockY);
		
		delete [] img;
		img = resampled;
		width = newWidth;
		height = newHeight;
	}
	
	GLuint texID;
	glGenTextures(1, &texID);
	checkGLErrors(AT);
	
	if (!texID)
		return 0;
	
	unsigned originalTextureFormat = GL_RGBA, internalTextureFormat = GL_RGBA;
	
	glBindTexture(GL_TEXTURE_2D, texID);
	checkGLErrors(AT);
	
	glTexImage2D(
			GL_TEXTURE_2D, 0,
			internalTextureFormat, width, height, 0,
			originalTextureFormat, GL_UNSIGNED_BYTE, img
	);
	checkGLErrors(AT);
	
	if (sGlobalConfig::CREATE_MIPMAPS) {
		int MIPlevel = 1, MIPwidth = (width + 1) / 2, MIPheight = (height + 1) / 2;
		unsigned char* resampled = new unsigned char[MIPwidth * MIPheight * __channels];
		while (((1 << MIPlevel) <= width) || ((1 << MIPlevel) <= height)) {
			mipmap_image(img, width, height, __channels, resampled,
				(1 << MIPlevel), (1 << MIPlevel)
			);
			glTexImage2D(
				GL_TEXTURE_2D, MIPlevel,
				internalTextureFormat, MIPwidth, MIPheight, 0,
				originalTextureFormat, GL_UNSIGNED_BYTE, resampled
			);
			checkGLErrors(AT);
			
			++MIPlevel;
			MIPwidth = (MIPwidth + 1) / 2;
			MIPheight = (MIPheight + 1) / 2;
		}
		delete [] resampled;
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		checkGLErrors(AT);
	} else {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		checkGLErrors(AT);
	}
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, __wrapping);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, __wrapping);
	checkGLErrors(AT);
	
	delete [] img;
	
	return texID;
}
		


/* TODO: Bump texture (normal map). */
/* TODO: Cube map support. */


