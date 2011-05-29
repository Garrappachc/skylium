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

Mesh::Mesh(const Mesh &_orig) :
		name(_orig.name),
		__vertices(_orig.__vertices),
		__vboID(_orig.__vboID),
		__index(_orig.__index),
		__vboIndexID(_orig.__vboIndexID),
		__hasNormals(_orig.__hasNormals),
		__material(NULL),
		__smooth(_orig.__smooth),
		__usage(_orig.__usage),
		__mode(_orig.__mode) {
	__material = new Material(*_orig.__material);
}

Mesh::~Mesh() {
#ifdef __DEBUG__STRONG__
	cout << "Destruktor: ~Mesh(name = \"" << name << "\".";
#endif
	glDeleteBuffers(1, &__vboID);
	glDeleteBuffers(1, &__vboIndexID);
}

void
Mesh::show() {
	if (__smooth) // czy jest smooth?
		glShadeModel(GL_SMOOTH);
	
	if (__material && __material -> hasAnyTexture()) { // czy mamy jakąkolwiek teksturę?
		glColor4f(1.0, 1.0, 1.0, 1.0); // musi być biały kolor - bez tego nie będzie tekstury
		__material -> setTexture(); // ustawiamy parametry tekstur
		glEnable(GL_TEXTURE_2D); // odblokowujemy tekstury
		glEnableClientState(GL_TEXTURE_COORD_ARRAY); // i ich współrzędne
	}
	
	if (__hasNormals) // mamy normalne?
		glEnableClientState(GL_NORMAL_ARRAY); // powiedzmy o tym OpenGLowi
	
	// chcemy tablice wierzchołków
	glEnableClientState(GL_VERTEX_ARRAY);
	
	// ustawiamy materiał
	if (__material)
		__material -> setMaterial();
	
	
	// renderujemy!
	glBindBuffer(GL_ARRAY_BUFFER, __vboID); // ustawiamy aktywny wskaźnik na odpowiednim buforze
	
	if (__material && __material -> hasAnyTexture())
		glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), BUFFER_OFFSET(12)); // gdzie są koordynaty tekstur
	
	if (__hasNormals)
		glNormalPointer(GL_FLOAT, sizeof(Vertex), BUFFER_OFFSET(20)); // gdzie normalne

	glVertexPointer(3, GL_FLOAT, sizeof(Vertex), BUFFER_OFFSET(0)); // i gdzie wierzchołki
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, __vboIndexID); // teraz chcemy bufor z indeksami

	// Teraz rysujemy wszystko za jednym zamachem.
	// __mode - defaultowo GL_TRIANGLES. Tryb rysowania;
	// __index.size() - ilość wierzchołków do odwzorowania;
	// GL_UNSIGNED_SHORT - typ danych w tablicy indeksów. Nasza tablica to std::vector<GLushort>, czyli
	// 	właśnie unsigned short;
	// BUFFER_OFFSET(0) - mówimy, skąd chcemy zacząć pobierać indeksy wierzchołków.
	glDrawElements(__mode, __index.size(), GL_UNSIGNED_SHORT, BUFFER_OFFSET(0));
	
	GLenum err = glGetError(); // pobieramy błędy
	while (err != GL_NO_ERROR) {
		cout << err << endl;
		err = glGetError();
	}
	
	if (__material && __material -> hasAnyTexture()) // blokujemy
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	
	if (__hasNormals) // blokujemy
		glDisableClientState(GL_NORMAL_ARRAY);
	
	glDisableClientState(GL_VERTEX_ARRAY); // blokujemy
	
	glBindBuffer(GL_ARRAY_BUFFER, 0); // bufor już nam nie będzie potrzebny - wracamy ze wskaźnikiem do ramu
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	
	glShadeModel(GL_FLAT); // zawsze!
}

void
Mesh::loadIntoVbo() {
	
	// generujemy VBO ID
	glGenBuffers(1, &__vboID);
	// ustawiamy aktywny wskaźnik
	glBindBuffer(GL_ARRAY_BUFFER, __vboID);
	
	// rezerwujemy miejsce w buforze
	// Moglibyśmy juz tutaj wysłać tablicę wierzchołków, ale ponoć jest to mniej wydajne.
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * __vertices.size(), NULL, __usage);
	// wysyłamy tablicę wierzchołków do bufora
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertex) * __vertices.size(), &__vertices[0]);
	int bufferSize_v;
	glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &bufferSize_v);
	
	// mówimy, gdzie co jest
	glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), BUFFER_OFFSET(12)); // 12 = rozmiar Position
	glNormalPointer(GL_FLOAT, sizeof(Vertex), BUFFER_OFFSET(20)); // 20 = rozmiar Position + rozmiar TexCoords
	glVertexPointer(3, GL_FLOAT, sizeof(Vertex), BUFFER_OFFSET(0)); // 0, żeby aktywny wskaźnik był na początku
	
	// generujemy VBO ID dla indeksów, ustawiamy aktywny wskaźnik
	glGenBuffers(1, &__vboIndexID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, __vboIndexID);
	
	// wysyłamy tablicę indeksów do karty graficznej
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * __index.size(), NULL, __usage);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(GLushort) * __index.size(), &__index[0]);
	int bufferSize_i;
	glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &bufferSize_i);
#ifdef __DEBUG__
	cout << LOG_INFO << "Mesh::loadIntoVbo(" << name << "): mesh w VBO. Rozmiar: " << bufferSize_v + bufferSize_i << " B.";
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
	return __vertices.size() - 1;
}

void
Mesh::addNewIdx(const int &_idx) {
	__index.push_back(_idx);
}
