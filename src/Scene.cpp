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

Scene::Scene(const string &_name) :
		name(_name),
		__objectList(0),
		__objectIterator(),
		__cameraList(0),
		__activeCamera(NULL),
		__lightList(0),
		__lightIterator(),
		__lightModelAmbient({0.2, 0.2, 0.2, 1.0}),
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
Scene::createCamera(GLfloat _x, GLfloat _y, GLfloat _z, const cType &_cType) {
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

Light *
Scene::createLight(GLfloat _x, GLfloat _y, GLfloat _z) {
	Light* newLight = new Light(sVector3D({_x, _y, _z}));
	__lightList.push_back(newLight);
	return newLight;
}

void
Scene::removeLight(Light* _light) {
	for (auto it = __lightList.begin(); it != __lightList.end(); ++it) {
		if ((*it) == _light) {
			__lightList.erase(it);
			delete (*it);
			return;
		}
	}
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
	for (Light*& l: __lightList) {
		l -> makeLight(i);
		++i;
	}
	/*
	 * TODO: Tell shader how many lights we have.
	 */
}

void
Scene::__endFrame() {
	__objectIterator = __objectList.begin();
	while (__objectIterator != __objectList.end())
		(*__objectIterator) -> endFrame(), __objectIterator++;
}