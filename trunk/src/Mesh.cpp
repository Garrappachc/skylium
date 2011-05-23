/*
    Mesh.cpp
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

#include <GL/glew.h>

#include "../include/Mesh.h"

#include "../include/defines.h"

// pomocne makro do obliczania pozycji
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

using namespace std;

Mesh::Mesh(const string &_name) :
		name(_name),
		__vertices(0),
		__vboID(0),
		__index(0),
		__vboIndexID(0),
		__hasNormals(false),
		__material(NULL),
		__smooth(false),
		__usage(GL_STATIC_DRAW),
		__mode(GL_TRIANGLES) {}

Mesh::~Mesh() {
	glDeleteBuffers(1, &__vboID);
	glDeleteBuffers(1, &__vboIndexID);
}

void
Mesh::show() {
	if (__smooth)
		glShadeModel(GL_SMOOTH);
	
	if (__material && __material -> hasAnyTexture()) {
		glColor4f(1.0, 1.0, 1.0, 1.0);
		__material -> setTexture();
		glEnable(GL_TEXTURE_2D);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	}
	
	if (__hasNormals)
		glEnableClientState(GL_NORMAL_ARRAY);
	
	glEnableClientState(GL_VERTEX_ARRAY);
	
	if (__material)
		__material -> setMaterial();
	
	
	
	// renderujemy!
	glBindBuffer(GL_ARRAY_BUFFER, __vboID);
	
	glVertexPointer(3, GL_FLOAT, sizeof(Vertex), BUFFER_OFFSET(0));
	
	if (__material && __material -> hasAnyTexture())
		glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), BUFFER_OFFSET(12));
	
	if (__hasNormals)
		glNormalPointer(GL_FLOAT, sizeof(Vertex), BUFFER_OFFSET(20));
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, __vboIndexID);
	glDrawElements(__mode, __index.size(), GL_UNSIGNED_SHORT, BUFFER_OFFSET(0));
	
	GLenum err = glGetError();
	while (err != GL_NO_ERROR) {
		cout << err << endl;
		err = glGetError();
	}
	
	if (__material && __material -> hasAnyTexture())
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	
	if (__hasNormals)
		glDisableClientState(GL_NORMAL_ARRAY);
	
	glDisableClientState(GL_VERTEX_ARRAY);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	
	glShadeModel(GL_FLAT);
}

void
Mesh::loadIntoVbo() {
	
#ifdef __DEBUG__
	cout << LOG_INFO << "Mesh::loadIntoVbo(" << name << "): Rozmiar tablicy wierzchołków: " << __vertices.size() << ".";
	cout << LOG_INFO << "Mesh::loadIntoVbo(" << name << "): Rozmiar tablicy indeksów: " << __index.size() << ".";
	cout.flush();
#endif
	
	// generujemy VBO ID
	glGenBuffers(1, &__vboID);
	// ustawiamy aktywny wskaźnik
	glBindBuffer(GL_ARRAY_BUFFER, __vboID);
	
	// rezerwujemy miejsce w buforze
	// Moglibyśmy juz tutaj wysłać tablicę wierzchołków, ale ponoć jest to mniej wydajne.
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * __vertices.size(), NULL, __usage);
	// wysyłamy tablicę wierzchołków do bufora
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertex) * __vertices.size(), &__vertices[0]);
	int bufferSize;
	glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &bufferSize);
#ifdef __DEBUG__
	cout << LOG_INFO << "Mesh::loadIntoVbo(" << name << "): wierzchołki w VBO. Rozmiar: " << bufferSize << " B.";
	cout.flush();
#endif
	
	// mówimy, gdzie co jest
	glVertexPointer(3, GL_FLOAT, sizeof(Vertex), BUFFER_OFFSET(0)); // 0, żeby aktywny wskaźnik był na początku
	glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), BUFFER_OFFSET(12)); // 12 = rozmiar Position
	glNormalPointer(GL_FLOAT, sizeof(Vertex), BUFFER_OFFSET(20)); // 20 = rozmiar Position + rozmiar TexCoords
	
	// generujemy VBO ID dla indeksów, ustawiamy aktywny wskaźnik
	glGenBuffers(1, &__vboIndexID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, __vboIndexID);
	
	// wysyłamy tablicę indeksów do karty graficznej
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * __index.size(), NULL, __usage);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(GLushort) * __index.size(), &__index[0]);
	glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &bufferSize);
#ifdef __DEBUG__
	cout << LOG_INFO << "Mesh::loadIntoVbo(" << name << "): indeksy w VBO. Rozmiar: " << bufferSize << " B.";
	cout.flush();
#endif
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void
Mesh::useMtl(Material *_mtl) {
	__material = _mtl;
}

void
Mesh::smooth(const bool &_s) {
	__smooth = _s;
}

unsigned
Mesh::push_back(const Vertex &_v) {
	__vertices.push_back(_v);
	/*cout << "\nUmieszczony wierzchołek:\n  Pozycja: ("
		<< _v.vertexPosition.x << ", "
		<< _v.vertexPosition.y << ", "
		<< _v.vertexPosition.z << ")\n  Koordynaty tekstury: ("
		<< _v.textureCoords.u << ", "
		<< _v.textureCoords.v << ")\n  Normalna: ("
		<< _v.normalVector.x << ", "
		<< _v.normalVector.y << ", "
		<< _v.normalVector.z << ")\n";*/
	return __vertices.size() - 1;
}

void
Mesh::addNewIdx(const int &_idx) {
	//cout << "\nUmieszczony indeks: " << _idx << endl;
	__index.push_back(_idx);
}
