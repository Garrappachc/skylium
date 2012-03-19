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
		__gpu(GPUMemory::GetSingleton()),
		__isShown(true) {
	
	__materials[0].begin = 0;
	
	log(CONSTRUCTOR, "Mesh (\"%s\") constructed.", name.c_str());

}

Mesh::~Mesh() {
	__buffer.deleteBuffers();
	checkGLErrors(AT);
	log(DESTRUCTOR, "Mesh (\"%s\") destructed.", name.c_str());
}

void
Mesh::show() {
	if (!__isShown)
		return;
	// there we go!
	gl::BindVertexArray(__buffer.vaoID);
	checkGLErrors(AT);
	
	for (MeshRange& m: __materials) {
		if (m.begin == m.end)
			continue;
		
		if (m.material)
			m.material -> setMaterial();
		
		gl::DrawElements(
				__mode,
				m.end,
				GL_UNSIGNED_INT,
				BUFFER_OFFSET(sizeof(IndicesType) * m.begin)
			);
		checkGLErrors(AT);
		
		if (m.material)
			m.material -> unsetTextures();
	
	}
	
	gl::BindVertexArray(0);
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
	
	log(BUFFER, "Mesh (\"%s\") in the VBO. Size: %u B.", name.c_str(), __buffer.getBufferSize());
	
	__buffer.bind(ELEMENTS_ARRAY);
	__buffer.bind(DATA_ARRAY);
	
	gl::BindVertexArray(__buffer.vaoID);
	checkGLErrors(AT);
	
	gl::VertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(0)); // vertex
	gl::VertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(16)); // texture
	gl::VertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(24)); // normal
	checkGLErrors(AT);

	gl::EnableVertexAttribArray(0);
	gl::EnableVertexAttribArray(1);
	gl::EnableVertexAttribArray(2);
	checkGLErrors(AT);
	
	gl::BindVertexArray(0);
}

void
Mesh::useMtl(Material *_mtl) {
	if (!__indices.empty()) {
		__materials[__materials.size() - 1].end = __indices.size() - __materials[__materials.size() - 1].begin;
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
	__materials[__materials.size() - 1].end = __indices.size() - __materials[__materials.size() - 1].begin;
	
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

