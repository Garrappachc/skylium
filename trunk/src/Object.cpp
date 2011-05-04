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

#ifdef _WIN32
#include <GL/GLee.h>
#else
#include <GLee.h>
#endif

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
		pGroups_(0),
		defColor_(1.0, 1.0, 1.0, 1.0),
		mov_(0, 0, 0),
		rot_(0, 0, 0), 
		scale_(0, 0, 0),
		shader_(NULL),
		materials_(0) {
	pGroups_.resize(1);
	pGroups_[0] = new PolygonGroup("", _pointers, _size);
}

Object::Object(const string &_name, const sArray &_pointers) : 
		name(_name),
		pGroups_(0),
		defColor_(1.0, 1.0, 1.0, 1.0),
		mov_(0, 0, 0),
		rot_(0, 0, 0), 
		scale_(0, 0, 0),
		shader_(NULL),
		materials_(0) {
	pGroups_.resize(1);
	pGroups_[0] = new PolygonGroup("", _pointers);
}

Object::Object(const string &_name) : 
		name(_name),
		pGroups_(0),
		defColor_(1.0, 1.0, 1.0, 1.0),
		mov_(0, 0, 0),
		rot_(0, 0, 0), 
		scale_(0, 0, 0),
		shader_(NULL),
		materials_(0) {
	pGroups_.resize(1);
	pGroups_[0] = new PolygonGroup("");
}

Object::Object() : 
		name(""),
		pGroups_(0),
		defColor_(1.0, 1.0, 1.0, 1.0),
		mov_(0, 0, 0),
		rot_(0, 0, 0), 
		scale_(0, 0, 0),
		shader_(NULL),
		materials_(0) {}

Object::~Object() {
	while (!pGroups_.empty())
		delete pGroups_.back(), pGroups_.pop_back();
	while (!materials_.empty())
		delete materials_.back(), materials_.pop_back();
}

void
Object::show() {
	glPushMatrix();
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glColor4f(defColor_[0], defColor_[1], defColor_[2], defColor_[3]);
	
	glTranslated(mov_[0], mov_[1], mov_[2]);

	glScaled(scale_[0], scale_[1], scale_[2]);

	glRotated(rot_[0], 1.0, 0, 0);
	glRotated(rot_[1], 0, 1.0, 0);
	glRotated(rot_[2], 0, 0, 1.0);
	
	if (shader_)
		shader_ -> toggle();
	
	pGroupsIterator_ = pGroups_.begin();
	while (pGroupsIterator_ != pGroups_.end())
		(*pGroupsIterator_) -> show(), pGroupsIterator_++;
	
	if (shader_)
		shader_ -> toggle();

	glPopMatrix();
}

bool
Object::move(const GLdouble &_x, const GLdouble &_y, const GLdouble &_z) {
	mov_ += sVec3D< GLdouble >(_x, _y, _z);
	return true;
}

bool
Object::scale(const GLdouble &_x, const GLdouble &_y, const GLdouble &_z) {
	scale_ += sVec3D< GLdouble >(_x, _y, _z);
	return true;
}

bool
Object::rotate(const GLdouble &_x, const GLdouble &_y, const GLdouble &_z) {
	rot_ += sVec3D< GLdouble >(_x, _y, _z);
	return true;
}

bool
Object::loadTexture(const string &_textureFile, const GLfloat *_texturePointers, const int &_size) {
	if (_textureFile == "")
		return false;

	if (!fileExists(_textureFile)) {
#ifdef __DEBUG__
		cout << LOG_WARN << name << ": plik z teksturą nie istnieje! Szukam: " << _textureFile << endl;
#endif
		return false;
	}

	if (pGroups_.size() == 0) {
#ifdef __DEBUG__
		cout << LOG_ERROR << "Nie mam do czego załadować!";
#endif
		return false;
	}
	
	if (!pGroups_[0] -> material_) {
		Material * tempMaterial = new Material();
		pGroups_[0] -> material_ = tempMaterial;
	}
	
	if (!pGroups_[0] -> material_ -> loadTexture(_textureFile, TEXTURE_DIFFUSE))
		return false;
		

#ifdef __DEBUG__
	cout << LOG_INFO << name << ": teksturę załadowano pomyślnie.\n";
#endif

	pGroups_[0] -> texCoords_.resize(_size);
	for (int i = 0; i < _size; i++) {
		pGroups_[0] -> texCoords_[i] = _texturePointers[i];
	}

	
	return true;
}

