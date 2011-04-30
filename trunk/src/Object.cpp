/* 
 * File:   Object.cpp
 * Author: Michał Garapich
 * 
 * Created on 2 april 2011, 11:17
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

Object::Object (const string &_name, const GLfloat *vertex, const int &_size) :
		name(_name),
		texCoords_(0),
		texture_(0),
		normals_(0),
		hasNormals_(false),
		colors_(0),
		hasColors_(false),
		defColor_(1.0, 1.0, 1.0, 1.0),
		mov_(0, 0, 0),
		rot_(0, 0, 0), 
		scale_(0, 0, 0),
		shader_(0) {
	
	for (int i = 0; i < _size; i++) {
		pointers_[i] = vertex[i];
	}
}

Object::Object(const string &_name, const sArray &_pointers) : 
		name(_name),
		texCoords_(0),
		texture_(0),
		normals_(0),
		hasNormals_(false),
		colors_(0),
		hasColors_(false),
		defColor_(1.0, 1.0, 1.0, 1.0),
		mov_(0, 0, 0),
		rot_(0, 0, 0), 
		scale_(0, 0, 0),
		shader_(0) {
	pointers_ = _pointers;

}

Object::Object(const string &_name) : 
		name(_name),
		pointers_(0),
		texCoords_(0),
		texture_(0),
		normals_(0),
		hasNormals_(false),
		colors_(0),
		hasColors_(false),
		defColor_(1.0, 1.0, 1.0, 1.0),
		mov_(0, 0, 0),
		rot_(0, 0, 0), 
		scale_(0, 0, 0),
		shader_(0) {
}

Object::Object() : 
		name(""),
		pointers_(0),
		texCoords_(0),
		texture_(0),
		normals_(0),
		hasNormals_(false),
		colors_(0),
		hasColors_(false),
		defColor_(1.0, 1.0, 1.0, 1.0),
		mov_(0, 0, 0),
		rot_(0, 0, 0), 
		scale_(0, 0, 0),
		shader_(0) {
}

Object::~Object() {
	if (texture_) {
		glDeleteTextures(1, &texture_);
	}
}

void
Object::show() {
	glPushMatrix();
	if (!texture_)
		glColor4f(defColor_[0], defColor_[1], defColor_[2], defColor_[3]);
	else {
		glColor4f(1.0, 1.0, 1.0, 1.0);
		glBindTexture(GL_TEXTURE_2D, texture_);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	}

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	if(hasNormals_) {
		glEnableClientState(GL_NORMAL_ARRAY);
	}

	glEnableClientState(GL_VERTEX_ARRAY);

	if (texture_) {
		glEnable(GL_TEXTURE_2D);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	}

	glTranslated(mov_[0], mov_[1], mov_[2]);

	glScaled(scale_[0], scale_[1], scale_[2]);

	glRotated(rot_[0], 1.0, 0, 0);
	glRotated(rot_[1], 0, 1.0, 0);
	glRotated(rot_[2], 0, 0, 1.0);
	
	if (shader_)
		shader_ -> toggle();

	glVertexPointer(3, GL_FLOAT, 0, (GLvoid*)&pointers_[0]);
	if (texture_)
		glTexCoordPointer(2, GL_FLOAT, 0, (GLvoid*)&texCoords_[0]);
	if (hasNormals_)
		glNormalPointer(GL_FLOAT, 0, (GLvoid*)&normals_[0]);
	glDrawArrays(GL_TRIANGLES, 0, (int)(pointers_.size() / 3));

	if (texture_)
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	if (hasNormals_)
		glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	
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

	texture_ = SOIL_load_OGL_texture(
				_textureFile.c_str(),
				4,
				0,
				SOIL_FLAG_POWER_OF_TWO | SOIL_FLAG_MIPMAPS
			);
	if (!texture_)
		return false;

#ifdef __DEBUG__
	cout << LOG_INFO << name << ": teksturę załadowano pomyślnie.\n";
#endif

	texCoords_.resize(_size);
	for (int i = 0; i < _size; i++) {
		texCoords_[i] = _texturePointers[i];
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

	texture_ = SOIL_load_OGL_texture(
				_textureFile.c_str(),
				4,
				0,
				SOIL_FLAG_POWER_OF_TWO | SOIL_FLAG_MIPMAPS
			);
	if (!texture_)
		return false;

	texCoords_ = _texturePointers;

#ifdef __DEBUG__
	cout << LOG_INFO << name << ": teksturę załadowano pomyślnie.\n";
#endif
	return true;
}

bool
Object::loadTexture(const string &_textureFile) {
	if (_textureFile == "")
		return false;

	if (!fileExists(_textureFile)) {
#ifdef __DEBUG__
		cout << LOG_WARN << name << ": plik z teksturą nie istnieje! Szukam: " << _textureFile << endl;
#endif
		return false;
	}
	texture_ = SOIL_load_OGL_texture(
				_textureFile.c_str(),
				4,
				0,
				SOIL_FLAG_POWER_OF_TWO | SOIL_FLAG_MIPMAPS
			);
	if (!texture_)
		return false;

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
Object::loadFromObj(const string &_objFile, const string &_mtlFile, const unsigned int &_whatToLoad) {
#ifdef __DEBUG__
	cout << LOG_INFO << "Wczytywanie obiektu: " << name << "... ";
#endif
	if (!fileExists(_objFile)) {
#ifdef __DEBUG__
		cout << LOG_WARN << name << ": nie znalazłem pliku: " << _objFile << endl;
#endif
		return false;
	}
	if (!fileExists(_mtlFile)) {
#ifdef __DEBUG__
		cout << LOG_WARN << name << ": nie znalazłem pliku: " << _mtlFile << endl;
#endif
		return false;
	}

	vector < sVertex > vertices; // tymczasowy wektor współrzędnych wierzchołków
	vector < sCoords > texture; // tymczasowy wektor współrzędnych tekstury
	vector < sVertex > normals; // wektor normalnych

	/* Jeszcze kilka zmiennych tymczasowych */
	string buffer = "";
	string temp = "";
	string tbl[3];

	sVertex v(3);
	sCoords t(2);
	sVertex n(3);

	int i = 0, j = 0;

	fstream objFile(_objFile.c_str(), ios::in);
	
	while (!objFile.eof()) {
                getline(objFile, buffer);
				if (buffer[0] == '#')
					continue;
				
				istringstream line(buffer);
				string ch = buffer.substr(0, 2);

				if (ch == "v ") {
					line >> temp >> tbl[0] >> tbl[1] >> tbl[2];
					v[0] = strToNum< GLfloat >(tbl[0]);
					v[1] = strToNum< GLfloat >(tbl[1]);
					v[2] = strToNum< GLfloat >(tbl[2]);
					vertices.push_back(v);
				} else if (ch == "vt") {
					line >> temp >> tbl[0] >> tbl[1];
					t[0] = strToNum< int >(tbl[0]);
					t[1] = strToNum< int >(tbl[1]);
					texture.push_back(t);
				} else if (ch == "vn") {
					line >> temp >> tbl[0] >> tbl[1] >> tbl[2];
					n[0] = strToNum< GLfloat >(tbl[0]);
					n[1] = strToNum< GLfloat >(tbl[1]);
					n[2] = strToNum< GLfloat >(tbl[2]);
					normals.push_back(n);
				} else if (ch[0] == 'f') {
					if (_whatToLoad == (GET_VERTICES | GET_TEXTURE | GET_NORMALS)) {
						line >> temp >> tbl[0] >> tbl[1] >> tbl[2];
						int slashPos1 = tbl[0].find('/');
						int slashPos2 = tbl[0].find('/', slashPos1 + 1);
						int vx = strToNum< int >(tbl[0].substr(0, slashPos1)) - 1;
						int tx = strToNum< int >(tbl[0].substr(slashPos1+1, slashPos2 - slashPos1 - 1)) - 1;
						int nx = strToNum< int >(tbl[0].substr(slashPos2 + 1)) - 1;

						slashPos1 = tbl[1].find('/');
						slashPos2 = tbl[1].find('/', slashPos1 + 1);
						int vy = strToNum< int >(tbl[1].substr(0, slashPos1)) - 1;
						int ty = strToNum< int >(tbl[1].substr(slashPos1+1, slashPos2 - slashPos1 - 1)) - 1;
						int ny = strToNum< int >(tbl[1].substr(slashPos2+1)) - 1;

						slashPos1 = tbl[2].find('/');
						slashPos2 = tbl[2].find('/', slashPos1 + 1);
						int vz = strToNum< int >(tbl[2].substr(0, slashPos1)) - 1;
						int tz = strToNum< int >(tbl[2].substr(slashPos1+1, slashPos2 - slashPos1 - 1)) - 1;
						int nz = strToNum< int >(tbl[2].substr(slashPos2+1)) - 1;

						pointers_.resize(i + 9);
						texCoords_.resize(j + 6);
						normals_.resize(i + 9);

						pointers_[i] = vertices[vx][0];
						normals_[i++] = normals[nx][0];
						pointers_[i] = vertices[vx][1];
						normals_[i++] = normals[nx][1];
						pointers_[i] = vertices[vx][2];
						normals_[i++] = normals[nx][2];

						pointers_[i] = vertices[vy][0];
						normals_[i++] = normals[ny][0];
						pointers_[i] = vertices[vy][1];
						normals_[i++] = normals[ny][1];
						pointers_[i] = vertices[vy][2];
						normals_[i++] = normals[ny][2];

						pointers_[i] = vertices[vz][0];
						normals_[i++] = normals[nz][0];
						pointers_[i] = vertices[vz][1];
						normals_[i++] = normals[nz][1];
						pointers_[i] = vertices[vz][2];
						normals_[i++] = normals[nz][2];

						texCoords_[j++] = texture[tx][0];
						texCoords_[j++] = texture[tx][1];
						texCoords_[j++] = texture[ty][0];
						texCoords_[j++] = texture[ty][1];
						texCoords_[j++] = texture[tz][0];
						texCoords_[j++] = texture[tz][1];


					} else if (_whatToLoad == (GET_VERTICES | GET_TEXTURE)) {

					} else if (_whatToLoad == (GET_VERTICES | GET_NORMALS)) {
						line >> temp >> tbl[0] >> tbl[1] >> tbl[2];
						int slashPos1 = tbl[0].find('/');
						int slashPos2 = tbl[0].find('/', slashPos1 + 1);
						int vx = strToNum< int >(tbl[0].substr(0, slashPos1)) - 1;
						int nx = strToNum< int >(tbl[0].substr(slashPos2 + 1)) - 1;

						slashPos1 = tbl[1].find('/');
						slashPos2 = tbl[1].find('/', slashPos1 + 1);
						int vy = strToNum< int >(tbl[1].substr(0, slashPos1)) - 1;
						int ny = strToNum< int >(tbl[1].substr(slashPos2+1)) - 1;

						slashPos1 = tbl[2].find('/');
						slashPos2 = tbl[2].find('/', slashPos1 + 1);
						int vz = strToNum< int >(tbl[2].substr(0, slashPos1)) - 1;
						int nz = strToNum< int >(tbl[2].substr(slashPos2+1)) - 1;

						pointers_.resize(i + 9);
						normals_.resize(i + 9);

						pointers_[i] = vertices[vx][0];
						normals_[i++] = normals[nx][0];
						pointers_[i] = vertices[vx][1];
						normals_[i++] = normals[nx][1];
						pointers_[i] = vertices[vx][2];
						normals_[i++] = normals[nx][2];

						pointers_[i] = vertices[vy][0];
						normals_[i++] = normals[ny][0];
						pointers_[i] = vertices[vy][1];
						normals_[i++] = normals[ny][1];
						pointers_[i] = vertices[vy][2];
						normals_[i++] = normals[ny][2];

						pointers_[i] = vertices[vz][0];
						normals_[i++] = normals[nz][0];
						pointers_[i] = vertices[vz][1];
						normals_[i++] = normals[nz][1];
						pointers_[i] = vertices[vz][2];
						normals_[i++] = normals[nz][2];

					}
				}

	}

	objFile.close();

	fstream mtlFile(_mtlFile.c_str(), ios::in);

	if (_whatToLoad & GET_TEXTURE) {
		while(!mtlFile.eof()) {
			getline(mtlFile, buffer);
			if (buffer.substr(0, 4) != "map_") {
				continue;
			} else {
				istringstream line(buffer);
				string pic;
				line >> temp >> pic;
				string picLoc = "texture/" + pic;
				if (!this -> loadTexture(picLoc)) {
#ifdef __DEBUG__
					cout << LOG_WARN << name << ": nie udało się załadować tekstury!";
#endif
					return false;
				}
			}
		}
	}
	mtlFile.close();

	if (_whatToLoad & GET_NORMALS)
		hasNormals_ = true;
	
#ifdef __DEBUG__
	cout << "wczytano " << (int)(pointers_.size() / 3) << " poligonów";
	if (_whatToLoad & GET_NORMALS)
		cout << ", " << normals_.size() << " normalnych";
	cout << ".\n";
	cout.flush();
#endif
	
	return true;

}

void
Object::showStats() {
	cout << "== Obiekt: " << name << " ==\n" <<
			"Liczba wierzchołków: " << (int)pointers_.size() / 3 << endl <<
			"Liczba koordynat tekstury: " << (int)texCoords_.size() / 2 << endl <<
			"GLuint tekstury: " << texture_ << endl <<
			"Liczba normalnych: " << (int)normals_.size() / 3 << endl <<
			"Liczba zdefiniowanych kolorów: " << colors_.size() / 4 << endl <<
			"Kolor obiektu: " << defColor_[0] << ", " << defColor_[1] << ", " << defColor_[2] << ", " << defColor_[3] << endl <<
			"Shader: " << shader_ << endl << endl;
}

bool
Object::fileExists(const string &_fileName) {
	struct stat buf;
	if (stat(_fileName.c_str(), &buf) == 0)
		return 1;
	else
		return 0;
}