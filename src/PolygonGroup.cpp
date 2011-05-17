/* 
 * PolygonGroup.cpp
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

#include "../include/PolygonGroup.h"
#include "../include/defines.h"

using namespace std;

PolygonGroup::PolygonGroup() :
		name(""),
		__pointers(0),
		__texCoords(0),
		__normals(0),
		__hasNormals(0),
		__material(NULL),
		__smooth(false),
		__mode(GL_TRIANGLES)
			{}

PolygonGroup::PolygonGroup(const string &_name) :
		name(_name),
		__pointers(0),
		__texCoords(0),
		__normals(0),
		__hasNormals(0),
		__material(NULL),
		__smooth(false),
		__mode(GL_TRIANGLES)
			{}

PolygonGroup::PolygonGroup(const string &_name, const GLfloat *_pointers, const int &_size) :
		name(_name),
		__pointers(_pointers, _pointers + _size),
		__texCoords(0),
		__normals(0),
		__hasNormals(0),
		__material(NULL),
		__smooth(false),
		__mode(GL_TRIANGLES)
			{}

PolygonGroup::PolygonGroup(const string &_name, const sArray &_pointers) :
		name(_name),
		__pointers(_pointers),
		__texCoords(0),
		__normals(0),
		__hasNormals(0),
		__material(NULL),
		__smooth(false),
		__mode(GL_TRIANGLES)
			{}

PolygonGroup::~PolygonGroup() {
#ifdef __DEBUG__
	cout << LOG_INFO << "Destruktor: ~PolygonGroup() <" << name << ">";
#endif
}

void
PolygonGroup::show() {
	if (__smooth)
		glShadeModel(GL_SMOOTH);
	
	if (__material && __material -> hasAnyTexture()) {
		glColor4f(1.0, 1.0, 1.0, 1.0);
		__material -> setTexture();
		glEnable(GL_TEXTURE_2D);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	}
	
	if(__hasNormals) {
		glEnableClientState(GL_NORMAL_ARRAY);
	}
	
	glEnableClientState(GL_VERTEX_ARRAY);
	
	if (__material)
		__material -> setMaterial();
	
	glVertexPointer(3, GL_FLOAT, 0, (GLvoid*)&__pointers[0]);
	if (__material && __material -> hasAnyTexture())
		glTexCoordPointer(2, GL_FLOAT, 0, (GLvoid*)&__texCoords[0]);
	if (__hasNormals)
		glNormalPointer(GL_FLOAT, 0, (GLvoid*)&__normals[0]);
	glDrawArrays(__mode, 0, (int)(__pointers.size() / 3));

	if (__material && __material -> hasAnyTexture())
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	if (__hasNormals)
		glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	
}