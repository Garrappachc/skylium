/*
     __  __        _                   
    |  \/  |___ __| |_    __ _ __ _ __ 
    | |\/| / -_|_-< ' \ _/ _| '_ \ '_ \
    |_|  |_\___/__/_||_(_)__| .__/ .__/
                            |_|  |_|   

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

#include <GL/gl.h>

#include "../include/Mesh.h"

#include "../include/Skylium.h"
#include "../include/Shader.h"

#include "../include/defines.h"
#include "../include/config.h"
#include "../include/utils.h"

// makro to count the position
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

using namespace std;

Mesh::Mesh(const string &_name) :
		name(_name),
		__vertices(0),
		__vboID(0),
		__indices(0),
		__vaoID(0),
		__hasNormals(false),
		__material(NULL),
		__smooth(false),
		__usage(GL_STATIC_DRAW),
		__mode(GL_TRIANGLES) {
	__initGLExtensionsPointers();
	if ((sGlobalConfig::DEBUGGING & D_CONSTRUCTORS) == D_CONSTRUCTORS)
		cout << LOG_INFO << "Mesh (\"" << name << "\") constructed.";

}

Mesh::Mesh(const Mesh &_orig) :
		name(_orig.name),
		__vertices(_orig.__vertices),
		__vboID(_orig.__vboID),
		__indices(_orig.__indices),
		__vaoID(_orig.__vaoID),
		__hasNormals(_orig.__hasNormals),
		__material(NULL),
		__smooth(_orig.__smooth),
		__usage(_orig.__usage),
		__mode(_orig.__mode) {
	if ((sGlobalConfig::DEBUGGING & D_ALL_CONSTRUCTORS) == D_ALL_CONSTRUCTORS)
		cout << LOG_INFO << "Mesh (\"" << name << "\") constructed as a copy.";
	__material = new Material(*_orig.__material);
}

Mesh::~Mesh() {
	if (__vboID != 0)
		glDeleteBuffers(1, &__vboID);
	if (__vaoID != 0)
		glDeleteBuffers(1, &__vaoID);
	if ((sGlobalConfig::DEBUGGING & D_DESTRUCTORS) == D_DESTRUCTORS)
		cout << LOG_INFO << "Mesh (\"" << name << "\") destructed.";
}

void
Mesh::setAllParams() {
	if (__smooth) // smooth shading?
		glShadeModel(GL_SMOOTH);
	
	if (__material && __material -> hasAnyTexture()) { // do we have any texture?
		glColor4f(1.0, 1.0, 1.0, 1.0); // must be white - otherwise there's no texture visible
		__material -> setTextures(); // we set texture parameters
		glEnable(GL_TEXTURE_2D); // textures enabled
		glEnableClientState(GL_TEXTURE_COORD_ARRAY); // and texture's coords
	}
	
	if (__hasNormals) // do we have normals?
		glEnableClientState(GL_NORMAL_ARRAY); // tell that GL
	
	// we want vertices array
	glEnableClientState(GL_VERTEX_ARRAY);
	checkGLErrors(AT);
	
	// set the material
	if (__material)
		__material -> setMaterial();
}

void
Mesh::show() {
	// there we go!
	if (__vboID != 0) {
		glBindBuffer(GL_ARRAY_BUFFER, __vboID); // sets the active pointer on the correct buffer
		checkGLErrors(AT); // if not -D__DEBUG__, it does not retards the rendering process
	
		if (__material && __material -> hasAnyTexture())
			glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), BUFFER_OFFSET(12)); // textures coordinates
	
		if (__hasNormals)
			glNormalPointer(GL_FLOAT, sizeof(Vertex), BUFFER_OFFSET(20)); // normals

		glVertexPointer(3, GL_FLOAT, sizeof(Vertex), BUFFER_OFFSET(0)); // and vertices
		checkGLErrors(AT);
	} else {
		if (__material && __material -> hasAnyTexture())
			glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), &__vertices[0].textureCoords);
		
		if (__hasNormals)
			glNormalPointer(GL_FLOAT, sizeof(Vertex), &__vertices[0].normalVector);
		
		glVertexPointer(3, GL_FLOAT, sizeof(Vertex), &__vertices[0]);
		checkGLErrors(AT);
	}
	
	if (__vaoID != 0) {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, __vaoID); // now, what we want is the buffer with indices
		checkGLErrors(AT);

		// Now we draw all.
		// __mode - GL_TRIANGLES by default.
		// __indices.size() - number of vertices to be drawn.
		// GL_UNSIGNED_SHORT - Indices data type. Out vector is std::vector<GLushort>, so it is
		// 	exactly unsigned short.
		// BUFFER_OFFSET(0) - where do we want the OpenGL to start getting the vertices from.
		glDrawElements(__mode, __indices.size(), GL_UNSIGNED_SHORT, BUFFER_OFFSET(0));
		checkGLErrors(AT);
	} else { // no VBO
		glDrawElements(__mode, __indices.size(), GL_UNSIGNED_SHORT, &__indices[0]);
		checkGLErrors(AT);
	}
	
	if (__material && __material -> hasAnyTexture()) // disable
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	
	if (__hasNormals) // disable
		glDisableClientState(GL_NORMAL_ARRAY);
	
	glDisableClientState(GL_VERTEX_ARRAY); // disable
	
	if (__vboID != 0) {
		glBindBuffer(GL_ARRAY_BUFFER, 0); // we do not need buffer any more - get back to RAM
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
	checkGLErrors(AT);
	
	glShadeModel(GL_FLAT); // always!
}

void
Mesh::loadIntoVbo() {
	// generates VBO ID for indices, sets the active pointer
	glGenBuffers(1, &__vaoID);
	checkGLErrors(AT);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, __vaoID);
	checkGLErrors(AT);
	
	// generates the VBO ID
	glGenBuffers(1, &__vboID);
	checkGLErrors(AT);
	// sets the active pointer
	glBindBuffer(GL_ARRAY_BUFFER, __vboID);
	checkGLErrors(AT);
	
	// reservate the room in the buffer
	// We could send the vertices right now, but it seems to be less effective
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * __vertices.size(), NULL, __usage);
	checkGLErrors(AT);
	// send the vertices' array to the buffer
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertex) * __vertices.size(), &__vertices[0]);
	checkGLErrors(AT);
	int bufferSize_v;
	glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &bufferSize_v);
	checkGLErrors(AT);
	
	// we tell where is what
	glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), BUFFER_OFFSET(12)); // 12 = rozmiar Position
	glNormalPointer(GL_FLOAT, sizeof(Vertex), BUFFER_OFFSET(20)); // 20 = rozmiar Position + rozmiar TexCoords
	glVertexPointer(3, GL_FLOAT, sizeof(Vertex), BUFFER_OFFSET(0)); // 0, żeby aktywny wskaźnik był na początku
	
	// indices array to the buffer
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * __indices.size(), NULL, __usage);
	checkGLErrors(AT);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(GLushort) * __indices.size(), &__indices[0]);
	checkGLErrors(AT);
	int bufferSize_i;
	glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &bufferSize_i);
	checkGLErrors(AT);
	
	if ((sGlobalConfig::DEBUGGING & D_BUFFER) == D_BUFFER) {
		cout << LOG_INFO << "Mesh (" << name << "): in the VBO. Size: " << bufferSize_v + bufferSize_i << " B.";
		cout.flush();
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	checkGLErrors(AT);
	
}

void
Mesh::useMtl(Material *_mtl) {
	__material = _mtl;
}

void
Mesh::smooth(bool _s) {
	__smooth = _s;
}

unsigned
Mesh::push_back(const Vertex &_v) {
	__vertices.push_back(_v);
	return __vertices.size() - 1;
}

void
Mesh::addNewIdx(int _idx) {
	__indices.push_back(_idx);
}

void
Mesh::addThreeIdxs(int _idx) {
	__indices.push_back(__indices[__indices.size() - 3]);
	__indices.push_back(__indices[__indices.size() - 2]);
	__indices.push_back(_idx);
}

void
Mesh::__initGLExtensionsPointers() {
	glBindBuffer =	getProcAddr< decltype(glBindBuffer) >("glBindBufferARB");
	glDeleteBuffers = getProcAddr< decltype(glDeleteBuffers) >("glDeleteBuffersARB");
	glGenBuffers = getProcAddr< decltype(glGenBuffers) >("glGenBuffersARB");
	glBufferData = getProcAddr< decltype(glBufferData) >("glBufferDataARB");
	glBufferSubData = getProcAddr< decltype(glBufferSubData) >("glBufferSubDataARB");
	glGetBufferParameteriv = getProcAddr< decltype(glGetBufferParameteriv) >("glGetBufferParameterivARB");
}
