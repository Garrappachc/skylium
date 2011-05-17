/* 
 * Camera.cpp
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

#include <iostream>
#include <cmath>
#include <SDL/SDL.h>

#include "../include/Camera.h"
#include "../include/defines.h"

static const double PI = 3.1415265359;
static const double PIdiv180 = PI/180.0;
static const double PIdiv2 = PI/2;

using namespace std;

Camera::Camera() :
		__fovy(45.0),
		__zNear(1.0),
		__zFar(200.0),
		__eye(0, 0, 0),
		__center(0, 0, 0),
		__up(0, 1, 0),
		__mov(0, 0, 0) {
#ifdef __DEBUG__
	cout << LOG_INFO << "Konstruktor: Camera()";
#endif
}

Camera::Camera(const GLdouble& _x, const GLdouble& _y, const GLdouble& _z) :
		__fovy(45.0),
		__zNear(1.0),
		__zFar(200.0),
		__eye(_x, _y, _z),
		__center(0, 0, 0),
		__up(0, 1, 0),
		__mov(0, 0, 0) {
#ifdef __DEBUG__
	cout << LOG_INFO << "Konstruktor: Camera(const GLdouble&, const GLdouble&, const GLdouble&)";
#endif
}

Camera::~Camera() {
#ifdef __DEBUG__
	cout << LOG_INFO << "Destruktor: ~Camera()";
#endif
}

void
Camera::setProjection() {
	const SDL_VideoInfo *info = SDL_GetVideoInfo();
	__windowWidth = info -> current_w;
	__windowHeight = info -> current_h;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	GLdouble aspect = (GLdouble) __windowWidth / __windowHeight;
	glViewport(0, 0, __windowWidth, __windowHeight);
	gluPerspective(__fovy, aspect, __zNear, __zFar);
}

void
Camera::setView() {
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(
			__eye.x + __mov.x, __eye.y + __mov.y, __eye.z + __mov.z,
			__center.x + __mov.x, __center.y + __mov.y, __center.z + __mov.z,
			__up.x, __up.y, __up.z
		);
}

void
Camera::setOrtho(const sSingleCoord &_coord1, const sSingleCoord &_coord2) {
	glViewport((int)_coord1.x * __windowWidth,
			 (int)_coord1.y * __windowHeight,
			 (int)_coord2.x * __windowWidth,
			 (int)_coord2.y * __windowHeight);
	
	gluOrtho2D(_coord1.x, _coord2.x, _coord1.y, _coord2.y);
}

void
Camera::moveCamera(const GLdouble &movX, const GLdouble &movY, const GLdouble &movZ) {
	__mov += sVec3D< GLdouble >(
			movX,
			movY,
			movZ
		);
}

void
Camera::rotateCamera(const GLdouble& x, const GLdouble& y, const GLdouble&) {
	sVec3D < GLdouble > step1, step2;

	// Obracamy wokół osi Y
	if (x != 0) {
		step1.x = cos( (x + 90.0) * PIdiv180);
		step1.z = sin( x * PIdiv180);
	}

	// Obracamy wokół osi X
	GLdouble cosX = cos(y * PIdiv180);
	step2.x = step1.y * cosX;
	step2.z = step1.z * cosX;
	step2.y = sin(y * PIdiv180);
	
	__center += sVec3D< GLdouble >(step2.x * 5, step2.y, step2.z);
}

void
Camera::resetCameraPosition() {
	__mov = sVec3D< GLdouble >(0, 0, 0);
}

void
Camera::lookAt(const GLdouble &x, const GLdouble &y, const GLdouble &z) {
	__center = sVec3D< GLdouble >(x, y, z);
#ifdef __DEBUG__
	cout << LOG_INFO << "LookAt: " << __center.x << ", " << __center.y << ", " << __center.z;
#endif
}