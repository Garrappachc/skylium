/* 
 * File:   Scene.cpp
 * Author: Michał Garapich
 * 
 * Created on 2 april 2011, 18:07
 */

#include <vector>

#include "../include/Scene.h"
#include "../include/defines.h"

using namespace std;

// Potrzebne do ustawiania świateł
unsigned int lights[] = {
	GL_LIGHT0,
	GL_LIGHT1,
	GL_LIGHT2,
	GL_LIGHT3,
	GL_LIGHT4,
	GL_LIGHT5,
	GL_LIGHT6,
	GL_LIGHT7
};

Scene::Scene() :
		name(""),
		objectList_(0),
		cameraList_(0),
		activeCamera_(NULL),
		lightList_(8, NULL),
		isLightOn_(false) {}


Scene::Scene(const string& _name) :
		name(_name),
		objectList_(0),
		cameraList_(0),
		activeCamera_(NULL),
		lightList_(8, NULL),
		isLightOn_(false) {
#ifdef __DEBUG__
	cout << LOG_INFO << "Utworzono nową scenę: " << name << endl;
#endif
}

Scene::~Scene() {
	while (!objectList_.empty())
		delete objectList_.back(), objectList_.pop_back();
	while (!cameraList_.empty())
		delete cameraList_.back(), cameraList_.pop_back();
	while (!lightList_.empty())
		delete lightList_.back(), lightList_.pop_back();
}

void
Scene::show() {
	glLoadIdentity();
	if (activeCamera_) {
		activeCamera_ -> setProjection();
		activeCamera_ -> setView();
	}
	if (isLightOn_) {
		glEnable(GL_LIGHTING);
	}
	
	setLights();

	setObjects();
}

#ifndef __NO_OBJECT_MANAGEMENT__

Object *
Scene::createObject(const string &_name, const GLfloat *_pointers, const int &_size) {
	Object *newObject = new Object(_name, _pointers, _size);
	objectList_.push_back(newObject);
	return newObject;
}

Object *
Scene::createObject(const string &_name, const sArray &_pointers) {
	Object *newObject = new Object(_name, _pointers);
	objectList_.push_back(newObject);
	return newObject;
}

Object *
Scene::createObject(const string &_name) {
	Object *newObject = new Object(_name);
	objectList_.push_back(newObject);
	return newObject;
}

Object *
Scene::createBox(const string &_name) {
	Box *newBox = new Box(_name);
	objectList_.push_back(newBox);
	return newBox;
}

Object *
Scene::getObjectByName(const string &_name) {
	if (_name == "")
		return 0;
	for (unsigned int i = 0; i < objectList_.size(); i++) {
		if (objectList_[i] -> name == _name) {
			return objectList_[i];
		}
	}
	return 0;
}

void
Scene::setObjects() {
	if (objectList_.empty())
		return;
	
	objectIterator_ = objectList_.begin();
	while(objectIterator_ != objectList_.end()) {
		(*objectIterator_) -> show();
		objectIterator_++;
	}
}

#endif

#ifndef __NO_CAMERA_MANAGEMENT__

Camera *
Scene::createCamera(const GLdouble &_x, const GLdouble &_y, const GLdouble &_z) {
	Camera * newCamera = new Camera(_x, _y, _z);
	if (!activeCamera_)
		activeCamera_ = newCamera;
	cameraList_.push_back(newCamera);
	return newCamera;
}

Camera*
Scene::getActiveCamera() {
	return activeCamera_;
}
#endif

#ifndef __NO_LIGHTNING_MANAGEMENT__

int
Scene::createLight() {
	unsigned int i;
	for (i = 0; i < lightList_.size(); i++) {
		if (lightList_[i] == NULL)
			break;
	}
	if (i > 7)
		return -1;
	lightList_[i] = new Light();
	return i;	
}

int
Scene::createLight(const sVec3D< GLfloat > &_pos) {
	unsigned int i;
	for (i = 0; i < lightList_.size(); i++) {
		if (lightList_[i] == NULL)
			break;
	}
	if (i > 7)
		return -1;
	lightList_[i] = new Light(_pos);
	return i;
}

int
Scene::createLight(const GLfloat &_x, const GLfloat &_y, const GLfloat &_z) {
	unsigned int i;
	for (i = 0; i < lightList_.size(); i++) {
		if (lightList_[i] == NULL)
			break;
	}
	if (i > 7)
		return -1;
	lightList_[i] = new Light(_x, _y, _z);
	return i;
}

