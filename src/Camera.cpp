/*
    Camera.cpp
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
#include <cmath>
#include <SDL/SDL.h>

#include "../include/Camera.h"
#include "../include/defines.h"

static const double PI = 3.1415265359;
static const double PIdiv180 = PI/180.0;
static const double PIdiv2 = PI/2;

using namespace std;

Camera::Camera(const cType &_type) :
		__type(_type),
		__fovy(45.0),
		__zNear(1.0),
		__zFar(200.0),
		__eye(0, 0, 0),
		__center(0, 0, 0),
		__up(0, 1, 0) {
#ifdef __DEBUG__
	cout << LOG_INFO << "Konstruktor: Camera()";
#endif
}

Camera::Camera(const GLdouble& _x, const GLdouble& _y, const GLdouble& _z, const cType &_type) :
		__type(_type),
		__fovy(45.0),
		__zNear(1.0),
		__zFar(200.0),
		__eye(_x, _y, _z),
		__center(0, 0, 0),
		__up(0, 1, 0) {
	if (__type == SPHERICAL) {
		__eye.normalize();
		__eye *= 20;
	}
#ifdef __DEBUG__
	cout << LOG_INFO << "Konstruktor: Camera(" << __eye.x << ", " << __eye.y << ", " << __eye.z << ")";
#endif
}

Camera::~Camera() {
#ifdef __DEBUG_STRONG__
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
	if (__type == FPP) {
		gluLookAt(
				__eye.x, __eye.y, __eye.z,
				(__center.x + __eye.x), (__center.y + __eye.y), (__center.z + __eye.z),
				__up.x, __up.y, __up.z
			);
	} else if (__type == SPHERICAL) {
		gluLookAt(
				(__eye.x + __center.x), (__eye.y + __center.y), (__eye.z + __eye.z),
				__center.x, __center.y, __center.z,
				__up.x, __up.y, __up.z
			);
	}
}

void
Camera::moveCamera(const GLdouble &movX, const GLdouble &movY, const GLdouble &movZ) {
	if (__type == FPP) {
		__eye.x += (__center.x * movZ);
		__eye.z += (__center.z * movZ);
		__eye.x += (__center.z * movX * -1);
		__eye.z += (__center.x * movX);
		__eye.y += movY / 10;
	} else if (__type == SPHERICAL) {
		__center.x += movX;
		__center.y += movY;
		__center.z += movZ;
	}
}

void
Camera::rotateCamera(const GLdouble& _x, const GLdouble& _y, const GLdouble&) {
	if (__type == FPP) {
		/* Apdejtujemy nasz kąt */
		__angle.x -= (GLdouble)(_x / 100);
		__angle.y += (GLdouble)(_y / 200);
	
		// nie chcemy wyjść poza zakres
		if (__angle.y > 90 * PIdiv180) __angle.y = 90 * PIdiv180;
		else if (__angle.y < -90 * PIdiv180) __angle.y = -90 * PIdiv180;
	
		// podnosimy nasz wektor do góry
		__center.y = sin(__angle.y);
	
		/* Obliczamy nową pozycję na płaszczyźnie x-z */
		__center.x = cos(__angle.x - 90);
		__center.z = sin(__angle.x - 90);
	
		__center.normalize();
	} else if (__type == SPHERICAL) {
		__angle.x += (GLdouble)(_x / 300);
		__angle.y -= (GLdouble)(_y / 300);
		
		__eye.x = -1 * cos(__angle.y) * sin(__angle.x - 90);
		__eye.y = sin(__angle.y);
		__eye.z = cos(__angle.y) * cos(__angle.x - 90);
		
		__eye.normalize();
		__eye *= 20;
	}
}

void
Camera::lookAt(const GLdouble &x, const GLdouble &y, const GLdouble &z) {
	__center = sVector(x, y, z);
	if (__type == FPP)
		__center.normalize();
#ifdef __DEBUG__
	cout << LOG_INFO << "LookAt: " << __center.x << ", " << __center.y << ", " << __center.z;
#endif
}


void
Camera::printInfo() {
	cout << "Eye: " << __eye.x << ", " << __eye.y << ", " << __eye.z << endl;
	cout << "LookAt: " << __center.x << ", " << __center.y << ", " << __center.z << endl;
}