bool
Object::loadTexture(const string &_textureFile, const sArray &_texturePointers) {
	if (_textureFile == "")
		return false;

	if (!fileExists(_textureFile)) {
#ifdef __DEBUG__
		cout << LOG_WARN << name << ": plik z teksturą nie istnieje! Szukam: " << _textureFile << endl;
#endif
		return false;
	}

	if (!pGroups_[0] -> material_) {
		Material * tempMaterial = new Material();
		pGroups_[0] -> material_ = tempMaterial;
	}
	
	if (!pGroups_[0] -> material_ -> loadTexture(_textureFile, TEXTURE_DIFFUSE))
		return false;

	pGroups_[0] -> texCoords_ = _texturePointers;

#ifdef __DEBUG__
	cout << LOG_INFO << name << ": teksturę załadowano pomyślnie.\n";
#endif
	return true;
}

bool
Object::setColor(const GLfloat &_R, const GLfloat &_G, const GLfloat &_B, const GLfloat &_A) {
	if (_R < 0 || _R > 1 || _G < 0 || _G > 1 || _B < 0 || _B > 1 || _A < 0 || _A > 1)
		return false;
	defColor_[0] = _R;
	defColor_[1] = _G;
	defColor_[2] = _B;
	defColor_[3] = _A;
	
#ifdef __DEBUG__
	cout << LOG_INFO << name << ": nowy kolor: " << defColor_[0] << ", " << defColor_[1] << ", " << defColor_[2] << ", " << defColor_[3] << endl;
#endif
	
	return true;
}

bool
Object::setColor(const int &_R, const int &_G, const int &_B, const GLfloat &_T) {
	if (_R < 0 || _R > 255 || _G < 0 || _G > 255 || _B < 0 || _B > 255 || _T < 0 || _T > 1)
		return false;
	
	defColor_ = sColor((GLfloat)_R/255, (GLfloat)_G/255, (GLfloat)_B/255, _T);
	
#ifdef __DEBUG__
	cout << LOG_INFO << name << ": nowy kolor: " << defColor_[0] << ", " << defColor_[1] << ", " << defColor_[2] << endl;
#endif
	
	return true;
	
}

bool
Object::loadFromObj(const string &_objFile, const unsigned int &_whatToLoad) {
#ifdef __DEBUG__
	cout << LOG_INFO << "Wczytywanie obiektu: " << name << "... ";
	cout.flush();
#endif
	if (!fileExists(_objFile)) {
#ifdef __DEBUG__
		cout << LOG_WARN << name << ": nie znalazłem pliku: " << _objFile << endl;
#endif
		return false;
	}
	
	parseObj(_objFile, _whatToLoad);
	return true;
}

bool
Object::fileExists(const string &_fileName) {
	struct stat buf;
	if (stat(_fileName.c_str(), &buf) == 0)
		return 1;
	else
		return 0;
}

