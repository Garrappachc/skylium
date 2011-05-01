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

material::material() :
		ambient(0.2, 0.2, 0.2, 1.0),
		diffuse(0.8, 0.8, 0.8, 1.0),
		specular(0.0, 0.0, 0.0, 1.0) {}

Object::Object (const string &_name, const GLfloat *_pointers, const int &_size) :
		name(_name),
		texCoords_(0),
		texture_(0),
		normals_(0),
		hasNormals_(false),
		colors_(0),
		hasColors_(false),
		defColor_(1.0, 1.0, 1.0, 1.0),
		usedMtl_(),
		mov_(0, 0, 0),
		rot_(0, 0, 0), 
		scale_(0, 0, 0),
		shader_(0) {
	
	for (int i = 0; i < _size; i++) {
		pointers_[i] = _pointers[i];
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
		usedMtl_(),
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
		usedMtl_(),
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
		usedMtl_(),
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
	
	glMaterialfv(GL_FRONT, GL_AMBIENT, &usedMtl_.ambient[0]);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, &usedMtl_.diffuse[0]);
	glMaterialfv(GL_FRONT, GL_SPECULAR, &usedMtl_.specular[0]);

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
	
	unsigned int lastSlash = 0;
	while (_objFile.find('/', lastSlash+1) != string::npos) {
		lastSlash = _objFile.find('/', lastSlash+1);
	}
	
	string loc = _objFile.substr(0, lastSlash+1);

	vector < sVertex > vertices; // tymczasowy wektor współrzędnych wierzchołków
	vector < sCoords > texture; // tymczasowy wektor współrzędnych tekstury
	vector < sVertex > normals; // wektor normalnych

	/* Jeszcze kilka zmiennych tymczasowych */
	string buffer = "";
	string temp = "";

	sVertex v(3);
	sCoords t(2);
	sVertex n(3);

	int i = 0, j = 0;
	
	string mtlFileName;

	fstream objFile(_objFile.c_str(), ios::in);
	
	while (!objFile.eof()) {
                getline(objFile, buffer);
				if (buffer[0] == '#')
					continue;
				
				istringstream line(buffer);

				if (buffer.substr(0, 6) == "mtllib") {
					line >> temp >> mtlFileName;
					continue;
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

						pointers_.resize(i + 9);
						texCoords_.resize(j + 6);
						normals_.resize(i + 9);

						pointers_[i] = vertices[--vx][0];
						normals_[i++] = normals[--nx][0];
						pointers_[i] = vertices[vx][1];
						normals_[i++] = normals[nx][1];
						pointers_[i] = vertices[vx][2];
						normals_[i++] = normals[nx][2];

						pointers_[i] = vertices[--vy][0];
						normals_[i++] = normals[--ny][0];
						pointers_[i] = vertices[vy][1];
						normals_[i++] = normals[ny][1];
						pointers_[i] = vertices[vy][2];
						normals_[i++] = normals[ny][2];

						pointers_[i] = vertices[--vz][0];
						normals_[i++] = normals[--nz][0];
						pointers_[i] = vertices[vz][1];
						normals_[i++] = normals[nz][1];
						pointers_[i] = vertices[vz][2];
						normals_[i++] = normals[nz][2];

						texCoords_[j++] = texture[--tx][0];
						texCoords_[j++] = texture[tx][1];
						texCoords_[j++] = texture[--ty][0];
						texCoords_[j++] = texture[ty][1];
						texCoords_[j++] = texture[--tz][0];
						texCoords_[j++] = texture[tz][1];
						
						continue;

					} else if ((_whatToLoad & (GET_VERTICES | GET_TEXTURE)) == (GET_VERTICES | GET_TEXTURE)) {
						
						char d;
						int vx, vy, vz, tx, ty, tz;
						
						line >> temp >>
								vx >> d >> tx >>
								vy >> d >> ty >>
								vz >> d >> tz;

						pointers_.resize(i + 9);
						texCoords_.resize(j + 6);

						pointers_[i] = vertices[--vx][0];
						pointers_[i] = vertices[vx][1];
						pointers_[i] = vertices[vx][2];

						pointers_[i] = vertices[--vy][0];
						pointers_[i] = vertices[vy][1];
						pointers_[i] = vertices[vy][2];

						pointers_[i] = vertices[--vz][0];
						pointers_[i] = vertices[vz][1];
						pointers_[i] = vertices[vz][2];

						texCoords_[j++] = texture[--tx][0];
						texCoords_[j++] = texture[tx][1];
						texCoords_[j++] = texture[--ty][0];
						texCoords_[j++] = texture[ty][1];
						texCoords_[j++] = texture[--tz][0];
						texCoords_[j++] = texture[tz][1];
						
						continue;

					} else if ((_whatToLoad & (GET_VERTICES | GET_NORMALS)) == (GET_VERTICES | GET_NORMALS)) {
						
						char d;
						int vx, nx, vy, ny, vz, nz;
						
						/* example:
						 f 5521//1950 1296//1869 3589//1872
						 */
						
						line >> temp >>
								vx >> d >> d >> nx >>
								vy >> d >> d >> ny >>
								vz >> d >> d >> nz;

						pointers_.resize(i + 9);
						normals_.resize(i + 9);

						pointers_[i] = vertices[--vx][0];
						normals_[i++] = normals[--nx][0];
						pointers_[i] = vertices[vx][1];
						normals_[i++] = normals[nx][1];
						pointers_[i] = vertices[vx][2];
						normals_[i++] = normals[nx][2];

						pointers_[i] = vertices[--vy][0];
						normals_[i++] = normals[--ny][0];
						pointers_[i] = vertices[vy][1];
						normals_[i++] = normals[ny][1];
						pointers_[i] = vertices[vy][2];
						normals_[i++] = normals[ny][2];

						pointers_[i] = vertices[--vz][0];
						normals_[i++] = normals[--nz][0];
						pointers_[i] = vertices[vz][1];
						normals_[i++] = normals[nz][1];
						pointers_[i] = vertices[vz][2];
						normals_[i++] = normals[nz][2];
						
						continue;

					}
				}

	}

	objFile.close();
	
	mtlFileName = loc + mtlFileName;
	
	if (!fileExists(mtlFileName)) {
#ifdef __DEBUG__
		cout << endl << LOG_WARN << name << ": nie znalazłem pliku: " << mtlFileName << endl;
#endif
		return false;
	}

	fstream mtlFile(mtlFileName.c_str(), ios::in);
	
	while(!mtlFile.eof()) {
		getline(mtlFile, buffer);
		istringstream line(buffer);
		if (buffer[0] == '#') {
			continue;
		} else if (buffer.substr(0, 2) == "Ka") {
			if (!(_whatToLoad & GET_MATERIAL))
				continue;
			line >> temp >> usedMtl_.ambient[0] >> usedMtl_.ambient[1] >> usedMtl_.ambient[2];
			continue;
		} else if (buffer.substr(0, 2) == "Kd") {
			if (!(_whatToLoad & GET_MATERIAL))
				continue;
			line >> temp >> usedMtl_.diffuse[0] >> usedMtl_.diffuse[1] >> usedMtl_.diffuse[2];
			continue;
		} else if (buffer.substr(0, 2) == "Ks") {
			if (!(_whatToLoad & GET_MATERIAL))
				continue;
			line >> temp >> usedMtl_.specular[0] >> usedMtl_.specular[1] >> usedMtl_.specular[2];
			continue;
		} else if (buffer.substr(0, 4) == "map_") {
			if (!(_whatToLoad & GET_TEXTURE))
				continue;
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