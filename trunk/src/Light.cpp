/* 
 * File:   Light.cpp
 * Author: Michał Garapich
 * 
 * Created on 27 april 2011, 23:48
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
}

Light::Light(const sVec3D< GLfloat > &_position) :
		working_(true),
		ambientLight_(1.0, 1.0, 1.0, 0.0),
		diffuseLight_(1.0, 1.0, 1.0, 0.0),
		specularLight_(1.0, 1.0, 1.0, 0.0) {
	lightSrc_ = _position;
}

Light::Light(const GLfloat &_x, const GLfloat &_y, const GLfloat &_z) :
		working_(true),
		ambientLight_(1.0, 1.0, 1.0, 0.0),
		diffuseLight_(1.0, 1.0, 1.0, 0.0),
		specularLight_(1.0, 1.0, 1.0, 0.0) {
	lightSrc_ = sVec3D< GLfloat >(_x, _y, _z);
}

void
Light::setAmbient(const sColor &_col) {
	ambientLight_ = _col;
#ifdef __DEBUG__
	cout << LOG_INFO << "Ambient colour: " << ambientLight_[0] << ", " << ambientLight_[1] << ", " << ambientLight_[2] << ", " << ambientLight_[3] << endl;
#endif
}

void
Light::setDiffuse(const sColor &_col) {
	diffuseLight_ = _col;
#ifdef __DEBUG__
	cout << LOG_INFO << "Diffuse colour: " << diffuseLight_[0] << ", " << diffuseLight_[1] << ", " << diffuseLight_[2] << ", " << diffuseLight_[3] << endl;
#endif
}

void
Light::setSpecular(const sColor &_col) {
	specularLight_ = _col;
#ifdef __DEBUG__
	cout << LOG_INFO << "Specular colour: " << specularLight_[0] << ", " << specularLight_[1] << ", " << specularLight_[2] << ", " << specularLight_[3] << endl;
#endif
}

void
Light::setSrcPos(const sVec3D< GLfloat > &_pos) {
	lightSrc_ = _pos;
#ifdef __DEBUG__
	cout << LOG_INFO << "New light position: " << lightSrc_[0] << ", " << lightSrc_[1] << ", " << lightSrc_[2] << endl;
#endif
}

void
Light::toggle() {
	working_ = !working_;
}