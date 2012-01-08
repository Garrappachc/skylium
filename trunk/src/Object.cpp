/*
      ___  _     _        _                  
     / _ \| |__ (_)___ __| |_   __ _ __ _ __ 
    | (_) | '_ \| / -_) _|  _|_/ _| '_ \ '_ \
     \___/|_.__// \___\__|\__(_)__| .__/ .__/
              |__/                |_|  |_| 
              
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
#include <fstream>
#include <sstream>
#include <map>

#include <sys/stat.h>

#include "../include/Object.h"
#include "../include/Vertex.h"
#include "../include/FaceComp.h"
#include "../include/Shader.h"
#include "../include/Skylium.h"
#include "../include/Texture.h"

#include "../include/defines.h"
#include "../include/config.h"
#include "../include/utils.h"

using namespace std;

Object::Object(const string &_name) :
		 name(_name),
		 __defColor(1, 1, 1, 1),
		 __mov(0, 0, 0),
		 __rot(0, 0, 0),
		 __scale(1, 1, 1),
		 __shader(NULL),
		 __children(0),
		__childrenIterator(),
		 __meshes(0),
		 __meshesIterator(),
		 __materials(0) {
	if ((sGlobalConfig::DEBUGGING & D_CONSTRUCTORS) == D_CONSTRUCTORS)
		cout << LOG_INFO << "Object (\"" << _name << "\") constructed.";
}

Object::Object(const Object &_orig, const string &_name) :
		name(_name),
		__defColor(_orig.__defColor),
		__mov(_orig.__mov),
		__rot(_orig.__rot),
		__scale(_orig.__scale),
		__shader(_orig.__shader),
		__children(0),
		__childrenIterator(),
		__meshes(0),
		__meshesIterator(),
		__materials(0) {
	for (unsigned i = 0; i < _orig.__meshes.size(); i++) {
		Mesh *tempMtl = new Mesh(*_orig.__meshes[i]);
		__meshes.push_back(tempMtl);
		__materials.push_back(tempMtl -> getMaterialPtr());
		
	}
	
	if ((sGlobalConfig::DEBUGGING & D_ALL_CONSTRUCTORS) == D_ALL_CONSTRUCTORS)
		cout << LOG_INFO << "Object (\"" << name << "\") constructed as a copy.";
	
}


Object::~Object() {
	//while (!__children.empty())
	//	delete __children.back(), __children.pop_back();
	while (!__meshes.empty())
		delete __meshes.back(), __meshes.pop_back();
	while (!__materials.empty())
		delete __materials.back(), __materials.pop_back();
	if ((sGlobalConfig::DEBUGGING & D_DESTRUCTORS) == D_DESTRUCTORS)
		cout << LOG_INFO << "Object (\"" << name << "\") destructed.";
}

void
Object::show() {
	glPushMatrix();
		
		if (__shader) {
			__shader -> toggle();
			__shader -> setUniform4f("sDefColor", __defColor);
		} else
			glColor4f(__defColor.r, __defColor.g, __defColor.b, __defColor.a); // RGBA
		
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		checkGLErrors(AT);
		
		glTranslated(__mov.x, __mov.y, __mov.z);
		
		glScaled(__scale.x, __scale.y, __scale.z);
		
		glRotated(__rot.x, 1.0, 0.0, 0.0);
		glRotated(__rot.y, 0.0, 1.0, 0.0);
		glRotated(__rot.z, 0.0, 0.0, 1.0);
		checkGLErrors(AT);
		
		__meshesIterator = __meshes.begin();
		while (__meshesIterator != __meshes.end())
			(*__meshesIterator) -> show(), ++__meshesIterator;
		
		if (!__children.empty()) {
			__childrenIterator = __children.begin();
			while (__childrenIterator != __children.end())
				(*__childrenIterator) -> show(), ++__childrenIterator;
		}
		
		if (__shader)
			__shader -> toggle();
		
		__wasShown = true;
		
	glPopMatrix();
	checkGLErrors(AT);
}

void
Object::move(GLdouble _x, GLdouble _y, GLdouble _z) {
	__mov += sVector(_x, _y, _z);
}

void
Object::scale(GLdouble _x, GLdouble _y, GLdouble _z) {
	__scale += sVector(_x, _y, _z);
}

void
Object::rotate(GLdouble _x, GLdouble _y, GLdouble _z) {
	__rot += sVector(_x, _y, _z);
}

bool
Object::setColor(GLfloat _R, GLfloat _G, GLfloat _B, GLfloat _A) {
	if (_R < 0 || _R > 1 || _G < 0 || _G > 1 || _B < 0 || _B > 1 || _A < 0 || _A > 1)
		return false;
	
	__defColor = sColor(_R, _G, _B, _A);
	return true;
}

bool
Object::setColor(int _R, int _G, int _B, GLfloat _A) {
	if (_R < 0 || _R > 255 || _G < 0 || _G > 255 || _B < 0 || _B > 255 || _A < 0 || _A > 1)
		return false;
	
	__defColor = sColor(
			static_cast< GLfloat >(_R) / 255,
			static_cast< GLfloat >(_G) / 255,
			static_cast< GLfloat >(_B) / 255,
			_A
		);
	return true;
}

bool
Object::loadFromObj(const string &_objFile, unsigned _whatToLoad) {
	if ((sGlobalConfig::DEBUGGING & D_PARAMS) == D_PARAMS)
		cout << LOG_INFO << "Loading object (" << name << ")... ";
	
	if (!__fileExists(_objFile)) {
		if ((sGlobalConfig::DEBUGGING & D_WARNINGS) == D_WARNINGS)
			cout << LOG_WARN << name << ": error; file not found: " << _objFile;
		return false;
	}
	
	__parseObj(_objFile, _whatToLoad);
	
	if (sGlobalConfig::USING_VBO)
		loadIntoVBO();
	
	return true;
}

void
Object::loadIntoVBO() {
	if (!Skylium::GetSingleton().isSupported("GL_ARB_vertex_buffer_object"))
		return;
	__meshesIterator = __meshes.begin();
		while (__meshesIterator != __meshes.end()) {
			if ((*__meshesIterator) -> getSizeOfVertices() > sGlobalConfig::MIN_VBO_SIZE &&
				(*__meshesIterator) -> getSizeOfVertices() <  sGlobalConfig::MAX_VBO_SIZE)
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
Object::addChild(Object* _childPtr) {
	__children.push_back(_childPtr);
}

void
Object::__parseObj(const string &_fileName, unsigned _whatToLoad) {
	unsigned int lastSlash = _fileName.rfind('/'); // we need the localization
	string loc = (lastSlash == string::npos) ? "" : _fileName.substr(0, lastSlash+1);
	
	// some temporary variables
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
			continue;	// comment, pass this
			
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
					current = new Mesh();
				}
			} else {
				name = buffer.substr(2);
				if (current)
					__meshes.push_back(current);
				current = new Mesh(name);
			}
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
				Face idx; // temporary face - we have three longs
				line >> temp;
				for (int s = 0; s < 3; ++s) {
					line >> idx.v >> d >> idx.t >> d >> idx.n; // load face's idices
					if (idx.v < 0)
						idx.v = tempPos.size() + idx.v;
					if (idx.t < 0)
						idx.t = tempTex.size() + idx.t;
					if (idx.n < 0)
						idx.n = tempNor.size() + idx.n;
					faceMap::iterator it = faces.find(idx); // find the index
				
					if (it == faces.end()) { // face not found
						unsigned newVertIdx = current -> push_back( // add new face to the actual mesh
								Vertex(
										Position(tempPos[idx.v - 1]), // location
										TexCoords(tempTex[idx.t - 1]), // texture coords
										Normal(tempNor[idx.n - 1]) // normal vector
									)
							);
						faces.insert(pair< Face, int >(idx, newVertIdx)); // add this to our face's map
																// and give him our new index
						current -> addNewIdx(newVertIdx);
					} else {
						current -> addNewIdx(it -> second);
					}
					++p; // count faces
				}
				if (!line.eof()) { // we have 4 indices
					line >> idx.v >> d >> idx.t >> d >> idx.n;
					if (idx.v < 0)
						idx.v = tempPos.size() + idx.v;
					if (idx.t < 0)
						idx.t = tempTex.size() + idx.t;
					if (idx.n < 0)
						idx.n = tempNor.size() + idx.n;
					faceMap::iterator it = faces.find(idx);
					
					if (it == faces.end()) {
						unsigned newVertIdx = current -> push_back(
								Vertex(
										Position(tempPos[idx.v - 1]),
										TexCoords(tempTex[idx.t - 1]),
										Normal(tempNor[idx.n - 1])
									)
							);
						faces.insert(pair< Face, int >(idx, newVertIdx));
						current -> addThreeIdxs(newVertIdx);
					} else {
						current -> addThreeIdxs(it -> second);
					}
					++p;
				}
			} else if ((_whatToLoad & (GET_VERTICES | GET_NORMALS)) == (GET_VERTICES | GET_NORMALS)) {
				char d;
				Face idx;
				line >> temp;
				for (int s = 0; s < 3; ++s) {
					line >> idx.v >> d >> d >> idx.n;
					if (idx.v < 0)
						idx.v = tempPos.size() + idx.v;
					if (idx.n < 0)
						idx.n = tempNor.size() + idx.n;
					idx.t = 0;
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
					} else {
						current -> addNewIdx(it -> second);
					}
					++p;
				}
				if (!line.eof()) { // we have 4 indices
					line >> idx.v >> d >> d >> idx.n;
					if (idx.v < 0)
						idx.v = tempPos.size() + idx.v;
					if (idx.n < 0)
						idx.n = tempNor.size() + idx.n;
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
						current -> addThreeIdxs(newVertIdx);
					} else {
						current -> addThreeIdxs(it -> second);
					}
					++p;
				}
			} else if ((_whatToLoad & (GET_VERTICES | GET_TEXTURE)) == (GET_VERTICES | GET_TEXTURE)) {
				char d;
				Face idx;
				line >> temp;
				for (int s = 0; s < 3; ++s) {
					line >> idx.v >> d >> idx.t;
					if (idx.v < 0)
						idx.v = tempPos.size() + idx.v;
					if (idx.t < 0)
						idx.t = tempTex.size() + idx.t;
					idx.n = 0;
					auto it = faces.find(idx);
					
					if (it == faces.end()) {
						unsigned newVertIdx = current -> push_back(
								Vertex(
										Position(tempPos[idx.v - 1]),
									  	TexCoords(tempTex[idx.t - 1]),
									  	Normal(0, 0, 0)
								)
							);
						faces.insert(pair< Face, int >(idx, newVertIdx));
					
						current -> addNewIdx(newVertIdx);
					} else {
						current -> addNewIdx(it -> second);
					}
					++p;
				}
				if (!line.eof()) { // we have 4 indices
					line >> idx.v >> d >> idx.n;
					if (idx.v < 0)
						idx.v = tempPos.size() + idx.v;
					if (idx.t < 0)
						idx.t = tempTex.size() + idx.t;
					faceMap::iterator it = faces.find(idx);
					
					if (it == faces.end()) {
						unsigned newVertIdx = current -> push_back(
								Vertex(
										Position(tempPos[idx.v - 1]),
										TexCoords(tempTex[idx.t - 1]),
										Normal(0, 0, 0)
									)
							);
						faces.insert(pair< Face, int >(idx, newVertIdx));
						current -> addThreeIdxs(newVertIdx);
					} else {
						current -> addThreeIdxs(it -> second);
					}
					++p;
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

	if ((sGlobalConfig::DEBUGGING & D_PARAMS) == D_PARAMS)
		cout << LOG_INFO << p << " vertices loaded.";
}

void
Object::__parseMtl(const string &_fileName) {
	fstream mtlFile(_fileName.c_str(), ios::in);
	
	string buffer, temp;
	
	Material *current = (Material*)NULL;
	
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
				if ((sGlobalConfig::DEBUGGING & D_WARNINGS) == D_WARNINGS)
					cout << LOG_WARN << "Texture not found: " << "texture/" << texfile;
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