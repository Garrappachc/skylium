/* 
 * Box.cpp
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

#include "../include/Box.h"
#include "../include/Object.h"
#include "../include/Shader.h"
#include "../include/shapes.h"

using namespace std;

Box::Box(const string &_name) {
	name = _name;
	__pGroups.resize(1);
	__pGroups[0] = new PolygonGroup("", boxPointers, sizeof(boxPointers) / sizeof(GLfloat));
	__pGroups[0] -> __normals = vector < GLfloat >(boxNormals, boxNormals + sizeof(boxNormals) / sizeof(GLfloat));
	__pGroups[0] -> __mode = GL_QUADS;
}