/* 
 * Object.cpp
 * Copyright (C) 2011 Michał Garapich
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see http://www.gnu.org/licenses/.
 */

#include <SOIL/SOIL.h>
#include <GLee.h>
#include <sys/stat.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

#include "../include/Object.h"
#include "../include/functions.h"
#include "../include/Shader.h"

#include "../include/defines.h"

using namespace std;

Object::Object (const string &_name, const GLfloat *_pointers, const int &_size) :
		name(_name),
		__pGroups(0),
		__defColor(1.0, 1.0, 1.0, 1.0),
		__mov(0, 0, 0),
		__rot(0, 0, 0), 
		__scale(1, 1, 1),
		__shader(NULL),
		__materials(0) {
	__pGroups.resize(1);
	__pGroups[0] = new PolygonGroup("", _pointers, _size);
#ifdef __DEBUG__
	cout << LOG_INFO << "Konstruktor: Object(const std::string&, const GLfloat&, const int&) <" << name << ">";
#endif
}

Object::Object(const string &_name, const sArray &_pointers) : 
		name(_name),
		__pGroups(0),
		__defColor(1.0, 1.0, 1.0, 1.0),
		__mov(0, 0, 0),
		__rot(0, 0, 0), 
		__scale(1, 1, 1),
		__shader(NULL),
		__materials(0) {
	__pGroups.resize(1);
	__pGroups[0] = new PolygonGroup("", _pointers);
#ifdef __DEBUG__
	cout << LOG_INFO << "Konstruktor: Object(const std::string&, const std::vector< GLfloat >) <" << name << ">";
#endif
}

Object::Object(const string &_name) : 
		name(_name),
		__pGroups(1),
		__defColor(1.0, 1.0, 1.0, 1.0),
		__mov(0, 0, 0),
		__rot(0, 0, 0), 
		__scale(1, 1, 1),
		__shader(NULL),
		__materials(0) {
	__pGroups[0] = new PolygonGroup("");
#ifdef __DEBUG__
	cout << LOG_INFO << "Konstruktor: Object(const std::string&) <" << name << ">" ;
#endif
}

Object::Object() : 
		name(""),
		__pGroups(0),
		__defColor(1.0, 1.0, 1.0, 1.0),
		__mov(0, 0, 0),
		__rot(0, 0, 0), 
		__scale(1, 1, 1),
		__shader(NULL),
		__materials(0) {
#ifdef __DEBUG__
	cout << LOG_INFO << "Konstruktor: Object()";
#endif
}

Object::~Object() {
#ifdef __DEBUG__
	cout << LOG_INFO << "Destruktor: ~Object() <" << name << ">";
#endif
	while (!__pGroups.empty())
		delete __pGroups.back(), __pGroups.pop_back();
	while (!__materials.empty())
		delete __materials.back(), __materials.pop_back();
	
}

void
Object::show() {
	glPushMatrix();
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glColor4f(__defColor[0], __defColor[1], __defColor[2], __defColor[3]);
	
	glTranslated(__mov.x, __mov.y, __mov.z);

	glScaled(__scale.x, __scale.y, __scale.z);

	glRotated(__rot.x, 1.0, 0, 0);
	glRotated(__rot.y, 0, 1.0, 0);
	glRotated(__rot.z, 0, 0, 1.0);
	
	if (__shader)
		__shader -> toggle();
	
	__pGroupsIterator = __pGroups.begin();
	while (__pGroupsIterator != __pGroups.end())
		(*__pGroupsIterator) -> show(), __pGroupsIterator++;
	
	if (__shader)
		__shader -> toggle();

	glPopMatrix();
}

void
Object::move(const GLdouble &_x, const GLdouble &_y, const GLdouble &_z) {
	__mov += sVec3D< GLdouble >(_x, _y, _z);
}

void
Object::scale(const GLdouble &_x, const GLdouble &_y, const GLdouble &_z) {
	__scale = sVec3D< GLdouble >(_x, _y, _z);
}

void
Object::rotate(const GLdouble &_x, const GLdouble &_y, const GLdouble &_z) {
	__rot += sVec3D< GLdouble >(_x, _y, _z);
}

