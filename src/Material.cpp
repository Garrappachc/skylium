/*
    Material.cpp
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

#include "../include/Material.h"

#include "../include/defines.h"

using namespace std;

Material::Material(const string &_name) :
		name(_name),
		__mAmbient(0.2, 0.2, 0.2, 1.0),
		__mDiffuse(0.8, 0.8, 0.8, 1.0),
		__mSpecular(0.0, 0.0, 0.0, 1.0),
		__mAlpha(1.0),
		__mShininess(0),
		__tAmbient(0),
		__tDiffuse(0),
		__tSpecular(0),
		__tAlpha(0),
		__wrapping(GL_CLAMP_TO_BORDER) {
#ifdef __DEBUG__
	cout << LOG_INFO << "Konstruktor: Material(name = \"" << name << "\")";
#endif
}

Material::~Material() {
#ifdef __DEBUG__
	cout << LOG_INFO << "Destruktor: ~Material(name = \"" << name << "\")";
#endif
	if (__tAmbient)
		glDeleteTextures(1, &__tAmbient);
	if (__tDiffuse)
		glDeleteTextures(1, &__tDiffuse);
	if (__tSpecular)
		glDeleteTextures(1, &__tSpecular);
	if (__tAlpha)
		glDeleteTextures(1, &__tAlpha);
}

bool
Material::loadTexture(const string &_fileName, const unsigned int &_type) {
#ifdef __DEBUG__
	cout << LOG_INFO << "Ładowanie tekstury: " << _fileName << "... ";
#endif
	GLuint *texture;
	if (_type & TEXTURE_AMBIENT)
		texture = &__tAmbient;
	else if (_type & TEXTURE_DIFFUSE)
		texture = &__tDiffuse;
	else if (_type & TEXTURE_SPECULAR)
		texture = &__tSpecular;
	else if (_type & TEXTURE_ALPHA)
		texture = &__tAlpha;
	else
		return false;
	
	*texture = SOIL_load_OGL_texture(
			_fileName.c_str(),
			4,
			0,
			SOIL_FLAG_POWER_OF_TWO | SOIL_FLAG_MIPMAPS
		);
	if (!*texture) {
#ifdef __DEBUG__
		cout << LOG_WARN << "Nie udało się załadować tekstury!";
#endif
		return false;
	}
#ifdef __DEBUG__
	cout << "Załadowano.";
#endif
	return true;
}

void
Material::loadMaterial(const sColor &_param, const unsigned int &_type) {
	if (_type & MATERIAL_AMBIENT)
		__mAmbient = _param;
	else if (_type & MATERIAL_DIFFUSE)
		__mDiffuse = _param;
	else if (_type & MATERIAL_SPECULAR)
		__mSpecular = _param;
}

void
Material::loadAlpha(const GLfloat &_alpha) {
	__mAlpha = _alpha;
}

void
Material::loadShininess(const GLint &_shininess) {
	__mShininess = _shininess;
}

bool
Material::hasAnyTexture() {
	return (__tAmbient || __tDiffuse || __tSpecular || __tAlpha);
}

void
Material::setTexture() {
	if (__tAlpha) {
		glBindTexture(GL_TEXTURE_2D, __tAlpha);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, __wrapping);
	}
	if (__tDiffuse) {
		glBindTexture(GL_TEXTURE_2D, __tDiffuse);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, __wrapping);
	}
	if (__tSpecular) {
		glBindTexture(GL_TEXTURE_2D, __tSpecular);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, __wrapping);
	}
	if (__tAmbient) {
		glBindTexture(GL_TEXTURE_2D, __tAmbient);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, __wrapping);
	}
}

void
Material::setMaterial() {
	glMaterialfv(GL_FRONT, GL_AMBIENT, &__mAmbient[0]);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, &__mDiffuse[0]);
	glMaterialfv(GL_FRONT, GL_SPECULAR, &__mSpecular[0]);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glMateriali(GL_FRONT, GL_SHININESS, __mShininess);
}

void
Material::setWrapping(const GLenum &_wrapping) {
	__wrapping = _wrapping;
}