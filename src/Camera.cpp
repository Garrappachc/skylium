/* 
 * File:   Camera.cpp
 * Author: Michał Garapich
 * 
 * Created on 1 april 2011, 18:13
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
		fovy_(45.0),
		zNear_(1.0),
		zFar_(200.0),
		Yaxis_(0.0) {
	eye_ = sVec3D < GLdouble > (0, 0, 0);
	center_ = sVec3D < GLdouble > (0, 0, 0);
	up_ = sVec3D < GLdouble> (0, 1, 0);
	mov_ = sVec3D < GLdouble > (0, 0, 0);

	const SDL_VideoInfo *info = SDL_GetVideoInfo();
	windowWidth_ = info -> current_w;
	windowHeight_ = info -> current_h;
}

Camera::Camera(const GLdouble& x, const GLdouble& y, const GLdouble& z) :
		fovy_(45.0),
		zNear_(1.0),
		zFar_(1000.0),
		Yaxis_(0.0) {
	eye_[0] = x;
	eye_[1] = y;
	eye_[2] = z;
	center_ = sVec3D < GLdouble > (0, 0, 0);
	up_ = sVec3D < GLdouble> (0, 1, 0);
	mov_ = sVec3D < GLdouble > (0, 0, 0);

	const SDL_VideoInfo *info = SDL_GetVideoInfo();
	windowWidth_ = info -> current_w;
	windowHeight_ = info -> current_h;
}

void
Camera::setProjection() {
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	GLdouble aspect = (GLdouble) windowWidth_ / windowHeight_;
	gluPerspective(fovy_, aspect, zNear_, zFar_);
}

void
Camera::setView() {
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(
			eye_[0] + mov_[0], eye_[1] + mov_[1], eye_[2] + mov_[2],
			center_[0] + mov_[0], center_[1] + mov_[1], center_[2] + mov_[2],
			up_[0], up_[1], up_[2]
		);
}

void
Camera::moveCamera(const GLdouble &movX, const GLdouble &movY, const GLdouble &movZ) {
	mov_ += sVec3D< GLdouble >(
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
		step1[0] = cos( (x + 90.0) * PIdiv180);
		step1[2] = sin( x * PIdiv180);
	}

	// Obracamy wokół osi X
	GLdouble cosX = cos(y * PIdiv180);
	step2[0] = step1[1] * cosX;
	step2[2] = step1[2] * cosX;
	step2[1] = sin(y * PIdiv180);

	Yaxis_ += y;
	
	center_ += sVec3D< GLdouble >(step2[0] * 5, step2[1], step2[2]);
}

void
Camera::resetCameraPosition() {
	mov_ = sVec3D< GLdouble >(0, 0, 0);
}

void
Camera::lookAt(const GLdouble &x, const GLdouble &y, const GLdouble &z) {
	center_ = sVec3D< GLdouble >(x, y, z);
#ifdef __DEBUG__
	cout << LOG_INFO << "LookAt: " << center_[0] << ", " << center_[1] << ", " << center_[2] << endl;
#endif
}