bool
Object::loadTexture(const string &_textureFile, const GLfloat *_texturePointers, const int &_size) {
	if (_textureFile == "")
		return false;

	if (!__fileExists(_textureFile)) {
#ifdef __DEBUG__
		cout << LOG_WARN << name << ": plik z teksturą nie istnieje! Szukam: " << _textureFile;
#endif
		return false;
	}

	if (__pGroups.size() == 0) {
#ifdef __DEBUG__
		cout << LOG_ERROR << "Nie mam do czego załadować!";
#endif
		return false;
	}
	
	if (!__pGroups[0] -> __material) {
		Material * tempMaterial = new Material();
		__pGroups[0] -> __material = tempMaterial;
		__materials.push_back(tempMaterial);
	}
	
	if (!__pGroups[0] -> __material -> loadTexture(_textureFile, TEXTURE_DIFFUSE))
		return false;
		

#ifdef __DEBUG__
	cout << LOG_INFO << name << ": teksturę załadowano pomyślnie.";
#endif

	__pGroups[0] -> __texCoords.resize(_size);
	for (int i = 0; i < _size; i++) {
		__pGroups[0] -> __texCoords[i] = _texturePointers[i];
	}

	
	return true;
}

bool
Object::loadTexture(const string &_textureFile, const sArray &_texturePointers) {
	if (_textureFile == "")
		return false;

	if (!__fileExists(_textureFile)) {
#ifdef __DEBUG__
		cout << LOG_WARN << name << ": plik z teksturą nie istnieje! Szukam: " << _textureFile;
#endif
		return false;
	}

	if (!__pGroups[0] -> __material) {
		Material * tempMaterial = new Material();
		__pGroups[0] -> __material = tempMaterial;
		__materials.push_back(tempMaterial);
	}
	
	if (!__pGroups[0] -> __material -> loadTexture(_textureFile, TEXTURE_DIFFUSE))
		return false;

	__pGroups[0] -> __texCoords = _texturePointers;

#ifdef __DEBUG__
	cout << LOG_INFO << name << ": teksturę załadowano pomyślnie";
#endif
	return true;
}

bool
Object::setColor(const GLfloat &_R, const GLfloat &_G, const GLfloat &_B, const GLfloat &_A) {
	if (_R < 0 || _R > 1 || _G < 0 || _G > 1 || _B < 0 || _B > 1 || _A < 0 || _A > 1)
		return false;
	__defColor[0] = _R;
	__defColor[1] = _G;
	__defColor[2] = _B;
	__defColor[3] = _A;
	
#ifdef __DEBUG__
	cout << LOG_INFO << name << ": nowy kolor: " << __defColor[0] << ", " << __defColor[1] << ", " << __defColor[2] << ", " << __defColor[3];
#endif
	
	return true;
}

bool
Object::setColor(const int &_R, const int &_G, const int &_B, const GLfloat &_T) {
	if (_R < 0 || _R > 255 || _G < 0 || _G > 255 || _B < 0 || _B > 255 || _T < 0 || _T > 1)
		return false;
	
	__defColor = sColor((GLfloat)_R/255, (GLfloat)_G/255, (GLfloat)_B/255, _T);
	
#ifdef __DEBUG__
	cout << LOG_INFO << name << ": nowy kolor: " << __defColor[0] << ", " << __defColor[1] << ", " << __defColor[2];
#endif
	
	return true;
	
}

bool
Object::loadFromObj(const string &_objFile, const unsigned int &_whatToLoad) {
#ifdef __DEBUG__
	cout << LOG_INFO << "Wczytywanie obiektu: " << name << "... ";
	cout.flush();
#endif
	if (!__fileExists(_objFile)) {
#ifdef __DEBUG__
		cout << LOG_WARN << name << ": nie znalazłem pliku: " << _objFile;
#endif
		return false;
	}
	
	__parseObj(_objFile, _whatToLoad);
	return true;
}

bool
Object::__fileExists(const string &_fileName) {
	struct stat buf;
	if (stat(_fileName.c_str(), &buf) == 0)
		return 1;
	else
		return 0;
}
