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
		pointers_(0),
		texCoords_(0),
		normals_(0),
		hasNormals_(0),
		material_(NULL),
		smooth_(false),
		mode_(GL_TRIANGLES) {}

PolygonGroup::PolygonGroup(const string &_name) :
		name(_name),
		pointers_(0),
		texCoords_(0),
		normals_(0),
		hasNormals_(0),
		material_(NULL),
		smooth_(false),
		mode_(GL_TRIANGLES) {}

PolygonGroup::PolygonGroup(const string &_name, const GLfloat *_pointers, const int &_size) :
		name(_name),
		pointers_(_pointers, _pointers + _size),
		texCoords_(0),
		normals_(0),
		hasNormals_(0),
		material_(NULL),
		smooth_(false),
		mode_(GL_TRIANGLES) {}

PolygonGroup::PolygonGroup(const string &_name, const sArray &_pointers) :
		name(_name),
		pointers_(_pointers),
		texCoords_(0),
		normals_(0),
		hasNormals_(0),
		material_(NULL),
		smooth_(false),
		mode_(GL_TRIANGLES) {}

PolygonGroup::~PolygonGroup() {
#ifdef __DEBUG__
	cout << LOG_INFO << "Destruktor: ~PolygonGroup() <" << name << ">";
#endif
}

void
PolygonGroup::show() {
	if (smooth_)
		glShadeModel(GL_SMOOTH);
	
	if (material_ && material_ -> hasAnyTexture()) {
		glColor4f(1.0, 1.0, 1.0, 1.0);
		material_ -> setTexture();
		glEnable(GL_TEXTURE_2D);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	}
	
	if(hasNormals_) {
		glEnableClientState(GL_NORMAL_ARRAY);
	}
	
	glEnableClientState(GL_VERTEX_ARRAY);
	
	if (material_)
		material_ -> setMaterial();
	
	glVertexPointer(3, GL_FLOAT, 0, (GLvoid*)&pointers_[0]);
	if (material_ && material_ -> hasAnyTexture())
		glTexCoordPointer(2, GL_FLOAT, 0, (GLvoid*)&texCoords_[0]);
	if (hasNormals_)
		glNormalPointer(GL_FLOAT, 0, (GLvoid*)&normals_[0]);
	glDrawArrays(mode_, 0, (int)(pointers_.size() / 3));

	if (material_ && material_ -> hasAnyTexture())
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	if (hasNormals_)
		glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	
}