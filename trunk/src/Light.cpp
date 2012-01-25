/*
     _    _      _   _                  
    | |  (_)__ _| |_| |_   __ _ __ _ __ 
    | |__| / _` | ' \  _|_/ _| '_ \ '_ \
    |____|_\__, |_||_\__(_)__| .__/ .__/
           |___/             |_|  |_|   
       
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

#include "../include/Light.h"

#include "../include/Skylium.h"
#include "../include/ShaderDataHandler.h"

#include "../include/defines.h"
#include "../include/config.h"
#include "../include/utils.h"

using namespace std;

Light::Light() :
		__working(true),
		__ambientLight(1.0, 1.0, 1.0, 0.0),
		__diffuseLight(1.0, 1.0, 1.0, 0.0),
		__specularLight(1.0, 1.0, 1.0, 0.0),
		__lightSrc(0.0, 0.0, 0.0),
		__shaders(ShaderDataHandler::GetSingleton()) {
	if ((sGlobalConfig::DEBUGGING & D_CONSTRUCTORS) == D_CONSTRUCTORS)
		cout << LOG_INFO << "Light (0, 0, 0) constructed.";
}

Light::Light(const sPosition &_position) :
		__working(true),
		__ambientLight(1.0, 1.0, 1.0, 0.0),
		__diffuseLight(1.0, 1.0, 1.0, 0.0),
		__specularLight(1.0, 1.0, 1.0, 0.0),
		__lightSrc(&_position[0]),
		__shaders(ShaderDataHandler::GetSingleton()) {
	if ((sGlobalConfig::DEBUGGING & D_CONSTRUCTORS) == D_CONSTRUCTORS)
		cout << LOG_INFO << "Light ( " << __lightSrc[0] << ", " << __lightSrc[1] << ", " << __lightSrc[2] << ") constructed.";
}

Light::Light(GLfloat _x, GLfloat _y, GLfloat _z) :
		__working(true),
		__ambientLight(1.0, 1.0, 1.0, 0.0),
		__diffuseLight(1.0, 1.0, 1.0, 0.0),
		__specularLight(1.0, 1.0, 1.0, 0.0),
		__lightSrc(_x, _y, _z),
		__shaders(ShaderDataHandler::GetSingleton()) {
	if ((sGlobalConfig::DEBUGGING & D_CONSTRUCTORS) == D_CONSTRUCTORS)
		cout << LOG_INFO << "Light( " << __lightSrc[0] << ", " << __lightSrc[1] << ", " << __lightSrc[2] << ") constructed.";

}

Light::~Light() {
	if ((sGlobalConfig::DEBUGGING & D_DESTRUCTORS) == D_DESTRUCTORS)
		cout << LOG_INFO << "Light destroyed.";
	
}

void
Light::setAmbient(const sColor &_col) {
	__ambientLight = _col;
	if ((sGlobalConfig::DEBUGGING & D_PARAMS) == D_PARAMS)
		cout << LOG_INFO << "Ambient colour set ("
				<< __ambientLight[0] << ", "
				<< __ambientLight[1] << ", "
				<< __ambientLight[2] << ", "
				<< __ambientLight[3] << ").";

}

void
Light::setDiffuse(const sColor &_col) {
	__diffuseLight = _col;
	if ((sGlobalConfig::DEBUGGING & D_PARAMS) == D_PARAMS)
		cout << LOG_INFO << "Diffuse colour set ("
				<< __diffuseLight[0] << ", "
				<< __diffuseLight[1] << ", "
				<< __diffuseLight[2] << ", "
				<< __diffuseLight[3] << ").";
}

void
Light::setSpecular(const sColor &_col) {
	__specularLight = _col;
	if ((sGlobalConfig::DEBUGGING & D_PARAMS) == D_PARAMS)
		cout << LOG_INFO << "Specular colour set ("
				<< __specularLight[0] << ", "
				<< __specularLight[1] << ", "
				<< __specularLight[2] << ", "
				<< __specularLight[3] << ").";
}

void
Light::setSrcPos(const sPosition &_pos) {
	__lightSrc = _pos;
	if ((sGlobalConfig::DEBUGGING & D_PARAMS) == D_PARAMS)
		cout << LOG_INFO << "Light source set ("
				<< __lightSrc[0] << ", "
				<< __lightSrc[1] << ", "
				<< __lightSrc[2] << ").";
}

void
Light::toggle() {
	__working = !__working;
}

void
Light::makeLight(unsigned _count) const {
	if (!__working)
		return;
	__shaders.updateData("sLightSource[" + T2String(_count) + "].ambient", __ambientLight);
	__shaders.updateData("sLightSource[" + T2String(_count) + "].diffuse", __diffuseLight);
	__shaders.updateData("sLightSource[" + T2String(_count) + "].specular", __specularLight);
	__shaders.updateData("sLightSource[" + T2String(_count) + "].position", sVec4D< GLfloat >(
			__lightSrc[0],
			__lightSrc[1],
			__lightSrc[2],
			1.0f
		));
	__shaders.updateData("sLightSource[" + T2String(_count) + "].constantAttenuation", 1.0f);
	__shaders.updateData("sLightSource[" + T2String(_count) + "].linearAttenuation", 0.0f);
	__shaders.updateData("sLightSource[" + T2String(_count) + "].quadraticAttenuation", 0.0f);
}