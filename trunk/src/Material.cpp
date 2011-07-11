/*
     __  __      _           _      _                
    |  \/  |__ _| |_ ___ _ _(_)__ _| |  __ _ __ _ __ 
    | |\/| / _` |  _/ -_) '_| / _` | |_/ _| '_ \ '_ \
    |_|  |_\__,_|\__\___|_| |_\__,_|_(_)__| .__/ .__/
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

#include "../include/Material.h"
#include "../include/Texture.h"

#include "../include/defines.h"
#include "../include/config.h"

using namespace std;

Material::Material(const string &_name) :
		name(_name),
		__mAmbient(0.2, 0.2, 0.2, 1.0),
		__mDiffuse(0.8, 0.8, 0.8, 1.0),
		__mSpecular(0.0, 0.0, 0.0, 1.0),
		__mAlpha(1.0),
		__mShininess(0),
		__textures(0) {
	if ((sGlobalConfig::DEBUGGING & D_CONSTRUCTORS) == D_CONSTRUCTORS)
		cout << LOG_INFO << "Konstruktor: Material(\"" << name << "\")";
}

Material::Material(const Material &_orig) :
		name(_orig.name),
		__mAmbient(0.2, 0.2, 0.2, 1.0),
		__mDiffuse(0.8, 0.8, 0.8, 1.0),
		__mSpecular(0.0, 0.0, 0.0, 1.0),
		__mAlpha(1.0),
		__mShininess(0),
		__textures(0) {
	__textures = _orig.__textures;
	if ((sGlobalConfig::DEBUGGING & D_ALL_CONSTRUCTORS) == D_ALL_CONSTRUCTORS)
		cout << LOG_INFO << "Konstruktor kopiujący: Material(\"" << name << "\")";
}

Material::~Material() {
	if ((sGlobalConfig::DEBUGGING & D_DESTRUCTORS) == D_DESTRUCTORS)
		cout << LOG_INFO << "Destruktor: ~Material(name = \"" << name << "\")";
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

void
Material::setTextures() {
	if (__textures.empty())
		return;
	__texturesIterator = __textures.begin();
	while (__texturesIterator != __textures.end())
		(*__texturesIterator) -> setTexture(), __texturesIterator++;
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
Material::appendTexture(Texture *_tex) {
	__textures.push_back(_tex);
}