bool
Scene::setAmbientLight(const int &_id, const GLfloat &_R, const GLfloat &_G, const GLfloat &_B, const GLfloat &_A) {
	if (_id >= (int)lightList_.size() || _id < 0 || lightList_[_id] == 0) {
#ifdef __DEBUG__
		cout << name << ": nie znaleziono światła o podanym id! (" << _id << ")\n";
#endif
		return false;
	}
	if (_R < 0 || _R > 1 || _G < 0 || _G > 1 || _B < 0 || _B > 1 || _A < 0 || _A > 1) {
#ifdef __DEBUG__
		cout << "Zły kolor! (" << _R << ", " << _G << ", " << _B << ", " << _A << ")\n";
#endif
		return false;
	}
	lightList_[_id] -> setAmbient(sColor(_R, _G, _B, _A));
	return true;
}

bool
Scene::setDiffuseLight(const int &_id, const GLfloat &_R, const GLfloat &_G, const GLfloat &_B, const GLfloat &_A) {
	if (_id >= (int)lightList_.size() || _id < 0 || lightList_[_id] == 0) {
#ifdef __DEBUG__
		cout << name << ": nie znaleziono światła o podanym id! (" << _id << ")\n";
#endif
		return false;
	}
	if (_R < 0 || _R > 1 || _G < 0 || _G > 1 || _B < 0 || _B > 1 || _A < 0 || _A > 1) {
#ifdef __DEBUG__
		cout << "Zły kolor! (" << _R << ", " << _G << ", " << _B << ", " << _A << ")\n";
#endif
		return false;
	}
	lightList_[_id] -> setDiffuse(sColor(_R, _G, _B, _A));
	return true;
}

bool
Scene::setSpecularLight(const int &_id, const GLfloat &_R, const GLfloat &_G, const GLfloat &_B, const GLfloat &_A) {
	if (_id >= (int)lightList_.size() || _id < 0 || lightList_[_id] == 0) {
#ifdef __DEBUG__
		cout << name << ": nie znaleziono światła o podanym id! (" << _id << ")\n";
#endif
		return false;
	}
	if (_R < 0 || _R > 1 || _G < 0 || _G > 1 || _B < 0 || _B > 1 || _A < 0 || _A > 1) {
#ifdef __DEBUG__
		cout << "Zły kolor! (" << _R << ", " << _G << ", " << _B << ", " << _A << ")\n";
#endif
		return false;
	}
	lightList_[_id] -> setSpecular(sColor(_R, _G, _B, _A));
	return true;
}

bool
Scene::setLightPosition(const int &_id, const GLfloat &_x, const GLfloat &_y, const GLfloat &_z) {
	if (_id >= (int)lightList_.size() || _id < 0 || lightList_[_id] == 0) {
#ifdef __DEBUG__
		cout << name << ": nie znaleziono światła o podanym id! (" << _id << ")\n";
#endif
		return false;
	}
	
	lightList_[_id] ->setSrcPos(sVec3D< GLfloat >(_x, _y, _z));
	return true;
}

void
Scene::toggleLight() {
	isLightOn_ = !isLightOn_;
#ifdef __DEBUG__
	if (isLightOn_)
		cout << LOG_INFO << "Światła włączone.\n";
	else
		cout << LOG_INFO << "Światła wyłączone.\n";
#endif
}

bool
Scene::toggleLight(const int &_id) {
	if (_id >= (int)lightList_.size() || _id < 0 || lightList_[_id] == 0) {
#ifdef __DEBUG__
		cout << name << ": nie znaleziono światła o podanym id! (" << _id << ")\n";
#endif
		return false;
	}
	lightList_[_id] -> toggle();
	return true;
}

bool
Scene::removeLight(const int &_id = -1) {
	if (_id >= (int)lightList_.size() || lightList_[_id] == 0) {
#ifdef __DEBUG__
		cout << name << ": nie znaleziono światła o podanym id! (" << _id << ")\n";
#endif
		return false;
	}
	if (_id == -1) {
		int i;
		for (i = (int)lightList_.size(); i >= 0; i--) {
			if (lightList_[i] != NULL)
				break;
		}
		if (i != -1) {
			delete lightList_[i];
			lightList_[i] = NULL;
		}
	} else {
		delete lightList_[_id];
		lightList_[_id] = NULL;
	}
	return true;
}

void
Scene::setLights() {
	short i = 0;
	lightIterator_ = lightList_.begin();
	while (lightIterator_ != lightList_.end()) {
		if (*lightIterator_ != 0) {
			if ((*lightIterator_)->working_ )
				glEnable(lights[i]);
			else
				glDisable(lights[i]);
			glLightfv(lights[i], GL_AMBIENT, (*lightIterator_)->ambientLight_);
			glLightfv(lights[i], GL_DIFFUSE, (*lightIterator_)->diffuseLight_);
			glLightfv(lights[i], GL_SPECULAR, (*lightIterator_)->specularLight_);
			glLightfv(lights[i], GL_POSITION, (*lightIterator_)->lightSrc_);
			i++;
		}
		lightIterator_++;
	}
	
}

#endif // __NO_LIGHTING_MANAGEMENT__