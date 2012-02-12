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
#include <algorithm>

#include <sys/stat.h>

#include "../include/Object.h"
#include "../include/Vertex.h"
#include "../include/Shader.h"
#include "../include/Skylium.h"
#include "../include/Texture.h"
#include "../include/MatricesManager.h"
#include "../include/ShaderDataHandler.h"
#include "../include/TextureManager.h"
#include "../include/Timer.h"

#include "../include/defines.h"
#include "../include/config.h"
#include "../include/utils.h"

#define get16bits(d) (*((const uint16_t *) (d)))

using namespace std;

static const double PI = 3.1415265359;
static const double PIdiv180 = PI/180.0;

enum {
	GET_TEXTURE	= 1,
	GET_NORMALS	= 2
};

/* For std::map in obj parsing */
struct Index {
	long v;
	long t;
	long n;
	
	Index() : v(0), t(0), n(0) {}
	Index(const long& _v, const long& _t, const long& _n) :
			v(_v), t(_t), n(_n) {}
};

bool operator ==(const Index& _a, const Index& _b) {
	return (_a.v == _b.v) && (_a.t == _b.t) && (_a.n == _b.n);
}

struct HashMyIndex {
	/* This is our hashing function.
	 * See http://www.azillionmonkeys.com/qed/hash.html */
	size_t operator ()(const Index& _idx) const {
		char* x = (char*)&_idx;
		uint32_t len = sizeof(Index);
		uint32_t hash = len, tmp;
		int rem = len & 3;
		
		len >>= 2;
		
		for (; len > 0; len--) {
			hash += get16bits(x);
			tmp = (get16bits(x + 2) << 11) ^ hash;
			hash = (hash << 16) ^ tmp;
			x += 2 * sizeof(uint16_t);
			hash += hash >> 1;
		}
		
		switch(rem) {
		case 3:
			hash += get16bits(x);
			hash ^= hash << 16;
			hash ^= x[sizeof(uint16_t)] << 18;
			hash += hash >> 11;
			break;
		case 2:
			hash += get16bits(x);
			hash ^= hash << 11;
			hash += hash >> 17;
			break;
		case 1:
			hash += *x;
			hash ^= hash << 10;
			hash += hash >> 1;
		}
		
		hash ^= hash << 3;
		hash += hash >> 5;
		hash ^= hash << 4;
		hash += hash >> 17;
		hash ^= hash << 25;
		hash += hash >> 6;
		
		return hash;
	}
	
};


Object::Object(const string &_name) :
		name(_name),
		__defColor({1, 1, 1, 1}),
		__mov({0, 0, 0}),
		__rot({0, 0, 0}),
		__scale({1, 1, 1}),
		__shader(NULL),
		__children(0),
		__meshes(0),
		__materials(0),
		__content(0),
		__matrices(MatricesManager::GetSingleton()),
		__shaders(ShaderDataHandler::GetSingleton()) {
	if ((sGlobalConfig::DEBUGGING & D_CONSTRUCTORS) == D_CONSTRUCTORS)
		cout << LOG_INFO << "Object (\"" << _name << "\") constructed.";
}

Object::~Object() {
	while (!__children.empty())
		delete __children.back(), __children.pop_back();
	
	for (auto it = __meshes.begin(); it != __meshes.end(); ++it)
		delete it -> second;
	
	for (auto it = __materials.begin(); it != __materials.end(); ++it)
		delete it -> second;
	
	if ((sGlobalConfig::DEBUGGING & D_DESTRUCTORS) == D_DESTRUCTORS)
		cout << LOG_INFO << "Object (\"" << name << "\") destructed.";
}

void
Object::show() {
	__matrices.storeModelViewMatrix();
		__matrices.translate(__mov);
		__matrices.scale(__scale);
		__matrices.rotate(__rot.x, X);
		__matrices.rotate(__rot.y, Y);
		__matrices.rotate(__rot.z, Z);
		
		__shaders.updateData("sDefColor", __defColor);
		
		__shader -> toggle();
		__shaders.openStream(__shader);
		
#ifdef __DEBUG__
		cout << "\n  Rendering object...";
		cout.flush();
#endif // __DEBUG__
		
		for (auto it = __meshes.begin(); it != __meshes.end(); ++it) {			
			
			it -> second -> show();
		}

		__shaders.closeStream();
		__shader -> toggle();
		
		for (Object*& child: __children)
			child -> show();
		
		__wasShown = true;
		
	__matrices.restoreModelViewMatrix();
}

void
Object::move(GLfloat _x, GLfloat _y, GLfloat _z) {
	__mov += sVector3D( {_x, _y, _z} );
}

void
Object::scale(GLfloat _x, GLfloat _y, GLfloat _z) {
	__scale += sVector3D( {_x, _y, _z} );
}

