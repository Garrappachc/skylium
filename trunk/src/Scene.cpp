/*
     ___                                 
    / __| __ ___ _ _  ___   __ _ __ _ __ 
    \__ \/ _/ -_) ' \/ -_)_/ _| '_ \ '_ \
    |___/\__\___|_||_\___(_)__| .__/ .__/
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

#include <vector>

#include "../include/Scene.h"

#include "../include/Skylium.h"
#include "../include/ShaderDataHandler.h"

#include "../include/defines.h"
#include "../include/config.h"
#include "../include/utils.h"

using namespace std;

unsigned lights[] = {
	GL_LIGHT0,
	GL_LIGHT1,
	GL_LIGHT2,
	GL_LIGHT3,
	GL_LIGHT4,
	GL_LIGHT5,
	GL_LIGHT6,
	GL_LIGHT7
};

Scene::Scene(const string &_name) :
		name(_name),
		__objectList(0),
		__objectIterator(),
		__cameraList(0),
		__activeCamera(NULL),
		__lightList(8, (Light*)NULL),
		__lightIterator(),
		__lightModelAmbient(0.2, 0.2, 0.2, 1.0),
		__shaders(ShaderDataHandler::GetSingleton()) {
	if ((sGlobalConfig::DEBUGGING & D_CONSTRUCTORS) == D_CONSTRUCTORS)
		cout << LOG_INFO << "Scene (\"" << _name << "\") constructed.";
}

Scene::~Scene() {
	if ((sGlobalConfig::DEBUGGING & D_DESTRUCTORS) == D_DESTRUCTORS)
		cout << LOG_INFO << "Scene (\"" << name << "\") destructed.";
	while (!__objectList.empty())
		delete __objectList.back(), __objectList.pop_back();
	while (!__cameraList.empty())
		delete __cameraList.back(), __cameraList.pop_back();
	while (!__lightList.empty())
		delete __lightList.back(), __lightList.pop_back();
}

void
Scene::show() {
	if (__activeCamera)
		__activeCamera -> setView();
	
	__setLights();
	
	__shaders.updateData("sLightModel.ambient", __lightModelAmbient);
	
	__setObjects();
	
	__endFrame();
}

Object *
Scene::createObject(const string &_name, Object *_parent) {
	Object *newObject;
	newObject = new Object(_name);
	
	if (_parent != NULL)
		_parent -> addChild(newObject);
	
	__objectList.push_back(newObject);
	return newObject;
}

Object *
Scene::getObjectByName(const string &_name) {
	if (_name == "")
		return NULL;
	for (unsigned i = 0; i < __objectList.size(); i++) {
		if (__objectList[i] -> name == _name)
			return __objectList[i];
	}
	return NULL;
}

Camera *
Scene::createCamera(GLdouble _x, GLdouble _y, GLdouble _z, const cType &_cType) {
	Camera *newCamera = new Camera(_x, _y, _z, _cType);
	if (!__activeCamera) {
		__activeCamera = newCamera;
		__activeCamera -> setProjection(); // sets "seeing" parameters
	}
	__cameraList.push_back(newCamera);
	return newCamera;
}

bool
Scene::setActiveCamera(Camera *_camera, bool _checking) {
	if (_checking) {
		for (unsigned i = 0; i < __cameraList.size(); i++) {
			if (__cameraList[i] == _camera) {
				__activeCamera = _camera;
				return true;
			}
		}
		return false;
	} else {
		__activeCamera = _camera;
		return true;
	}
}

short
Scene::createLight(GLfloat _x, GLfloat _y, GLfloat _z) {
	unsigned i;
	for (i = 0; i < __lightList.size(); i++) {
		if (__lightList[i] == NULL)
			break;
	}
	
	if (i > 7)
		return -1;
	
	__lightList[i] = new Light(_x, _y, _z);
	return i;
}

bool
Scene::setAmbientLight(int _id, GLfloat _R, GLfloat _G, GLfloat _B, GLfloat _A) {
	if (_id >= static_cast< int >(__lightList.size()) || _id < 0 || __lightList[_id] == 0) {
		if ((sGlobalConfig::DEBUGGING & D_WARNINGS) == D_WARNINGS)
			cout << LOG_WARN << name << ": light with given ID (" << _id << ") not found!";
		return false;
	}
	if (_R < 0 || _R > 1 || _G < 0 || _G > 1 || _B < 0 || _B > 1 || _A < 0 || _A > 1) {
		if ((sGlobalConfig::DEBUGGING & D_WARNINGS) == D_WARNINGS)
			cout << LOG_WARN << "Colour not supported! (" << _R << ", " << _G << ", " << _B << ", " << _A << ")";
		return false;
	}
	__lightList[_id] -> setAmbient(sColor(_R, _G, _B, _A));
	return true;
}

bool
Scene::setDiffuseLight(int _id, GLfloat _R, GLfloat _G, GLfloat _B, GLfloat _A) {
	if (_id >= static_cast< int >(__lightList.size()) || _id < 0 || __lightList[_id] == 0) {
		if ((sGlobalConfig::DEBUGGING & D_WARNINGS) == D_WARNINGS)
			cout << LOG_WARN << name << ": light with given ID (" << _id << ") not found!";
		return false;
	}
	if (_R < 0 || _R > 1 || _G < 0 || _G > 1 || _B < 0 || _B > 1 || _A < 0 || _A > 1) {
		if ((sGlobalConfig::DEBUGGING & D_WARNINGS) == D_WARNINGS)
			cout << LOG_WARN << "Colour not supported! (" << _R << ", " << _G << ", " << _B << ", " << _A << ")";
		return false;
	}
	__lightList[_id] -> setDiffuse(sColor(_R, _G, _B, _A));
	return true;
}

bool
Scene::setSpecularLight(int _id, GLfloat _R, GLfloat _G, GLfloat _B, GLfloat _A) {
	if (_id >= static_cast< int >(__lightList.size()) || _id < 0 || __lightList[_id] == 0) {
		if ((sGlobalConfig::DEBUGGING & D_WARNINGS) == D_WARNINGS)
			cout << LOG_WARN << name << ": light with given ID (" << _id << ") not found!";
		return false;
	}
	if (_R < 0 || _R > 1 || _G < 0 || _G > 1 || _B < 0 || _B > 1 || _A < 0 || _A > 1) {
		if ((sGlobalConfig::DEBUGGING & D_WARNINGS) == D_WARNINGS)
			cout << LOG_WARN << "Colour not supported! (" << _R << ", " << _G << ", " << _B << ", " << _A << ")";
		return false;
	}
	__lightList[_id] -> setSpecular(sColor(_R, _G, _B, _A));
	return true;
}

bool
Scene::setLightPosition(int _id, GLfloat _x, GLfloat _y, GLfloat _z) {
	if (_id >= static_cast< int >(__lightList.size()) || _id < 0 || __lightList[_id] == 0) {
		if ((sGlobalConfig::DEBUGGING & D_WARNINGS) == D_WARNINGS)
			cout << LOG_WARN << name << ": light with given ID (" << _id << ") not found!";
		return false;
	}
	
	__lightList[_id] -> setSrcPos(sPosition(_x, _y, _z));
	return true;
}

bool
Scene::moveLight(int _id, GLfloat _x, GLfloat _y, GLfloat _z) {
	if (_id >= static_cast< int >(__lightList.size()) || _id < 0 || __lightList[_id] == 0) {
		if ((sGlobalConfig::DEBUGGING & D_WARNINGS) == D_WARNINGS)
			cout << LOG_WARN << name << ": light with given ID (" << _id << ") not found!";
		return false;
	}
	__lightList[_id] -> __lightSrc += sVec3D< GLfloat >(_x, _y, _z);
	return true;
}

bool
Scene::removeLight(int _id) {
	if (_id >= static_cast< int >(__lightList.size()) || __lightList[_id] == 0) {
		if ((sGlobalConfig::DEBUGGING & D_WARNINGS) == D_WARNINGS)
			cout << LOG_WARN << name << ": light with given ID (" << _id << ") not found!";
		return false;
	}
	if (_id == -1) {
		int i;
		for (i = static_cast< int >(__lightList.size()) - 1; i >= 0; i--) {
			if (__lightList[i] != NULL)
				break;
		}
		if (i != -1) {
			delete __lightList[i];
			__lightList[i] = NULL;
		}
	} else {
		delete __lightList[_id];
		__lightList[_id] = NULL;
	}
	return true;
}

void
Scene::__setObjects() {
	if (__objectList.empty())
		return;
	
	__objectIterator = __objectList.begin();
	while(__objectIterator != __objectList.end()) {
		if (!(*__objectIterator) -> wasShown())
			(*__objectIterator) -> show();
		++__objectIterator;
	}
}

void
Scene::__setLights() {
	unsigned i = 0;
	__lightIterator = __lightList.begin();
	while (__lightIterator != __lightList.end()) {
		if (*__lightIterator != 0) {
			(*__lightIterator) -> makeLight(i);
		}
		++i;
		__lightIterator++;
	}
	
}

void
Scene::__endFrame() {
	__objectIterator = __objectList.begin();
	while (__objectIterator != __objectList.end())
		(*__objectIterator) -> endFrame(), __objectIterator++;
}