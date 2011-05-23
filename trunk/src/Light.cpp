/*
    Light.cpp
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

#ifndef __NO_LIGHTING_MANAGEMENT__

#include <iostream>

#include "../include/Light.h"
#include "../include/defines.h"

using namespace std;

Light::Light() :
		__working(true),
		__ambientLight(1.0, 1.0, 1.0, 0.0),
		__diffuseLight(1.0, 1.0, 1.0, 0.0),
		__specularLight(1.0, 1.0, 1.0, 0.0),
		__lightSrc(0.0, 0.0, 0.0) {
#ifdef __DEBUG__
	cout << LOG_INFO << "Konstruktor: Light()";
#endif
}

Light::Light(const sPosition &_position) :
		__working(true),
		__ambientLight(1.0, 1.0, 1.0, 0.0),
		__diffuseLight(1.0, 1.0, 1.0, 0.0),
		__specularLight(1.0, 1.0, 1.0, 0.0),
		__lightSrc(&_position[0]) {
#ifdef __DEBUG__
	cout << LOG_INFO << "Konstruktor: Light( " << __lightSrc[0] << ", " << __lightSrc[1] << ", " << __lightSrc[2] << ")";
#endif
}

Light::Light(const GLfloat &_x, const GLfloat &_y, const GLfloat &_z) :
		__working(true),
		__ambientLight(1.0, 1.0, 1.0, 0.0),
		__diffuseLight(1.0, 1.0, 1.0, 0.0),
		__specularLight(1.0, 1.0, 1.0, 0.0),
		__lightSrc(_x, _y, _z) {
#ifdef __DEBUG__
	cout << LOG_INFO << "Konstruktor: Light( " << __lightSrc[0] << ", " << __lightSrc[1] << ", " << __lightSrc[2] << ")";
#endif
}

Light::~Light() {
#ifdef __DEBUG__
	cout << LOG_INFO << "Destruktor: ~Light()";
#endif
}

void
Light::setAmbient(const sColor &_col) {
	__ambientLight = _col;
#ifdef __DEBUG__
	cout << LOG_INFO << "Ambient colour: " << __ambientLight[0] << ", " << __ambientLight[1] << ", " << __ambientLight[2] << ", " << __ambientLight[3];
#endif
}

void
Light::setDiffuse(const sColor &_col) {
	__diffuseLight = _col;
#ifdef __DEBUG__
	cout << LOG_INFO << "Diffuse colour: " << __diffuseLight[0] << ", " << __diffuseLight[1] << ", " << __diffuseLight[2] << ", " << __diffuseLight[3];
#endif
}

void
Light::setSpecular(const sColor &_col) {
	__specularLight = _col;
#ifdef __DEBUG__
	cout << LOG_INFO << "Specular colour: " << __specularLight[0] << ", " << __specularLight[1] << ", " << __specularLight[2] << ", " << __specularLight[3];
#endif
}

void
Light::setSrcPos(const sPosition &_pos) {
	__lightSrc = _pos;
#ifdef __DEBUG__
	cout << LOG_INFO << "New light position: " << __lightSrc[0] << ", " << __lightSrc[1] << ", " << __lightSrc[2];
#endif
}

void
Light::toggle() {
	__working = !__working;
}

#endif // __NO_LIGHTING_MANAGEMENT__