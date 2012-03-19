/*
     ___ ___ _   _ __  __                                       
    / __| _ \ | | |  \/  |___ _ __  ___ _ _ _  _   __ _ __ _ __ 
   | (_ |  _/ |_| | |\/| / -_) '  \/ _ \ '_| || |_/ _| '_ \ '_ \
    \___|_|  \___/|_|  |_\___|_|_|_\___/_|  \_, (_)__| .__/ .__/
                                            |__/     |_|  |_|   
   
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

#include "../include/GPUMemory.h"

#include "../include/BufferObject.h"

#include "../include/defines.h"
#include "../include/config.h"
#include "../include/utils.h"

using namespace std;

GPUMemory::GPUMemory() : __vboUsage(0) {
	log(CONSTRUCTOR, "GPUMemory constructed.");
}

GPUMemory::~GPUMemory() {
	log(DESTRUCTOR, "GPUMemory destructed.");
}

void
GPUMemory::prepareRoom(BufferObject& _buffer, vboUsage _usage) {
	gl::GenVertexArrays(1, &_buffer.vaoID);
	checkGLErrors(AT);
	gl::GenBuffers(1, &_buffer.vboID[ELEMENTS_ARRAY].vboID);
	checkGLErrors(AT);
	gl::GenBuffers(1, &_buffer.vboID[DATA_ARRAY].vboID);
	checkGLErrors(AT);
	
	gl::BindVertexArray(_buffer.vaoID);
	checkGLErrors(AT);
	gl::BindBuffer(GL_ELEMENT_ARRAY_BUFFER, _buffer.vboID[ELEMENTS_ARRAY].vboID);
	checkGLErrors(AT);
	gl::BindBuffer(GL_ARRAY_BUFFER, _buffer.vboID[DATA_ARRAY].vboID);
	checkGLErrors(AT);
	
	GLenum usage;
	switch (_usage) {
		case STATIC_DRAW:
			usage = GL_STATIC_DRAW;
			break;
	}
	
	gl::BufferData(
			GL_ELEMENT_ARRAY_BUFFER,
			_buffer.vboID[ELEMENTS_ARRAY].dataSize,
			NULL, usage
		);
	gl::BufferData(
			GL_ARRAY_BUFFER,
			_buffer.vboID[DATA_ARRAY].dataSize,
			NULL, usage
		);
	
	checkGLErrors(AT);
	
	// restore previous VAO state
	gl::BindBuffer(GL_ARRAY_BUFFER, 0);
	gl::BindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	gl::BindVertexArray(0);
	checkGLErrors(AT);
	
	__buffers.push_back(&_buffer);
}

void
GPUMemory::sendData(const BufferObject& _buffer, short unsigned _target, void* _data) {
	gl::BindVertexArray(_buffer.vaoID);
	checkGLErrors(AT);
	
	if (!_buffer.vboID[_target].bound) {
		gl::BindBuffer(_buffer.vboID[_target].target, _buffer.vboID[_target].vboID);
		checkGLErrors(AT);
	}
	
	gl::BufferSubData(_buffer.vboID[_target].target, 0,
			_buffer.vboID[_target].dataSize,
			_data
		);
	checkGLErrors(AT);

	// restore previous VAO state
	if (!_buffer.vboID[_target].bound) {
		gl::BindBuffer(_buffer.vboID[_target].target, 0);
		checkGLErrors(AT);
	}
	
	gl::BindVertexArray(0);
	
	__vboUsage += _buffer.vboID[_target].dataSize;
}

unsigned
GPUMemory::getBufferSize(const BufferObject& _buffer) {
	int elemsSize, dataSize;
	
	gl::BindVertexArray(_buffer.vaoID);
	checkGLErrors(AT);
	
	if (!_buffer.vboID[ELEMENTS_ARRAY].bound)
		gl::BindBuffer(GL_ELEMENT_ARRAY_BUFFER, _buffer.vboID[ELEMENTS_ARRAY].vboID);
	gl::GetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &elemsSize);
	checkGLErrors(AT);
	
	if (!_buffer.vboID[DATA_ARRAY].bound)
		gl::BindBuffer(GL_ARRAY_BUFFER, _buffer.vboID[DATA_ARRAY].vboID);
	gl::GetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &dataSize);
	checkGLErrors(AT);
	
	if (!_buffer.vboID[ELEMENTS_ARRAY].bound)
		gl::BindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	if (!_buffer.vboID[DATA_ARRAY].bound)
		gl::BindBuffer(GL_ARRAY_BUFFER, 0);
	
	gl::BindVertexArray(0);
	
	return elemsSize + dataSize;
}

void
GPUMemory::deleteBuffers(BufferObject& _buffer) {
	gl::DeleteBuffers(1, &_buffer.vboID[ELEMENTS_ARRAY].vboID);
	checkGLErrors(AT);
	_buffer.vboID[ELEMENTS_ARRAY].bound = false;
	_buffer.vboID[ELEMENTS_ARRAY].vboID = 0;
	
	gl::DeleteBuffers(1, &_buffer.vboID[DATA_ARRAY].vboID);
	checkGLErrors(AT);
	_buffer.vboID[DATA_ARRAY].bound = false;
	_buffer.vboID[DATA_ARRAY].vboID = 0;
	
	gl::DeleteVertexArrays(1, &_buffer.vaoID);
	checkGLErrors(AT);
	_buffer.vaoID = 0;
	
	__vboUsage -= _buffer.vboID[ELEMENTS_ARRAY].dataSize;
	__vboUsage -= _buffer.vboID[DATA_ARRAY].dataSize;
}

void *
GPUMemory::mapBuffer(BufferObject& _buffer, short unsigned _target, unsigned _acs) {
	GLenum access;
	if ((_acs & (READ | WRITE)) == (READ | WRITE))
		access = GL_READ_WRITE;
	else if (_acs & READ)
		access = GL_READ_ONLY;
	else if (_acs & WRITE)
		access = GL_WRITE_ONLY;
	else
		return NULL;
	
	gl::BindVertexArray(_buffer.vaoID);
	checkGLErrors(AT);
	
	if (!_buffer.vboID[_target].bound) {
		gl::BindBuffer(_buffer.vboID[_target].target, _buffer.vboID[_target].vboID);
		checkGLErrors(AT);
	}
	
	void* data = gl::MapBuffer(_buffer.vboID[_target].target, access);
	checkGLErrors(AT);
	
	return data;
}

bool
GPUMemory::unmapBuffer(BufferObject& _buffer, short unsigned _target) {
	GLboolean result = gl::UnmapBuffer(_buffer.vboID[_target].target);
	checkGLErrors(AT);
	
	if (!_buffer.vboID[_target].bound) {
		gl::BindBuffer(_buffer.vboID[_target].target, 0);
		checkGLErrors(AT);
	}
	
	gl::BindVertexArray(0);
	
	return result;
}

void
GPUMemory::bind(BufferObject& _buffer, short unsigned _target) {
	gl::BindVertexArray(_buffer.vaoID);
	checkGLErrors(AT);
	
	gl::BindBuffer(_buffer.vboID[_target].target, _buffer.vboID[_target].vboID);
	checkGLErrors(AT);
	
	_buffer.vboID[_target].bound = true;
	
	gl::BindVertexArray(0);
}

void
GPUMemory::unbind(BufferObject& _buffer, short unsigned _target) {
	gl::BindVertexArray(_buffer.vaoID);
	checkGLErrors(AT);
	
	gl::BindBuffer(_buffer.vboID[_target].target,0);
	checkGLErrors(AT);
	
	_buffer.vboID[_target].bound = false;
	
	gl::BindVertexArray(0);
}