void
Object::parseObj(const string &_fileName, const unsigned int &_whatToLoad) {
	unsigned int lastSlash = 0;
	while (_fileName.find('/', lastSlash+1) != string::npos) {
		lastSlash = _fileName.find('/', lastSlash+1);
	}
	
	string loc = _fileName.substr(0, lastSlash+1);
	
	
	// kilka tymczasowych zmiennych
	vector< sVertex > vertices; // tymczasowy wektor współrzędnych wierzchołków
	vector< sCoords > texture; // tymczasowy wektor współrzędnych tekstury
	vector< sVertex > normals; // tymczasowy wektor normalnych
	string buffer;
	string temp;
	sVertex v(3);
	sCoords t(2);
	sVertex n(3);
	unsigned int currentG = 0;
	
	string mtlFileName;
	
	unsigned long int i = 0, j = 0;
	
	fstream objFile(_fileName.c_str(), ios::in);
	
	while (!objFile.eof()) {
		getline (objFile, buffer);
		
		if (buffer[0] == '#')
			continue;	// komentarz, idziemy dalej
		
		istringstream line(buffer);
		
		if (buffer.substr(0, 6) == "mtllib") {
			if (!(_whatToLoad & GET_MATERIAL))
				continue;
			line >> temp >> mtlFileName;
			parseMtl(loc + mtlFileName);
			continue;
		} else if (buffer[0] == 'g') {
			if (currentG == 0 && pGroups_[currentG] -> name == "") {
				pGroups_[currentG] -> name = buffer.substr(2);
			} else {
				PolygonGroup *tg = new PolygonGroup(buffer.substr(2));
				pGroups_.push_back(tg);
				currentG++;
			}
			i = j = 0;
			v.clear();
			t.clear();
			n.clear();
			continue;
		} else if (buffer.substr(0, 6) == "usemtl") {
			if (!(_whatToLoad & GET_MATERIAL))
				continue;
			string mtlName;
			line >> temp >> mtlName;
			for (unsigned int y = 0; y < materials_.size(); y++) {
				if (materials_[y] -> name == mtlName) {
					pGroups_[currentG] -> material_ = materials_[y];
					break;
				}
			}
		} else if (buffer[0] == 's') {
			int tf;
			line >> temp >> tf;
			if (tf == 1)
				pGroups_[currentG] -> smooth_ = true;
			else
				pGroups_[currentG] -> smooth_ = false;
		} else if (buffer.substr(0, 2) == "v ") {
			line >> temp >> v[0] >> v[1] >> v[2];
			vertices.push_back(v);
			continue;
		} else if (buffer.substr(0, 2) == "vt") {
			line >> temp >> t[0] >> t[1];
			texture.push_back(t);
			continue;
		} else if (buffer.substr(0, 2) == "vn") {
			line >> temp >> n[0] >> n[1] >> n[2];
			normals.push_back(n);
			continue;
		} else if (buffer[0] == 'f') {
			if ((_whatToLoad & (GET_VERTICES | GET_TEXTURE | GET_NORMALS)) == (GET_VERTICES | GET_TEXTURE | GET_NORMALS)) {
				char d;
				int vx, nx, tx, vy, ny, ty, vz, nz, tz;
				
				line >> temp >>
						vx >> d >> tx >> d >> nx >>
						vy >> d >> ty >> d >> ny >>
						vz >> d >> tz >> d >> nz;
				
				pGroups_[currentG] -> pointers_.resize(i + 9);
				pGroups_[currentG] -> texCoords_.resize(j + 6);
				pGroups_[currentG] -> normals_.resize(i + 9);
				
				pGroups_[currentG] -> pointers_[i] = vertices[--vx][0];
				pGroups_[currentG] -> normals_[i++] = normals[--nx][0];
				pGroups_[currentG] -> pointers_[i] = vertices[vx][1];
				pGroups_[currentG] -> normals_[i++] = normals[nx][1];
				pGroups_[currentG] -> pointers_[i] = vertices[vx][2];
				pGroups_[currentG] -> normals_[i++] = normals[nx][2];
				
				pGroups_[currentG] -> pointers_[i] = vertices[--vy][0];
				pGroups_[currentG] -> normals_[i++] = normals[--ny][0];
				pGroups_[currentG] -> pointers_[i] = vertices[vy][1];
				pGroups_[currentG] -> normals_[i++] = normals[ny][1];
				pGroups_[currentG] -> pointers_[i] = vertices[vy][2];
				pGroups_[currentG] -> normals_[i++] = normals[ny][2];
				
				pGroups_[currentG] -> pointers_[i] = vertices[--vz][0];
				pGroups_[currentG] -> normals_[i++] = normals[--nz][0];
				pGroups_[currentG] -> pointers_[i] = vertices[vz][1];
				pGroups_[currentG] -> normals_[i++] = normals[nz][1];
				pGroups_[currentG] -> pointers_[i] = vertices[vz][2];
				pGroups_[currentG] -> normals_[i++] = normals[nz][2];
				
				pGroups_[currentG] -> texCoords_[j++] = texture[--tx][0];
				pGroups_[currentG] -> texCoords_[j++] = texture[tx][1];
				pGroups_[currentG] -> texCoords_[j++] = texture[--ty][0];
				pGroups_[currentG] -> texCoords_[j++] = texture[ty][1];
				pGroups_[currentG] -> texCoords_[j++] = texture[--tz][0];
				pGroups_[currentG] -> texCoords_[j++] = texture[tz][1];
				
				continue;
			} else if ((_whatToLoad & (GET_VERTICES | GET_TEXTURE)) == (GET_VERTICES | GET_TEXTURE)) {
				char d;
				int vx, vy, vz, tx, ty, tz;
				
				line >> temp >>
						vx >> d >> tx >>
						vy >> d >> ty >>
						vz >> d >> tz;
				
				pGroups_[currentG] -> pointers_.resize(i + 9);
				pGroups_[currentG] -> texCoords_.resize(j + 6);
				
				pGroups_[currentG] -> pointers_[i++] = vertices[--vx][0];
				pGroups_[currentG] -> pointers_[i++] = vertices[vx][1];
				pGroups_[currentG] -> pointers_[i++] = vertices[vx][2];
				
				pGroups_[currentG] -> pointers_[i++] = vertices[--vy][0];
				pGroups_[currentG] -> pointers_[i++] = vertices[vy][1];
				pGroups_[currentG] -> pointers_[i++] = vertices[vy][2];
				
				pGroups_[currentG] -> pointers_[i++] = vertices[--vz][0];
				pGroups_[currentG] -> pointers_[i++] = vertices[vz][1];
				pGroups_[currentG] -> pointers_[i++] = vertices[vz][2];
				
				pGroups_[currentG] -> texCoords_[j++] = texture[--tx][0];
				pGroups_[currentG] -> texCoords_[j++] = texture[tx][1];
				pGroups_[currentG] -> texCoords_[j++] = texture[--ty][0];
				pGroups_[currentG] -> texCoords_[j++] = texture[ty][1];
				pGroups_[currentG] -> texCoords_[j++] = texture[--tz][0];
				pGroups_[currentG] -> texCoords_[j++] = texture[tz][1];
				
				continue;
			} else if ((_whatToLoad & (GET_VERTICES | GET_NORMALS)) == (GET_VERTICES | GET_NORMALS)) {
				char d;
				int vx, nx, vy, ny, vz, nz;
				
				line >> temp >>
						vx >> d >> d >> nx >>
						vy >> d >> d >> ny >>
						vz >> d >> d >> nz;
				
				pGroups_[currentG] -> pointers_.resize(i + 9);
				pGroups_[currentG] -> normals_.resize(i + 9);
				
				pGroups_[currentG] -> pointers_[i] = vertices[--vx][0];
				pGroups_[currentG] -> normals_[i++] = normals[--nx][0];
				pGroups_[currentG] -> pointers_[i] = vertices[vx][1];
				pGroups_[currentG] -> normals_[i++] = normals[nx][1];
				pGroups_[currentG] -> pointers_[i] = vertices[vx][2];
				pGroups_[currentG] -> normals_[i++] = normals[nx][2];
				
				pGroups_[currentG] -> pointers_[i] = vertices[--vy][0];
				pGroups_[currentG] -> normals_[i++] = normals[--ny][0];
				pGroups_[currentG] -> pointers_[i] = vertices[vy][1];
				pGroups_[currentG] -> normals_[i++] = normals[ny][1];
				pGroups_[currentG] -> pointers_[i] = vertices[vy][2];
				pGroups_[currentG] -> normals_[i++] = normals[ny][2];
				
				pGroups_[currentG] -> pointers_[i] = vertices[--vz][0];
				pGroups_[currentG] -> normals_[i++] = normals[--nz][0];
				pGroups_[currentG] -> pointers_[i] = vertices[vz][1];
				pGroups_[currentG] -> normals_[i++] = normals[nz][1];
				pGroups_[currentG] -> pointers_[i] = vertices[vz][2];
				pGroups_[currentG] -> normals_[i++] = normals[nz][2];
				
				continue;
			}
		}
	}
	
	objFile.close();
	
	if (_whatToLoad & GET_NORMALS) {
		for (unsigned int y = 0; y < pGroups_.size(); y++) {
				pGroups_[y] -> hasNormals_ = true;
		}
	}
}

