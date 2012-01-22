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
		__vaoID(0),
		__vertices(0),
		__verticesVboID(0),
		__indices(0),
		__indicesVboID(0),
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
		__vaoID(_orig.__vaoID),
		__vertices(_orig.__vertices),
		__verticesVboID(_orig.__verticesVboID),
		__indices(_orig.__indices),
		__indicesVboID(_orig.__indicesVboID),
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
	if (__verticesVboID != 0)
		glDeleteBuffers(1, &__verticesVboID);
	if (__indicesVboID != 0)
		glDeleteBuffers(1, &__indicesVboID);
	if (__vaoID != 0)
		glDeleteVertexArrays(1, &__vaoID);
	checkGLErrors(AT);
	if ((sGlobalConfig::DEBUGGING & D_DESTRUCTORS) == D_DESTRUCTORS)
		cout << LOG_INFO << "Mesh (\"" << name << "\") destructed.";
}

void
Mesh::setAllParams() {
	if (__material && __material -> hasAnyTexture()) // do we have any texture?
		__material -> setTextures(); // we set texture parameters 
	
	// set the material
	if (__material)
		__material -> setMaterial();
}

void
Mesh::show() {
	// there we go!
	glBindVertexArray(__vaoID);
	checkGLErrors(AT);
	
	glDrawElements(__mode, __indices.size(), GL_UNSIGNED_SHORT, BUFFER_OFFSET(0));
	checkGLErrors(AT);
	
	glBindVertexArray(0);
}

void
Mesh::loadIntoVbo() {
	checkGLErrors(AT);
	// generate VAO
	glGenVertexArrays(1, &__vaoID);
	checkGLErrors(AT);
	
	// generates the VBO ID for indices
	glGenBuffers(1, &__indicesVboID);
	checkGLErrors(AT);
	
	// generates the VBO ID for vertices
	glGenBuffers(1, &__verticesVboID);
	checkGLErrors(AT);
	
	glBindVertexArray(__vaoID);
	checkGLErrors(AT);
	
	// sets the active pointer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, __indicesVboID);
	checkGLErrors(AT);
	
	glBindBuffer(GL_ARRAY_BUFFER, __verticesVboID);
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
	
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(0)); // vertex
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(16)); // texture
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(24)); // normal
	
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

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	checkGLErrors(AT);
	
	glBindVertexArray(0);
	
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
	checkGLErrors(AT);
	glGenVertexArrays = getProcAddr< decltype(glGenVertexArrays) >("glGenVertexArrays");
	glBindVertexArray = getProcAddr< decltype(glBindVertexArray) >("glBindVertexArray");
	glBindBuffer =	getProcAddr< decltype(glBindBuffer) >("glBindBufferARB");
	glDeleteBuffers = getProcAddr< decltype(glDeleteBuffers) >("glDeleteBuffersARB");
	glDeleteVertexArrays = getProcAddr< decltype(glDeleteVertexArrays) >("glDeleteVertexArrays");
	glGenBuffers = getProcAddr< decltype(glGenBuffers) >("glGenBuffersARB");
	glBufferData = getProcAddr< decltype(glBufferData) >("glBufferDataARB");
	glBufferSubData = getProcAddr< decltype(glBufferSubData) >("glBufferSubDataARB");
	glGetBufferParameteriv = getProcAddr< decltype(glGetBufferParameteriv) >("glGetBufferParameterivARB");
	glVertexAttribPointer = getProcAddr< decltype(glVertexAttribPointer) >("glVertexAttribPointer");
	glEnableVertexAttribArray = getProcAddr< decltype(glEnableVertexAttribArray) >("glEnableVertexAttribArray");
	glDisableVertexAttribArray = getProcAddr< decltype(glDisableVertexAttribArray) >("glDisableVertexAttribArray");
	checkGLErrors(AT);
}
