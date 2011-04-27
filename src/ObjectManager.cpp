/* 
 * File:   ObjectManager.cpp
 * Author: Michał Garapich
 * 
 * Created on 2 april 2011, 12:10
 */

#include <iterator>

#include "../include/ObjectManager.h"
#include "../include/Object.h"
#include "../include/Box.h"
#include "../include/functions.h"

using namespace std;

ObjectManager::ObjectManager() {
}

ObjectManager::~ObjectManager() {
	for (int i = objectList_.size() - 1; i >= 0; i--) {
		delete objectList_[i];
	}
}

Object *
ObjectManager::newObject(const string &_name, const GLfloat *_pointers, const int &_size) {
	Object *newObject = new Object(_name, _pointers, _size);
	objectList_.push_back(newObject);
	return newObject;
}

Object *
ObjectManager::newObject(const string &_name, const sArray &_pointers) {
	Object *newObject = new Object(_name, _pointers);
	objectList_.push_back(newObject);
	return newObject;
}

Object *
ObjectManager::newObject(const std::string &_name) {
	Object *newObject = new Object(_name);
	objectList_.push_back(newObject);
	return newObject;
}

Object *
ObjectManager::newBox(const string &name) {
	Box *newBox = new Box(name);
	objectList_.push_back(newBox);
	return newBox;
}

Object*
ObjectManager::getObjectByName(const string &_name) {
	if (_name == "")
		return 0;
	for (int i = 0; i < (int)objectList_.size(); i++) {
		if (objectList_[i] -> name == _name) {
			return objectList_[i];
		}
	}
	return 0;
}

bool
ObjectManager::showAll() {
	if (objectList_.size() == 0)
		return false;

	objectIterator_ = objectList_.begin();
	while (objectIterator_ != objectList_.end()) {
	    if (!(*objectIterator_) -> show())
		return false;
	    objectIterator_++;
	}
	
	return true;
}