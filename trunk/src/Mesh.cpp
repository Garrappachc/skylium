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
		__indices(0),
		__materials(1),
		__smooth(false),
		__usage(STATIC_DRAW),
		__mode(GL_TRIANGLES),
		__gpu(GPUMemory::GetSingleton()) {
	__initGLExtensionsPointers();
	
	__materials[0].begin = 0;
	
	if ((sGlobalConfig::DEBUGGING & D_CONSTRUCTORS) == D_CONSTRUCTORS)
		cout << LOG_INFO << "Mesh (\"" << name << "\") constructed.";

}

Mesh::~Mesh() {
	__buffer.deleteBuffers();
	checkGLErrors(AT);
	if ((sGlobalConfig::DEBUGGING & D_DESTRUCTORS) == D_DESTRUCTORS)
		cout << LOG_INFO << "Mesh (\"" << name << "\") destructed.";
}

void
Mesh::show() {
	// there we go!
	glBindVertexArray(__buffer.vaoID);
	checkGLErrors(AT);
	
	for (MeshRange& m: __materials) {
		if (m.begin == m.end)
			continue;
		
#ifdef __DEBUG__
		cout << "\n  Material = " << m.material;
		cout.flush();
#endif // __DEBUG__
		
		if (m.material)
			m.material -> setMaterial();

#ifdef __DEBUG__
		cout << "\n  Drawing elements from " << m.begin <<
			" to " << m.end + m.begin << "...";
		cout.flush();
#endif // __DEBUG__
		
		glDrawRangeElements(
				__mode,
				m.begin,
				__buffer.vboID[ELEMENTS_ARRAY].dataCount,
				m.end,
				GL_UNSIGNED_INT,
				BUFFER_OFFSET(0)
			);
		checkGLErrors(AT);

#ifdef __DEBUG__
		cout << "\n  Elements drawn.";
		cout.flush();
#endif // __DEBUG__
		
		//if (m.material)
		//	m.material -> unsetTextures();
	
	}
	
	glBindVertexArray(0);
}

void
Mesh::loadIntoVbo() {
	__buffer.vboID[ELEMENTS_ARRAY].dataCount = __indices.size();
	__buffer.vboID[ELEMENTS_ARRAY].dataSize = sizeof(IndicesType) * __indices.size();
	
	__buffer.vboID[DATA_ARRAY].dataCount = __vertices.size();
	__buffer.vboID[DATA_ARRAY].dataSize = sizeof(Vertex) * __vertices.size();
	
	__buffer.prepareRoom();
	__buffer.sendData(ELEMENTS_ARRAY, &__indices[0]);
	__buffer.sendData(DATA_ARRAY, &__vertices[0]);
	
	if ((sGlobalConfig::DEBUGGING & D_BUFFER) == D_BUFFER) {
		cout << LOG_INFO << "Mesh (" << name << "): in the VBO. Size: " << __buffer.getBufferSize() << " B.";
		cout.flush();
	}
	
	__buffer.bind(ELEMENTS_ARRAY);
	__buffer.bind(DATA_ARRAY);
	
	glBindVertexArray(__buffer.vaoID);
	checkGLErrors(AT);
	
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(0)); // vertex
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(16)); // texture
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(24)); // normal
	checkGLErrors(AT);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	checkGLErrors(AT);
	
	glBindVertexArray(0);
}

void
Mesh::useMtl(Material *_mtl) {
	if (!__indices.empty()) {
		__materials[__materials.size() - 1].end = __indices.size() - __materials[__materials.size() - 1].begin - 1;
		__materials.push_back(MeshRange(__indices.size()));
	}
	__materials[__materials.size() - 1].material = _mtl;
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
Mesh::closeMesh(Material* _mat) {
	if ((__materials[__materials.size() - 1].material == NULL) && (_mat != NULL))
		__materials[__materials.size() - 1].material = _mat;
	__materials[__materials.size() - 1].end = __indices.size();
	
	loadIntoVbo();
}

void
Mesh::flush() {
	__indices.clear();
	__vertices.clear();
}

void
Mesh::raise() {
	__indices.resize(__buffer.vboID[ELEMENTS_ARRAY].dataCount);
	memcpy(
			&__indices[0],
			__buffer.mapBuffer(ELEMENTS_ARRAY, READ),
			__buffer.vboID[ELEMENTS_ARRAY].dataSize
		);
	__buffer.unmapBuffer(ELEMENTS_ARRAY);
	
	__vertices.resize(__buffer.vboID[DATA_ARRAY].dataCount);
	memcpy(
		&__vertices[0],
	   __buffer.mapBuffer(DATA_ARRAY, READ),
		  __buffer.vboID[DATA_ARRAY].dataSize
	);
	__buffer.unmapBuffer(DATA_ARRAY);
}

void
Mesh::__initGLExtensionsPointers() {
	glBindVertexArray = getProcAddr< decltype(glBindVertexArray) >("glBindVertexArray");
	glBindBuffer =	getProcAddr< decltype(glBindBuffer) >("glBindBufferARB");
	glVertexAttribPointer = getProcAddr< decltype(glVertexAttribPointer) >("glVertexAttribPointer");
	glEnableVertexAttribArray = getProcAddr< decltype(glEnableVertexAttribArray) >("glEnableVertexAttribArray");
	glDisableVertexAttribArray = getProcAddr< decltype(glDisableVertexAttribArray) >("glDisableVertexAttribArray");
}
