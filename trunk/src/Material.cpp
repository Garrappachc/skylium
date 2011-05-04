/* 
 * Material.cpp
 * Copyright (C) 2011 Michał Garapich
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see http://www.gnu.org/licenses/.
 */

#include <SOIL/SOIL.h>

#include "../include/Material.h"
#include "../include/defines.h"

using namespace std;

Material::Material() :
		name(""),
		mAmbient_(0.2, 0.2, 0.2, 1.0),
		mDiffuse_(0.8, 0.8, 0.8, 1.0),
		mSpecular_(0.0, 0.0, 0.0, 1.0),
		mAlpha_(1.0),
		mShininess_(0),
		tAmbient_(0),
		tDiffuse_(0),
		tSpecular_(0),
		tAlpha_(0),
		tBump_(0) {}

Material::Material(const string &_name) :
		name(_name),
		mAmbient_(0.2, 0.2, 0.2, 1.0),
		mDiffuse_(0.8, 0.8, 0.8, 1.0),
		mSpecular_(0.0, 0.0, 0.0, 1.0),
		mAlpha_(1.0),
		mShininess_(0),
		tAmbient_(0),
		tDiffuse_(0),
		tSpecular_(0),
		tAlpha_(0),
		tBump_(0) {}

Material::~Material() {
	if (tAmbient_)
		glDeleteTextures(1, &tAmbient_);
	if (tDiffuse_)
		glDeleteTextures(1, &tDiffuse_);
	if (tSpecular_)
		glDeleteTextures(1, &tSpecular_);
	if (tAlpha_)
		glDeleteTextures(1, &tAlpha_);
	if (tBump_)
		glDeleteTextures(1, &tBump_);
}

bool
Material::loadTexture(const string &_fileName, const unsigned int &_type) {
	GLuint *texture;
	if (_type & TEXTURE_AMBIENT)
		texture = &tAmbient_;
	else if (_type & TEXTURE_DIFFUSE)
		texture = &tDiffuse_;
	else if (_type & TEXTURE_SPECULAR)
		texture = &tSpecular_;
	else if (_type & TEXTURE_ALPHA)
		texture = &tAlpha_;
	else if (_type & TEXTURE_BUMP)
		texture = &tBump_; // on to-do list
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
	return true;
}

void
Material::loadMaterial(const sColor &_param, const unsigned int &_type) {
	if (_type & MATERIAL_AMBIENT)
		mAmbient_ = _param;
	else if (_type & MATERIAL_DIFFUSE)
		mDiffuse_ = _param;
	else if (_type & MATERIAL_SPECULAR)
		mSpecular_ = _param;
}

void
Material::loadAlpha(const GLfloat &_alpha) {
	mAlpha_ = _alpha;
}

void
Material::loadShininess(const GLint &_shininess) {
	mShininess_ = _shininess;
}

bool
Material::hasAnyTexture() {
	return (tAmbient_ || tDiffuse_ || tSpecular_ || tAlpha_ || tBump_);
}

void
Material::setTexture() {
	if (tAlpha_) {
		glBindTexture(GL_TEXTURE_2D, tAlpha_);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	}
	if (tDiffuse_) {
		glBindTexture(GL_TEXTURE_2D, tDiffuse_);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	}
	if (tSpecular_) {
		glBindTexture(GL_TEXTURE_2D, tSpecular_);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	}
	if (tAmbient_) {
		glBindTexture(GL_TEXTURE_2D, tAmbient_);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	}
	if (tBump_)
		glBindTexture(GL_TEXTURE_2D, tBump_); // that has to be done
	
}

void
Material::setMaterial() {
	glMaterialfv(GL_FRONT, GL_AMBIENT, &mAmbient_[0]);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, &mDiffuse_[0]);
	glMaterialfv(GL_FRONT, GL_SPECULAR, &mSpecular_[0]);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glMateriali(GL_FRONT, GL_SHININESS, mShininess_);
}