void
Object::parseMtl(const string &_fileName) {
	fstream mtlFile(_fileName.c_str(), ios::in);
	
	string buffer, temp;
	
	Material *current;
	
	while (!mtlFile.eof()) {
		getline (mtlFile, buffer);
		
		if (buffer[0] == '#')
			continue;	// komentarz, idziemy dalej
		
		istringstream line(buffer);
		
		if (buffer.substr(0, 6) == "newmtl") {
			string newMtlName;
			line >> temp >> newMtlName;
			Material *newMtl = new Material(newMtlName);
			materials_.push_back(newMtl); // Object zarządza materiałami
			current = newMtl;
		} else if (buffer.substr(0, 2) == "Ka") {
			sColor param;
			line >> temp >> param[0] >> param[1] >> param[2];
			current -> loadMaterial(param, MATERIAL_AMBIENT);
		} else if (buffer.substr(0, 2) == "Kd") {
			sColor param;
			line >> temp >> param[0] >> param[1] >> param[2];
			current -> loadMaterial(param, MATERIAL_DIFFUSE);
		} else if (buffer.substr(0, 2) == "Ks") {
			sColor param;
			line >> temp >> param[0] >> param[1] >> param[2];
			current -> loadMaterial(param, MATERIAL_SPECULAR);
		} else if (buffer.substr(0, 2) == "Ns") {
			GLint param;
			line >> temp >> param;
			current -> loadShininess(param);
		}
	}
	
	mtlFile.close();
}