/*
    Object.cpp
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

#ifndef __NO_OBJECT_MANAGEMENT__

#include <iostream>
#include <fstream>
#include <sstream>
#include <map>

#include <sys/stat.h>
#include <GLee.h>

#include "../include/Object.h"
#include "../include/Vertex.h"
#include "../include/FaceComp.h"
#include "../include/Shader.h"
#include "../include/Skylium.h"
#include "../include/Texture.h"

#include "../include/defines.h"

using namespace std;

Object::Object(const string &_name) :
		 name(_name),
		 __defColor(1, 1, 1, 1),
		 __mov(0, 0, 0),
		 __rot(0, 0, 0),
		 __scale(1, 1, 1),
		 __shader(NULL),
		 __meshes(0),
		 __meshesIterator(),
		 __materials(0) {
#ifdef __DEBUG__
	cout << LOG_INFO << "Konstruktor: Object(name = \"" << _name << "\")";
#endif
}

Object::Object(const Object &_orig) :
		name(_orig.name),
		__defColor(_orig.__defColor),
		__mov(_orig.__mov),
		__rot(_orig.__rot),
		__scale(_orig.__scale),
		__shader(_orig.__shader),
		__meshes(0),
		__meshesIterator(),
		__materials(0) {
	for (unsigned i = 0; i < _orig.__meshes.size(); i++) {
		__meshes.push_back(new Mesh(*_orig.__meshes[i]));
	}
	for (unsigned i = 0; i < _orig.__materials.size(); i++) {
		__materials.push_back(new Material(*_orig.__materials[i]));
	}
}


Object::~Object() {
#ifdef __DEBUG_STRONG__
	cout << LOG_INFO << "Destruktor: ~Object(name = \"" << name << "\")";
#endif

	while (!__meshes.empty())
		delete __meshes.back(), __meshes.pop_back();
	while (!__materials.empty())
		delete __materials.back(), __materials.pop_back();
}

void
Object::show() {
	glPushMatrix();
		
		if (__shader)
			__shader -> toggle();

		glColor4f(__defColor.r, __defColor.g, __defColor.b, __defColor.a); // RGBA
		
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		
		glTranslated(__mov.x, __mov.y, __mov.z);
		
		glScaled(__scale.x, __scale.y, __scale.z);
		
		glRotated(__rot.x, 1.0, 0.0, 0.0);
		glRotated(__rot.y, 0.0, 1.0, 0.0);
		glRotated(__rot.z, 0.0, 0.0, 1.0);
		
		__meshesIterator = __meshes.begin();
		while (__meshesIterator != __meshes.end())
			(*__meshesIterator) -> show(), __meshesIterator++;
		
		if (__shader)
			__shader -> toggle();
		
	glPopMatrix();
}

void
Object::move(const GLdouble &_x, const GLdouble &_y, const GLdouble &_z) {
	__mov += sVector(_x, _y, _z);
}

void
Object::scale(const GLdouble &_x, const GLdouble &_y, const GLdouble &_z) {
	__scale += sVector(_x, _y, _z);
}

void
Object::rotate(const GLdouble &_x, const GLdouble &_y, const GLdouble &_z) {
	__rot += sVector(_x, _y, _z);
}

bool
Object::setColor(const GLfloat &_R, const GLfloat &_G, const GLfloat &_B, const GLfloat &_A) {
	if (_R < 0 || _R > 1 || _G < 0 || _G > 1 || _B < 0 || _B > 1 || _A < 0 || _A > 1)
		return false;
	
	__defColor = sColor(_R, _G, _B, _A);
	return true;
}

bool
Object::setColor(const int &_R, const int &_G, const int &_B, const GLfloat &_A) {
	if (_R < 0 || _R > 255 || _G < 0 || _G > 255 || _B < 0 || _B > 255 || _A < 0 || _A > 1)
		return false;
	
	__defColor = sColor(
			(GLfloat)_R / 255,
			(GLfloat)_G / 255,
			(GLfloat)_B / 255,
			_A
		);
	return true;
}

bool
Object::loadFromObj(const string &_objFile, const unsigned &_whatToLoad) {
#ifdef __DEBUG__
	cout << LOG_INFO << "Wczytywanie obiektu: " << name << "... ";
#endif
	
	if (!__fileExists(_objFile)) {
#ifdef __DEBUG__
		cout << LOG_WARN << name << ": nie znalazłem pliku: " << _objFile;
#endif
		return false;
	}
	
	__parseObj(_objFile, _whatToLoad);
	
	loadIntoVBO();
	
	return true;
}

void
Object::loadIntoVBO() {
	if (!Skylium::GetSingleton().isSupported("GL_ARB_vertex_buffer_object"))
		return;
	__meshesIterator = __meshes.begin();
		while (__meshesIterator != __meshes.end()) {
			if ((*__meshesIterator) -> getSizeOfVertices() > MIN_SIZE_OF_VERTEX_ARRAY &&
				(*__meshesIterator) -> getSizeOfVertices() <  MAX_SIZE_OF_VERTEX_ARRAY)
				(*__meshesIterator) -> loadIntoVbo();
			__meshesIterator++;
		}
}

Material *
Object::getMaterialByName(const string &_name) {
	for (unsigned i = 0; i < __materials.size(); i++) {
		if (__materials[i] -> name == _name)
			return __materials[i];
	}
	return (Material*)0;
}

void
Object::__parseObj(const string &_fileName, const unsigned &_whatToLoad) {
	unsigned int lastSlash = 0; // potrzebujemy lokalizacji na dysku
	while (_fileName.find('/', lastSlash+1) != string::npos) {
		lastSlash = _fileName.find('/', lastSlash+1);
	}
	string loc = _fileName.substr(0, lastSlash+1);
	
	// kilka tymczasowych zmiennych
	typedef map< Face, long, FaceComp > faceMap;
	faceMap faces;
	vector< Position > tempPos;
	vector< TexCoords > tempTex;
	vector< Normal > tempNor;
	string buffer, temp;
	long p = 0;
	GLfloat x, y, z;
	
	Mesh *current = NULL;
	
 	fstream objFile(_fileName.c_str(), ios::in);
	
	while (!objFile.eof()) {
		getline (objFile, buffer);
		
		if (buffer[0] == '#')
			continue;	// komenatrz, jedziemy dalej
			
		istringstream line(buffer);
		
		if (buffer.substr(0, 6) == "mtllib") {
			if (!(_whatToLoad & GET_MATERIAL))
				continue;
			string mtlFileName;
			line >> temp >> mtlFileName;
			__parseMtl(loc + mtlFileName);
			continue;
		} else if (buffer[0] == 'g') {
			string gName;
			if (buffer.length() <= 2) {
				if (!current)
					current = new Mesh();
				else {
					__meshes.push_back(current);
					current = NULL;
					current = new Mesh();
				}
			} else {
				name = buffer.substr(2);
				if (current)
					__meshes.push_back(current);
				current = new Mesh(name);
			}
			tempPos.clear();
			tempNor.clear();
			tempTex.clear();
			faces.clear();
		} else if (buffer.substr(0, 6) == "usemtl") {
			if (!(_whatToLoad & GET_MATERIAL))
				continue;
			if (!current)
				current = new Mesh();
			string mtlName;
			line >> temp >> mtlName;
			for (unsigned int i = 0; i < __materials.size(); i++) {
				if (__materials[i] -> name == mtlName) {
					current -> useMtl(__materials[i]);
					break;
				}
			}
		} else if (buffer[0] == 's') {
			if (!current)
				current = new Mesh();
			string tf;
			line >> temp >> tf;
			if (tf == "off")
				current -> smooth(false);
			else
				current -> smooth();
		} else if (buffer.substr(0, 2) == "v ") {
			line >> temp >> x >> y >> z;
			tempPos.push_back(Position(x, y, z));
		} else if (buffer.substr(0, 2) == "vt") {
			line >> temp >> x >> y;
			x = 1-x;
			tempTex.push_back(TexCoords(x, y));
		} else if (buffer.substr(0, 2) == "vn") {
			line >> temp >> x >> y >> z;
			tempNor.push_back(Normal(x, y, z));
		} else if (buffer[0] == 'f') {
			if ((_whatToLoad & (GET_VERTICES | GET_TEXTURE | GET_NORMALS)) == (GET_VERTICES | GET_TEXTURE | GET_NORMALS)) {
				
				char d; // delim
				Face idx; // tymczasowy face - trzy longi mamy
				line >> temp;
				for (int s = 0; s < 3; s++) {
					line >> idx.v >> d >> idx.t >> d >> idx.n; // wczytujemy dane face'a
					faceMap::iterator it = faces.find(idx); // znajdujemy index
				
					if (it == faces.end()) { // lub nie
						unsigned newVertIdx = current -> push_back( // do aktualnego mesha dorzucamy nowego face'a
								Vertex(
										Position(tempPos[idx.v - 1]), // położenie
										TexCoords(tempTex[idx.t - 1]), // koordynaty tekstury
										Normal(tempNor[idx.n - 1]) // normalna
									)
							);
						faces.insert(pair< Face, int >(idx, newVertIdx)); // wrzucamy do mapy naszego face'a
																	// i przypisujemy do niego nowo nabyty indeks
						current -> addNewIdx(newVertIdx); // wczucamy do tablicy indeksów naszego mesha nowy indeks
					} else {
						current -> addNewIdx(it -> second); // to samo, cop wyżej
					}
					p++; // zliczamy face'y
				}
			} else if ((_whatToLoad & (GET_VERTICES | GET_NORMALS)) == (GET_VERTICES | GET_NORMALS)) {
				char d;
				Face idx;
				line >> temp;
				for (int s = 0; s < 3; s++) {
					line >> idx.v >> d >> d >> idx.n;
					faceMap::iterator it = faces.find(idx);
					
					if (it == faces.end()) {
						unsigned newVertIdx = current -> push_back(
								Vertex(
										Position(tempPos[idx.v - 1]),
										TexCoords(0, 0),
										Normal(tempNor[idx.n - 1])
								)
							);
						faces.insert(pair< Face, int >(idx, newVertIdx));
						
						current -> addNewIdx(newVertIdx);
						p++;
					} else {
						current -> addNewIdx(it -> second);
						p++;
					}
				}
			}
		}
	}
	objFile.close();
	__meshes.push_back(current);
	
	if ((_whatToLoad & GET_NORMALS) == GET_NORMALS) {
		__meshesIterator = __meshes.begin();
		while (__meshesIterator != __meshes.end())
			(*__meshesIterator) -> enableNormals(), __meshesIterator++;
	}

#ifdef __DEBUG__
	cout << LOG_INFO << "Wczytano " << p << " wierzchołków.";
#endif
}

void
Object::__parseMtl(const string &_fileName) {
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
			__materials.push_back(newMtl); // Object zarządza materiałami
			current = newMtl;
		} else if (buffer.substr(0, 2) == "Ka") {
			sColor param;
			line >> temp >> param[0] >> param[1] >> param[2];
			param[3] = 1.0;
			current -> loadMaterial(param, MATERIAL_AMBIENT);
		} else if (buffer.substr(0, 2) == "Kd") {
			sColor param;
			line >> temp >> param[0] >> param[1] >> param[2];
			param[3] = 1.0;
			current -> loadMaterial(param, MATERIAL_DIFFUSE);
		} else if (buffer.substr(0, 2) == "Ks") {
			sColor param;
			line >> temp >> param[0] >> param[1] >> param[2];
			param[3] = 1.0;
			current -> loadMaterial(param, MATERIAL_SPECULAR);
		} else if (buffer.substr(0, 2) == "Ns") {
			GLint param;
			line >> temp >> param;
			current -> loadShininess(param);
		} else if (buffer.substr(0, 6) == "map_Kd") {
			string texfile;
			line >> temp >>texfile;
			if (!__fileExists("texture/" + texfile)) {
#ifdef __DEBUG__
				cout << LOG_WARN << "Nie znalazłem tekstury: " << "texture/" << texfile;
#endif
			}
			Texture *newTex = new Texture("texture/" + texfile);
			current -> appendTexture(newTex);
		}
	}
	
	mtlFile.close();
	
}

bool
Object::__fileExists(const string &_fileName) {
	struct stat buf;
	if (stat(_fileName.c_str(), &buf) == 0)
		return 1;
	else
		return 0;
}

#endif