void
Object::rotate(GLfloat _x, GLfloat _y, GLfloat _z) {
	__rot += sVector3D( {_x, _y, _z} );
}

bool
Object::setColor(GLfloat _R, GLfloat _G, GLfloat _B, GLfloat _A) {
	if (_R < 0 || _R > 1 || _G < 0 || _G > 1 || _B < 0 || _B > 1 || _A < 0 || _A > 1)
		return false;
	
	__defColor = sColor( {_R, _G, _B, _A} );
	return true;
}

bool
Object::setColor(int _R, int _G, int _B, GLfloat _A) {
	if (_R < 0 || _R > 255 || _G < 0 || _G > 255 || _B < 0 || _B > 255 || _A < 0 || _A > 1)
		return false;
	
	__defColor = sColor( {
			static_cast< GLfloat >(_R) / 255,
			static_cast< GLfloat >(_G) / 255,
			static_cast< GLfloat >(_B) / 255,
			_A
		} );
	return true;
}

bool
Object::loadFromObj(const string &_objFile, unsigned _invert) {
	if ((sGlobalConfig::DEBUGGING & D_PARAMS) == D_PARAMS)
		cout << LOG_INFO << "Loading object (" << name << ")... ";
	
	if (!__fileExists(_objFile)) {
		if ((sGlobalConfig::DEBUGGING & D_WARNINGS) == D_WARNINGS)
			cout << LOG_WARN << name << ": error; file not found: " << _objFile;
		return false;
	}
	
	__parseObj(_objFile, _invert);
	__bindAppropriateShader();
	
	return true;
}

void
Object::loadIntoVBO() {
	for (auto it = __meshes.begin(); it != __meshes.end(); ++it)
		it -> second -> loadIntoVbo();
}

void
Object::addChild(Object* _childPtr) {
	__children.push_back(_childPtr);
}

void
Object::__parseObj(const string &_fileName, unsigned _invert) {
	Timer objTime;
	auto now = objTime.update(MICROSECONDS);
	unsigned int lastSlash = _fileName.rfind('/'); // we need the localization
	string loc = (lastSlash == string::npos) ? "" : _fileName.substr(0, lastSlash+1);
	
	// some temporary variables
	indicesMap indices;
	
	vector< Position > tempPos;
	tempPos.push_back(Position(0, 0, 0));
	
	vector< TexCoords > tempTex;
	tempTex.push_back(TexCoords(0, 0));
	
	vector< Normal > tempNor;
	tempNor.push_back(Normal(0, 0, 0));
	
	unsigned gLastPosSize = 0, gLastTexSize = 0, gLastNorSize = 0;
	unsigned howToParse = 0;
	bool normalsChecked = false, textureChecked = false;
	string buffer, temp;
	long p = 0;
	GLfloat x, y, z;
	
	Material* lastMtl = (Material*)NULL;
	
	Mesh *current = NULL;
	
 	fstream objFile(_fileName.c_str(), ios::in);
	
	while (!objFile.eof()) {
		getline (objFile, buffer);
		
		if (buffer[0] == '#')
			continue;	// comment, pass this
		
		if (buffer.find((char)92) != string::npos) { // we have backslash - divided to next line
			buffer.erase(buffer.find((char)92), 1);
			string secondLine;
			getline(objFile, secondLine);
			buffer += secondLine;
		}
		
		istringstream line(buffer);
		
		if (buffer.substr(0, 6) == "mtllib") {
			string mtlFileName;
			line >> temp >> mtlFileName;
			__parseMtl(loc + mtlFileName);
			continue;
		} else if (buffer[0] == 'g') {
			string gName = "";
			if (buffer.length() > 2) {
				char g;
				line >> g >> temp;
				while (!line.eof()) {
					gName += temp;
					line >> temp;
				}
			}
			
			// to prevent from creating two meshes with the same name
			while (__meshes.find(gName) != __meshes.end())
				gName += "_";
			
			if (current && !current -> empty()) {
				current -> closeMesh(lastMtl);
				__meshes.insert(make_pair(current -> name, current));
				current = new Mesh(gName);
			} else if (current && current -> empty())
				current -> name = gName;
			else
				current = new Mesh(gName);
			
			gLastPosSize = tempPos.size();
			gLastTexSize = tempTex.size();
			gLastNorSize = tempNor.size();
		} else if (buffer.substr(0, 6) == "usemtl") {
			if (!current)
				current = new Mesh();
			string mtlName;
			line >> temp >> mtlName;
			lastMtl = getMaterialByName(mtlName);
			current -> useMtl(lastMtl);
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
			if (!(_invert & INVERT_X))
				x = 1 - x;
			if (_invert & INVERT_Y)
				y = 1 - y;
			tempTex.push_back(TexCoords(x, y));
			if (!textureChecked) {
				howToParse |= GET_TEXTURE;
				__content |= TEXTURE;
				textureChecked = true;
			}
		} else if (buffer.substr(0, 2) == "vn") {
			line >> temp >> x >> y >> z;
			tempNor.push_back(Normal(x, y, z));
			if (!normalsChecked) {
				howToParse |= GET_NORMALS;
				__content |= NORMALS;
				normalsChecked = true;
			}
		} else if (buffer[0] == 'f')
			__parseFace(
					line, current,
					tempPos, tempTex, tempNor,
					indices,
					gLastPosSize, gLastTexSize, gLastNorSize,
					p, howToParse
				);
	}
	objFile.close();
	
	current -> closeMesh(lastMtl);
	__meshes.insert(make_pair(current -> name, current));
	
	if ((sGlobalConfig::DEBUGGING & D_PARAMS) == D_PARAMS) {
		cout << LOG_INFO << "Detected: ";
		if ((__content & (TEXTURE | NORMALS)) == (TEXTURE | NORMALS))
			cout << "texture coordinates and normals.";
		else if (__content & TEXTURE)
			cout << "texture coordinates.";
		else if (__content & NORMALS)
			cout << "normals.";
		else
			cout << "only vertices.";
	}

	if ((sGlobalConfig::DEBUGGING & D_PARAMS) == D_PARAMS) {
		cout << LOG_INFO << p << " vertices loaded. ";
		cout << "Time: " << (objTime.update(MICROSECONDS) - now) << " ms.";
	}
}

