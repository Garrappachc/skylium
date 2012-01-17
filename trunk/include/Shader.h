/*
     ___ _            _          _    
    / __| |_  __ _ __| |___ _ _ | |_  
    \__ \ ' \/ _` / _` / -_) '_|| ' \ 
    |___/_||_\__,_\__,_\___|_|(_)_||_|
   
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


#ifndef SHADER_H
#define SHADER_H

#include <string>

#include "Object.h"
#include "Vectors.h"
#include "Matrices.h"

#define GL_FRAGMENT_SHADER 0x8B30
#define GL_VERTEX_SHADER	0x8B31
#define GL_COMPILE_STATUS 0x8B81
#define GL_LINK_STATUS 0x8B82

/**
 * High-level shader program.
 */
class Shader {

public:
	/**
	 * Ctor that gets two arguments.
	 * @param vertFileName Vertex shader source file;
	 * @param fragFileName Flagment shader source file.
	 */
	Shader(const std::string&, const std::string&);
	
	/**
	 * Detaches and destroys the shader.
	 */
	virtual ~Shader();

	/**
	 * Compiles and links the shader program.
	 * @return False if something went wrong.
	 */
	bool make();

	/**
	 * Toggles on/off.
	 */
	void toggle();
	
	/**
	 * Bind shader to an object.
	 * @param dest Pointer to the object that the shader has to be bound to.
	 */
	void bind(Object*);
	
	/**
	 * Unbinds shader from the object.
	 */
	void unbind(Object*);
	
	/**
	 * @return True, if the shader is bound to an object.
	 */
	bool isBound(Object*);
	
	/**
	 * Sends some data to the shader.
	 * @param name Name of variable in the shader.
	 * @param params Vector of N float to be sent.
	 */
	void setUniformFloat(const std::string&, const sVec< GLfloat >&);
	
	/**
	 * Sends a matrix to the shader.
	 * @param name Name of variable in the shader.
	 * @param matrix Matrix of 16 floats to be sent.
	 */
	void setMatrixFloat(const std::string&, const sMat16&);

private:
	std::string __vertFile;
	std::string __fragFile;

	GLint __vertexShader;
	GLint __fragmentShader;

	GLint __shaderProgram;

	GLboolean __isRunning;
	
	void __initGLExtensionsPointers();

	bool __fileExists(const std::string&);
	
	/* Pointer to GL's extensions. */
	GLuint	(*glCreateShader) (GLenum);
	void		(*glDetachShader) (GLuint, GLuint);
	void		(*glDeleteProgram) (GLuint);
	void		(*glDeleteShader) (GLuint);
	void		(*glShaderSource) (GLuint, GLsizei, const GLchar**, const GLint*);
	void		(*glCompileShader) (GLuint);
	void		(*glGetShaderiv) (GLuint, GLenum, GLint*);
	void		(*glGetShaderInfoLog) (GLuint, GLsizei, GLsizei*, GLchar*);
	GLuint	(*glCreateProgram) (void);
	void		(*glAttachShader) (GLuint, GLuint);
	void		(*glLinkProgram) (GLuint);
	void		(*glGetProgramiv) (GLuint, GLenum, GLint*);
	void		(*glGetProgramInfoLog) (GLuint, GLsizei, GLsizei*, GLchar*);
	void		(*glUseProgram) (GLuint);
	GLint	(*glGetUniformLocation) (GLuint, const GLchar*);
	void		(*glUniform2f) (GLint, GLfloat, GLfloat);
	void		(*glUniform3f) (GLint, GLfloat, GLfloat, GLfloat);
	void		(*glUniform4f) (GLint, GLfloat, GLfloat, GLfloat, GLfloat);
	void		(*glUniformMatrix4fv) (GLint, GLsizei, GLboolean, const GLfloat*);
	
};

#endif // SHADER_H
