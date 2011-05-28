/*
    Scene.cpp
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

#include "../include/defines.h"

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
		__isLightOn(false) {
#ifdef __DEBUG__
	cout << LOG_INFO << "Konstruktor: Scene(name = \"" << _name << "\")";
#endif
}

Scene::~Scene() {
#ifdef __DEBUG_STRONG__
	cout << LOG_INFO << "Destruktor: ~Scene(name = \"" << name << "\")";
#endif
	while (!__objectList.empty())
		delete __objectList.back(), __objectList.pop_back();
	while (!__cameraList.empty())
		delete __cameraList.back(), __cameraList.pop_back();
	while (!__lightList.empty())
		delete __lightList.back(), __lightList.pop_back();
}

void
Scene::show() {
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	
	if (__activeCamera)
		__activeCamera -> setView();
	
	if (__isLightOn)
		glEnable(GL_LIGHTING);
	
	__setLights();
	
	__setObjects();
}

Object *
Scene::createObject(const string &_name, const Object *_orig) {
	Object *newObject;
	if (_orig == NULL) // wartość domyślna
		newObject = new Object(_name);
	else {
		newObject = new Object(*_orig);
		newObject -> name = _name;
	}
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
Scene::createCamera(const GLdouble &_x, const GLdouble &_y, const GLdouble &_z, const cType &_cType) {
	Camera *newCamera = new Camera(_x, _y, _z, _cType);
	if (!__activeCamera) {
		__activeCamera = newCamera;
		__activeCamera -> setProjection(); // ustawiamy parametry "widzenia"
	}
	__cameraList.push_back(newCamera);
	return newCamera;
}

bool
Scene::setActiveCamera(Camera *_camera, const bool &_checking) {
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
Scene::createLight(const GLfloat &_x, const GLfloat &_y, const GLfloat &_z) {
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
Scene::setAmbientLight(const int &_id, const GLfloat &_R, const GLfloat &_G, const GLfloat &_B, const GLfloat &_A) {
	if (_id >= static_cast< int >(__lightList.size()) || _id < 0 || __lightList[_id] == 0) {
#ifdef __DEBUG__
		cout << LOG_WARN << name << ": nie znaleziono światła o podanym ID! (" << _id << ")";
#endif
		return false;
	}
	if (_R < 0 || _R > 1 || _G < 0 || _G > 1 || _B < 0 || _B > 1 || _A < 0 || _A > 1) {
#ifdef __DEBUG__
		cout << "Zły kolor! (" << _R << ", " << _G << ", " << _B << ", " << _A << ")";
#endif
		return false;
	}
	__lightList[_id] -> setAmbient(sColor(_R, _G, _B, _A));
	return true;
}

bool
Scene::setDiffuseLight(const int &_id, const GLfloat &_R, const GLfloat &_G, const GLfloat &_B, const GLfloat &_A) {
	if (_id >= static_cast< int >(__lightList.size()) || _id < 0 || __lightList[_id] == 0) {
#ifdef __DEBUG__
		cout << LOG_WARN << name << ": nie znaleziono światła o podanym id! (" << _id << ")";
#endif
		return false;
	}
	if (_R < 0 || _R > 1 || _G < 0 || _G > 1 || _B < 0 || _B > 1 || _A < 0 || _A > 1) {
#ifdef __DEBUG__
		cout << "Zły kolor! (" << _R << ", " << _G << ", " << _B << ", " << _A << ")";
#endif
		return false;
	}
	__lightList[_id] -> setDiffuse(sColor(_R, _G, _B, _A));
	return true;
}

bool
Scene::setSpecularLight(const int &_id, const GLfloat &_R, const GLfloat &_G, const GLfloat &_B, const GLfloat &_A) {
	if (_id >= static_cast< int >(__lightList.size()) || _id < 0 || __lightList[_id] == 0) {
#ifdef __DEBUG__
		cout << LOG_WARN << name << ": nie znaleziono światła o podanym id! (" << _id << ")";
#endif
		return false;
	}
	if (_R < 0 || _R > 1 || _G < 0 || _G > 1 || _B < 0 || _B > 1 || _A < 0 || _A > 1) {
#ifdef __DEBUG__
		cout << "Zły kolor! (" << _R << ", " << _G << ", " << _B << ", " << _A << ")";
#endif
		return false;
	}
	__lightList[_id] -> setSpecular(sColor(_R, _G, _B, _A));
	return true;
}

bool
Scene::setLightPosition(const int &_id, const GLfloat &_x, const GLfloat &_y, const GLfloat &_z) {
	if (_id >= static_cast< int >(__lightList.size()) || _id < 0 || __lightList[_id] == 0) {
#ifdef __DEBUG__
		cout << LOG_WARN << name << ": nie znaleziono światła o podanym id! (" << _id << ")";
#endif
		return false;
	}
	
	__lightList[_id] -> setSrcPos(sPosition(_x, _y, _z));
	return true;
}

bool
Scene::moveLight(const int &_id, const GLfloat &_x, const GLfloat &_y, const GLfloat &_z) {
	if (_id >= static_cast< int >(__lightList.size()) || _id < 0 || __lightList[_id] == 0) {
#ifdef __DEBUG__
		cout << LOG_WARN << name << ": nie znaleziono światła o podanym id! (" << _id << ")";
#endif
		return false;
	}
	__lightList[_id] -> __lightSrc += sVec3D< GLfloat >(_x, _y, _z);
	return true;
}

void
Scene::toggleLight() {
	__isLightOn = !__isLightOn;
#ifdef __DEBUG__
	if (__isLightOn)
		cout << LOG_INFO << "Światła włączone.";
	else
		cout << LOG_INFO << "Światła wyłączone.";
#endif
}

bool
Scene::toggleLight(const int &_id) {
	if (_id >= static_cast< int >(__lightList.size()) || _id < 0 || __lightList[_id] == 0) {
#ifdef __DEBUG__
		cout << LOG_WARN << name << ": nie znaleziono światła o podanym id! (" << _id << ")";
#endif
		return false;
	}
	__lightList[_id] -> toggle();
	return true;
}

bool
Scene::removeLight(const int &_id) {
	if (_id >= static_cast< int >(__lightList.size()) || __lightList[_id] == 0) {
#ifdef __DEBUG__
		cout << LOG_WARN << name << ": nie znaleziono światła o podanym id! (" << _id << ")";
#endif
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
		(*__objectIterator) -> show();
		__objectIterator++;
	}
}

void
Scene::__setLights() {
	short i = 0;
	__lightIterator = __lightList.begin();
	while (__lightIterator != __lightList.end()) {
		if (*__lightIterator != 0) {
			if ((*__lightIterator) -> __working )
				glEnable(lights[i]);
			else
				glDisable(lights[i]);
			glLightfv(lights[i],	GL_AMBIENT,	&(*__lightIterator)->__ambientLight[0]);
			glLightfv(lights[i],	GL_DIFFUSE,	&(*__lightIterator)->__diffuseLight[0]);
			glLightfv(lights[i],	GL_SPECULAR,	&(*__lightIterator)->__specularLight[0]);
			glLightfv(lights[i],	GL_POSITION,	&(*__lightIterator)->__lightSrc[0]);
			i++;
		}
		__lightIterator++;
	}
	
}