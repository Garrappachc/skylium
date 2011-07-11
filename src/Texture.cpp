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

#include <SOIL/SOIL.h>
#include <sys/stat.h>

#include "../include/Texture.h"
#include "../include/TextureManager.h"

#include "../include/defines.h"
#include "../include/config.h"

using namespace std;

Texture::Texture(const string &_fileName) :
		name(""),
		__texture(0),
		__type(GL_TEXTURE_2D),
		__wrapping(GL_CLAMP_TO_BORDER),
		__file(_fileName),
		__boss(TextureManager::GetSingletonPtr()) {
	if ((sGlobalConfig::DEBUGGING & D_CONSTRUCTORS) == D_CONSTRUCTORS)
		cout << LOG_INFO << "Ładowanie tekstury: " << _fileName << "... ";
	
	if (!__fileExists(_fileName))
		throw "File " + _fileName + " was not found!";
	
	unsigned lastDot = 0;
	while (_fileName.find('.', lastDot + 1) != string::npos)
		lastDot = _fileName.find('.', lastDot + 1);
	name = _fileName.substr(0, lastDot + 1);

	__texture = SOIL_load_OGL_texture(
			_fileName.c_str(),
			4,
			0,
			SOIL_FLAG_POWER_OF_TWO | SOIL_FLAG_MIPMAPS
		);
	
	if (!__texture) {
		if ((sGlobalConfig::DEBUGGING & D_WARNINGS) == D_WARNINGS)
			cout << LOG_WARN << "Nie udało się załadować tekstury!";
		throw "Nie udało się załadować tekstury!";
	}
	
	__boss -> pushBack(this);
	
	if ((sGlobalConfig::DEBUGGING & D_CONSTRUCTORS) == D_CONSTRUCTORS)
		cout << "Załadowano.";
}

Texture::~Texture() {
	glDeleteTextures(1, &__texture);
}

void
Texture::setTexture() {
	glBindTexture(__type, __texture);
	glTexParameteri(__type, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(__type, GL_TEXTURE_WRAP_S, __wrapping);
}

bool
Texture::__fileExists(const string &_fileName) {
	struct stat buf;
	if (stat(_fileName.c_str(), &buf) == 0)
		return 1;
	else
		return 0;
}


/* TODO: Bump texture (normal map). */
/* TODO: Cube map support. */


