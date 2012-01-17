/*
	 __  __      _       _           __  __                                      
	|  \/  |__ _| |_ _ _(_)__ ___ __|  \/  |__ _ _ _  __ _ __ _ ___ _ _  __ _ __ _ __ 
	| |\/| / _` |  _| '_| / _/ -_|_-< |\/| / _` | ' \/ _` / _` / -_) '_|/ _| '_ \ '_ \
	|_|  |_\__,_|\__|_| |_\__\___/__/_|  |_\__,_|_||_\__,_\__, \___|_|(_)__| .__/ .__/
	                                                      |___/            |_|  |_|
      
     
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

#include <cmath>
#include <iostream>

#include "../include/MatricesManager.h"

#include "../include/config.h"
#include "../include/defines.h"

using namespace std;

static const double PI = 3.1415265359;
static const double PIdiv180 = PI/180.0;

MatricesManager::MatricesManager() {
	__modelViewMatrix.loadIdentity();
	__projectionMatrix.loadIdentity();
	
	if ((sGlobalConfig::DEBUGGING & D_CONSTRUCTORS) == D_CONSTRUCTORS)
		cout << LOG_INFO << "MatricesManager constructed.";
}

void
MatricesManager::sLookAt(const sVector& _eye, const sVector& _center, const sVector& _up) {
	/* http://pyopengl.sourceforge.net/documentation/manual/gluLookAt.3G.html */
	sVector f = _center - _eye;
	f.normalize();
	sVector s = normalOfPlane(f, _up);
	s.normalize();
	sVector u = normalOfPlane(s, f);
	f *= -1;
	
	__modelViewMatrix.loadIdentity();
	
	__modelViewMatrix.setRow(0, s);
	__modelViewMatrix.setRow(1, u);
	__modelViewMatrix.setRow(2, f);
	
	__modelViewMatrix[15] = 1;
	
	sMat16 translation;
	translation.loadIdentity();
	translation[12] = -_eye[0];
	translation[13] = -_eye[1];
	translation[14] = -_eye[2];
	
	__modelViewMatrix *= translation;
}

void
MatricesManager::sPerspective(GLdouble _fovy, GLdouble _aspect, GLdouble _zNear, GLdouble _zFar) {
	/* http://www.opengl.org/sdk/docs/man/xhtml/gluPerspective.xml */
	if (_zNear == 0)
		return;
	
	__projectionMatrix.loadIdentity();
	
	GLdouble f = 1 / (tan((_fovy * PIdiv180) / 2)); // ctg(_fovy/2)
	
	__projectionMatrix[0] = f / _aspect;
	__projectionMatrix[5] = f;
	__projectionMatrix[10] = (_zFar + _zNear) / (_zNear - _zFar);
	__projectionMatrix[14] = (2 * _zFar * _zNear) / (_zNear -_zFar);
	__projectionMatrix[11] = -1;
}

void
MatricesManager::translate(const sVector& _trans) {
	sMat16 temp;
	temp.loadIdentity();
	temp.setColumn(3, _trans);
	
	__modelViewMatrix *= temp;
}

void
MatricesManager::scale(const sVector& _scale) {
	sMat16 temp;
	temp[0] = _scale.x;
	temp[5] = _scale.y;
	temp[10] = _scale.z;
	temp[15] = 1;
	
	__modelViewMatrix *= temp;
}

void
MatricesManager::rotate(GLdouble _angle, Axis _axis) {
	sMat16 temp;
	temp.loadIdentity();
	
	double radangle = _angle * PIdiv180; // convert to radians
	
	double c = cos(radangle);
	double s = sin(radangle);
	
	double anti_c = 1 - c;
	
	sVector rot;
	switch (_axis) {
		case X:
			rot = sVector(1.0, 0.0, 0.0);
			break;
		case Y:
			rot = sVector(0.0, 1.0, 0.0);
			break;
		case Z:
			rot = sVector(0.0, 0.0, 1.0);
			break;
	}
	
	temp[0] = rot.x * rot.x * (anti_c) + c;
	temp[1] = rot.x * rot.y * (anti_c) + (rot.z * s);
	temp[2] = rot.x * rot.z * (anti_c) - (rot.y * s);
	
	temp[4] = rot.x * rot.y * (anti_c) - (rot.z * s);
	temp[5] = rot.y * rot.y * (anti_c) + c;
	temp[6] = rot.y * rot.z * (anti_c) + (rot.x * s);
	
	temp[8] = rot.x * rot.z * (anti_c) + (rot.y * s);
	temp[9] = rot.y * rot.z * (anti_c) - (rot.x * s);
	temp[10] = rot.z * rot.z * (anti_c) + c;
	
	temp[15] = 1;
	
	__modelViewMatrix *= temp;
}

void
MatricesManager::storeModelViewMatrix() {
	__MVStack.push(__modelViewMatrix);
	
	if ((sGlobalConfig::DEBUGGING & D_ALL_PARAMS) == D_ALL_PARAMS)
		cout << LOG_INFO << "MatricesManager: ModelView matrix pushed. Current stack size: " << __MVStack.size();
}

void
MatricesManager::restoreModelViewMatrix() {
	__modelViewMatrix = __MVStack.top();
	__MVStack.pop();
	
	if ((sGlobalConfig::DEBUGGING & D_ALL_PARAMS) == D_ALL_PARAMS)
		cout << LOG_INFO << "MatricesManager: ModelView matrix popped. Current stack size: " << __MVStack.size();
}

void
MatricesManager::storeProjectionMatrix() {
	__PStack.push(__projectionMatrix);
	
	if ((sGlobalConfig::DEBUGGING & D_ALL_PARAMS) == D_ALL_PARAMS)
		cout << LOG_INFO << "MatricesManager: Projection matrix pushed. Current stack size: " << __PStack.size();
}

void
MatricesManager::restoreProjectionMatrix() {
	__projectionMatrix = __PStack.top();
	__PStack.pop();
	
	if ((sGlobalConfig::DEBUGGING & D_ALL_PARAMS) == D_ALL_PARAMS)
		cout << LOG_INFO << "MatricesManager: Projection matrix popped. Current stack size: " << __PStack.size();
}