void
Object::__parseFace(
		istringstream&			_line,		// current line
		Mesh*&				_current,		// mesh - where to load indices
		vector< Position >&		_tPos,		// "v"s vector
		vector< TexCoords >&	_tTex,		// "vt"s vector
		vector< Normal >&		_tNor,		// "vn"s vector
		indicesMap&			_indices,		// already parsed indices
		unsigned				_tPosSize,	// we store these 3 sizes to indicate whether
		unsigned				_tTexSize,	// 	the following vertex belongs to the current
		unsigned				_tNorSize,	//	group or not
		long&				_iCount,		// count vertices
		unsigned				_howToParse	// v, v/t, v//n or v/t/n
	   ) {
	char d; // delim
	Index idx; // temporary index
	_line >> d; // passes 'f'
	
	for (int s = 0; s < 3; ++s) {
		if ((_howToParse & (GET_TEXTURE | GET_NORMALS)) == (GET_TEXTURE | GET_NORMALS))
			_line >> idx.v >> d >> idx.t >> d >> idx.n;
		else if (_howToParse & GET_TEXTURE)
			_line >> idx.v >> d >> idx.t;
		else if (_howToParse & GET_NORMALS)
			_line >> idx.v >> d >> d >> idx.n;
		else
			_line >> idx.v;
		
		if (idx.v < 0)
			idx.v = _tPos.size() + idx.v;
		if (idx.t < 0)
			idx.t = _tTex.size() + idx.t;
		if (idx.n < 0)
			idx.n = _tNor.size() + idx.n;
		
		auto it = _indices.find(idx);
		if (it == _indices.end()) {
			unsigned newVertIdx = _current -> push_back(
					Vertex(
							_tPos[idx.v],
		  					_tTex[idx.t],
		  					_tNor[idx.n]
						)
				);
			_indices.insert(make_pair(idx, newVertIdx));
			_current -> addNewIdx(newVertIdx);
		} else {
			if (idx.v < _tPosSize || idx.t < _tTexSize || idx.n < _tNorSize) {
				unsigned newVertIdx = _current -> push_back(
					Vertex(
							_tPos[idx.v],
		  					_tTex[idx.t],
		  					_tNor[idx.n]
						)
					);
				it -> second = newVertIdx;
			}
			_current -> addNewIdx(it -> second);
		}
		++_iCount;
		
	}
	
	if (!_line.eof()) { // we have quad here
		if ((_howToParse & (GET_TEXTURE | GET_NORMALS)) == (GET_TEXTURE | GET_NORMALS))
			_line >> idx.v >> d >> idx.t >> d >> idx.n;
		else if ((_howToParse & GET_TEXTURE) == GET_TEXTURE)
			_line >> idx.v >> d >> idx.t;
		else if ((_howToParse & GET_NORMALS) == GET_NORMALS)
			_line >> idx.v >> d >> d >> idx.n;
		else
			_line >> idx.v;
		
		if (idx.v < 0)
			idx.v = _tPos.size() + idx.v;
		if (idx.t < 0)
			idx.t = _tTex.size() + idx.t;
		if (idx.n < 0)
			idx.n = _tNor.size() + idx.n;
		
		auto it = _indices.find(idx);
		if (it == _indices.end()) {
			unsigned newVertIdx = _current -> push_back(
					Vertex(
							_tPos[idx.v],
		  					_tTex[idx.t],
		  					_tNor[idx.n]
						)
				);
			_indices.insert(make_pair(idx, newVertIdx));
			_current -> addThreeIdxs(newVertIdx);
		} else {
			if (idx.v < _tPosSize || idx.t < _tTexSize || idx.n < _tNorSize) {
				unsigned newVertIdx = _current -> push_back(
					Vertex(
							_tPos[idx.v],
		  					_tTex[idx.t],
		  					_tNor[idx.n]
						)
					);
				it -> second = newVertIdx;
			}
			_current -> addThreeIdxs(it -> second);
		}
		++_iCount;
	}
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
		
		string paramName;
		line >> paramName;
		
		if (paramName == "newmtl") {
			string newMtlName;
			line >>  newMtlName;
			Material *newMtl = new Material(newMtlName);
			__materials.insert(make_pair(newMtlName, newMtl));
			current = newMtl;
		} else if (paramName == "Ka") {
			sColor param;
			line >> param[0] >> param[1] >> param[2];
			param[3] = 1.0;
			current -> loadMaterial(param, MATERIAL_AMBIENT);
		} else if (paramName == "Kd") {
			sColor param;
			line >> param[0] >> param[1] >> param[2];
			param[3] = 1.0;
			current -> loadMaterial(param, MATERIAL_DIFFUSE);
		} else if (paramName == "Ks") {
			sColor param;
			line >> param[0] >> param[1] >> param[2];
			param[3] = 1.0;
			current -> loadMaterial(param, MATERIAL_SPECULAR);
		} else if (paramName == "Ns") {
			GLfloat param;
			line >> param;
			param /= 128;
			param = 128 - param;
			current -> loadShininess(param);
		} else if (paramName == "map_Kd") {
			string texfile;
			line >> texfile;
			if (texfile == "")
				continue;
			Texture* newTex = TextureManager::GetSingleton().getTextureByName(Texture::getName(texfile));
			if (newTex != NULL) {
				current -> appendTexture(newTex);
				continue;
			}
			if (!__fileExists("texture/" + texfile)) {
				if ((sGlobalConfig::DEBUGGING & D_WARNINGS) == D_WARNINGS)
					cout << LOG_WARN << "Texture not found: " << "texture/" << texfile;
				exit(1);
			}
			newTex = new Texture("texture/" + texfile);
			current -> appendTexture(newTex);
		} else if (paramName == "map_bump") {
			string texfile;
			line >> texfile;
			if (texfile == "")
				continue;
			Texture* newTex = TextureManager::GetSingleton().getTextureByName(Texture::getName(texfile));
			if (newTex != NULL) {
				current -> appendTexture(newTex);
				continue;
			}
			if (!__fileExists("texture/" + texfile)) {
				if ((sGlobalConfig::DEBUGGING & D_WARNINGS) == D_WARNINGS)
					cout << LOG_WARN << "Texture not found: " << "texture/" << texfile;
				exit(1);
			}
			newTex = new Texture("texture/" + texfile, MODE_NORMAL_MAP);
			current -> appendTexture(newTex);
			__content |= NORMAL_MAP;
		} else if (paramName == "map_kS") {
			string texfile;
			line >> texfile;
			if (texfile == "")
				continue;
			Texture* newTex = TextureManager::GetSingleton().getTextureByName(Texture::getName(texfile));
			if (newTex != NULL) {
				current -> appendTexture(newTex);
				continue;
			}
			if (!__fileExists("texture/" + texfile)) {
				if ((sGlobalConfig::DEBUGGING & D_WARNINGS) == D_WARNINGS)
					cout << LOG_WARN << "Texture not found: " << "texture/" << texfile;
				exit(1);
			}
			newTex = new Texture("texture/" + texfile, MODE_SPECULAR_MAP);
			current -> appendTexture(newTex);
			__content |= SPECULAR_MAP;
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

void
Object::__bindAppropriateShader() {
	Skylium& global = Skylium::GetSingleton();
	
	if ((__content & (TEXTURE | NORMAL_MAP)) == (TEXTURE | NORMAL_MAP)) {
		global.normalMapShader -> bind(this);
		return;
	} else if ((__content & (TEXTURE | NORMALS)) == (TEXTURE | NORMALS)) {
		global.texturedShadingShader -> bind(this);
		return;
	} else if (__content & (NORMALS)) {
		global.shadingShader -> bind(this);
		return;
	}
	
	global.identityShader -> bind(this);
}