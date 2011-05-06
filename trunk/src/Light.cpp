/* 
 * Light.cpp
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

#include "../include/Light.h"
#include "../include/defines.h"

using std::cout;
using std::endl;

Light::Light() :
		working_(true),
		ambientLight_(1.0, 1.0, 1.0, 0.0),
		diffuseLight_(1.0, 1.0, 1.0, 0.0),
		specularLight_(1.0, 1.0, 1.0, 0.0),
		lightSrc_(0.0, 0.0, 0.0) {
#ifdef __DEBUG__
	cout << LOG_INFO << "Konstruktor: Light()";
#endif
}

Light::Light(const sVec3D< GLfloat > &_position) :
		working_(true),
		ambientLight_(1.0, 1.0, 1.0, 0.0),
		diffuseLight_(1.0, 1.0, 1.0, 0.0),
		specularLight_(1.0, 1.0, 1.0, 0.0) {
	lightSrc_ = _position;
#ifdef __DEBUG__
	cout << LOG_INFO << "Konstruktor: Light(const sVec3D< GLfloat >&)";
#endif
}

Light::Light(const GLfloat &_x, const GLfloat &_y, const GLfloat &_z) :
		working_(true),
		ambientLight_(1.0, 1.0, 1.0, 0.0),
		diffuseLight_(1.0, 1.0, 1.0, 0.0),
		specularLight_(1.0, 1.0, 1.0, 0.0) {
	lightSrc_ = sVec3D< GLfloat >(_x, _y, _z);
#ifdef __DEBUG__
	cout << LOG_INFO << "Konstruktor: Light(const GLfloat&, const GLfloat&, const GLfloat&)";
#endif
}

Light::~Light() {
#ifdef __DEBUG__
	cout << LOG_INFO << "Destruktor: ~Light()";
#endif
}

void
Light::setAmbient(const sColor &_col) {
	ambientLight_ = _col;
#ifdef __DEBUG__
	cout << LOG_INFO << "Ambient colour: " << ambientLight_[0] << ", " << ambientLight_[1] << ", " << ambientLight_[2] << ", " << ambientLight_[3];
#endif
}

void
Light::setDiffuse(const sColor &_col) {
	diffuseLight_ = _col;
#ifdef __DEBUG__
	cout << LOG_INFO << "Diffuse colour: " << diffuseLight_[0] << ", " << diffuseLight_[1] << ", " << diffuseLight_[2] << ", " << diffuseLight_[3];
#endif
}

void
Light::setSpecular(const sColor &_col) {
	specularLight_ = _col;
#ifdef __DEBUG__
	cout << LOG_INFO << "Specular colour: " << specularLight_[0] << ", " << specularLight_[1] << ", " << specularLight_[2] << ", " << specularLight_[3];
#endif
}

void
Light::setSrcPos(const sVec3D< GLfloat > &_pos) {
	lightSrc_ = _pos;
#ifdef __DEBUG__
	cout << LOG_INFO << "New light position: " << lightSrc_[0] << ", " << lightSrc_[1] << ", " << lightSrc_[2];
#endif
}

void
Light::toggle() {
	working_ = !working_;
}