/*
          _  ___      _ _      _    
     __ _| |/ __|__ _| | |___ | |_  
    / _` | | (__/ _` | | (_-<_| ' \ 
    \__, |_|\___\__,_|_|_/__(_)_||_|
    |___/                           
 
    Copyright (C) 2012  Michał Garapich garrappachc@gmail.com

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


#ifndef GLFUNCTIONS_H
#define GLFUNCTIONS_H

#include <iostream>

#include <GL/gl.h>

#include "utils.h"

namespace gl {
	
	/*
	 * This namespace contains and handles all OpenGL's functions. */
	
	void initGLExtensionsPointers();
	
	
	
	/* Typedefs */
	
	typedef	GLboolean	Boolean;
	typedef	GLenum	Enum;
	typedef	GLfloat	Float;
	typedef	GLint	Int;
	typedef	GLsizei	Sizei;
	typedef	GLubyte	Ubyte;
	typedef	GLuint	Uint;
	
	
	
	/* GL extensions pointers */
	
	extern void		(* glAttachShader)				(GLuint, GLuint);
	extern void		(* glBindAttribLocation)			(GLuint, GLuint, const GLchar*);
	extern void		(* glBindBuffer)				(GLenum, GLuint);
	extern void		(* glBindVertexArray)			(GLuint);
	extern void		(* glBufferData)				(GLenum, int, const GLvoid*, GLenum);
	extern void		(* glBufferSubData)				(GLenum, GLintptr, GLsizeiptr, const GLvoid*);
	extern void		(* glCompileShader)				(GLuint);
	extern GLuint		(* glCreateProgram)				(void);
	extern GLuint		(* glCreateShader)				(GLenum);
	extern void		(* glDeleteBuffers)				(GLsizei, const GLuint*);
	extern void		(* glDeleteProgram)				(GLuint);
	extern void		(* glDeleteShader)				(GLuint);
	extern void		(* glDeleteVertexArrays)			(GLsizei, const GLuint*);
	extern void		(* glDetachShader)				(GLuint, GLuint);
	extern void		(* glDisableVertexAttribArray)	(GLuint);
	extern void		(* glEnableVertexAttribArray)		(GLuint);
	extern void		(* glGenBuffers)				(GLsizei, GLuint*);
	extern void		(* glGenVertexArrays)			(GLsizei, GLuint*);
	extern void		(* glGetBufferParameteriv)		(GLenum, GLenum, GLint*);
	extern void		(* glGetProgramInfoLog)			(GLuint, GLsizei, GLsizei*, GLchar*);
	extern void		(* glGetProgramiv)				(GLuint, GLenum, GLint*);
	extern void		(* glGetShaderInfoLog)			(GLuint, GLsizei, GLsizei*, GLchar*);
	extern void		(* glGetShaderiv)				(GLuint, GLenum, GLint*);
	extern const GLubyte *	(*glGetStringi)			(GLenum, GLuint);
	extern GLint		(* glGetUniformLocation)			(GLuint, const GLchar*);
	extern GLboolean	(* glIsProgram)				(GLuint);
	extern void		(* glLinkProgram)				(GLuint);
	extern void *		(* glMapBuffer)				(GLenum, GLenum);
	extern GLboolean	(* glUnmapBuffer)				(GLenum);
	extern void		(* glUniform1i)				(GLint, GLint);
	extern void		(* glUniform1f)				(GLint, GLfloat);
	extern void		(* glUniform2f)				(GLint, GLfloat, GLfloat);
	extern void		(* glUniform3f)				(GLint, GLfloat, GLfloat, GLfloat);
	extern void		(* glUniform4f)				(GLint, GLfloat, GLfloat, GLfloat, GLfloat);
	extern void		(* glUniformMatrix3fv)			(GLint, GLsizei, GLboolean, const GLfloat*);
	extern void		(* glUniformMatrix4fv)			(GLint, GLsizei, GLboolean, const GLfloat*);
	extern void		(* glUseProgram)				(GLuint);
	extern void		(* glShaderSource)				(GLuint, GLsizei, const GLchar**, const GLint*);
	extern void		(* glVertexAttribPointer)		(GLuint, GLint, GLenum, GLboolean, GLsizei, const GLvoid*);
	
	
	
	/* Functions */
	
	inline void ActiveTexture(GLenum texture) {
		glActiveTexture(texture);
	}
	
	inline void AttachShader(GLuint program,
						GLuint shader) {
		glAttachShader(program, shader);
	}
	
	inline void BindAttribLocation(GLuint program,
							 GLuint index,
							 const GLchar* name) {
		glBindAttribLocation(program, index, name);
	}
	
	inline void BindBuffer(GLenum target,
					   GLuint buffer) {
		glBindBuffer(target, buffer);
	}
	
	inline void BindTexture(GLenum target,
					    GLuint texture) {
		glBindTexture(target, texture);
	}
	
	inline void BindVertexArray(GLuint array) {
		glBindVertexArray(array);
	}
	
	inline void BlendFunc(GLenum sfactor,
					  GLenum dfactor) {
		glBlendFunc(sfactor, dfactor);
	}
	
	inline void BufferData(GLenum target,
					   GLsizeiptr size,
					   const GLvoid* data,
					   GLenum usage) {
		glBufferData(target, size, data, usage);
	}
	
	inline void BufferSubData(GLenum target,
						 GLintptr offset,
						 GLsizeiptr size,
						 const GLvoid* data) {
		glBufferSubData(target, offset, size, data);
	}
	
	inline void CompileShader(GLuint shader) {
		glCompileShader(shader);
	}
	
	inline Uint CreateProgram(void) {
		return glCreateProgram();
	}
	
	inline Uint CreateShader(GLenum shaderType) {
		return glCreateShader(shaderType);
	}
	
	inline void Clear(GLbitfield mask) {
		glClear(mask);
	}
	
	inline void ClearColor(GLclampf red,
					   GLclampf green,
					   GLclampf blue,
					   GLclampf alpha) {
		glClearColor(red, green, blue, alpha);
	}
	
	inline void Color4f(GLfloat red,
					GLfloat green,
					GLfloat blue,
					GLfloat alpha) {
		glColor4f(red, green, blue, alpha);
	}
	
	inline void DeleteBuffers(GLsizei n,
						 const GLuint* buffers) {
		glDeleteBuffers(n, buffers);
	}
	
	inline void DeleteProgram(GLuint program) {
		glDeleteProgram(program);
	}
	
	inline void DeleteShader(GLuint shader) {
		glDeleteShader(shader);
	}
	
	inline void DeleteTextures(GLsizei n,
						  const GLuint* textures) {
		glDeleteTextures(n, textures);
	}
	
	inline void DeleteVertexArrays(GLsizei n,
							 const GLuint* arrays) {
		glDeleteVertexArrays(n, arrays);
	}
	
	inline void DetachShader(GLuint program,
						GLuint shader) {
		glDetachShader(program, shader);
	}
	
	inline void Disable(GLenum cap) {
		glDisable(cap);
	}
	
	inline void DisableVertexAttribArray(GLuint index) {
		glDisableVertexAttribArray(index);
	}
	
	inline void DrawArrays(GLenum mode,
					   GLint first,
					   GLsizei count) {
		glDrawArrays(mode, first, count);
	}
	
	inline void DrawElements(GLenum mode,
						GLsizei count,
						GLenum type,
						const GLvoid* indices) {
		glDrawElements(mode, count, type, indices);
	}
	
	inline void Enable(GLenum cap) {
		glEnable(cap);
	}
	
	inline void EnableVertexAttribArray(GLuint index) {
		glEnableVertexAttribArray(index);
	}
	
	inline void GenBuffers(GLsizei n,
					   GLuint* buffers) {
		glGenBuffers(n, buffers);
	}
	
	inline void GenTextures(GLsizei n,
					    GLuint* textures) {
		glGenTextures(n, textures);
	}
	
	inline void GenVertexArrays(GLsizei n,
						   GLuint* arrays) {
		glGenVertexArrays(n, arrays);
	}
	
	inline void GetIntegerv(GLenum pname,
					    GLint* params) {
		glGetIntegerv(pname, params);
	}
	
	inline void GetBufferParameteriv(GLenum target,
							   GLenum value,
							   GLint* data) {
		glGetBufferParameteriv(target, value, data);
	}
	
	inline void GetProgramInfoLog(GLuint program,
							GLsizei maxLength,
							GLsizei* length,
							GLchar* infoLog) {
		glGetProgramInfoLog(program, maxLength, length, infoLog);
	}
	
	inline void GetProgramiv(GLuint program,
						GLenum pname,
						GLint* params) {
		glGetProgramiv(program, pname, params);
	}
	
	inline void GetShaderInfoLog(GLuint shader,
						    GLsizei maxLength,
						    GLsizei* length,
						    GLchar* infoLog) {
		glGetShaderInfoLog(shader, maxLength, length, infoLog);
	}
	
	inline void GetShaderiv(GLuint shader,
					    GLenum pname,
					    GLint* params) {
		glGetShaderiv(shader, pname, params);
	}
	
	inline const Ubyte* GetString(GLenum name) {
		return glGetString(name);
	}
	
	inline const Ubyte* GetStringi(GLenum name,
						GLuint index) {
		return glGetStringi(name, index);
	}
	
	inline Int GetUniformLocation(GLuint program,
							const GLchar* name) {
		return glGetUniformLocation(program, name);
	}
	
	inline Boolean IsProgram(GLuint program) {
		return glIsProgram(program);
	}
	
	inline void LinkProgram(GLuint program) {
		glLinkProgram(program);
	}
	
	inline void Hint(GLenum target,
				  GLenum mode) {
		glHint(target, mode);
	}
	
	inline void LineWidth(GLfloat width) {
		glLineWidth(width);
	}
	
	inline void * MapBuffer(GLenum target,
					    GLenum access) {
		return glMapBuffer(target, access);
	}
	
	inline void TexImage2D(GLenum target,
					   GLint level,
					   GLint internalFormat,
					   GLsizei width,
					   GLsizei height,
					   GLint border,
					   GLenum format,
					   GLenum type,
					   const GLvoid* data) {
		glTexImage2D(target, level, internalFormat, width, height,
				   border, format, type, data);
	}
	
	inline void TexParameteri(GLenum target,
						 GLenum pname,
						 GLint param) {
		glTexParameteri(target, pname, param);
	}
	
	inline Boolean UnmapBuffer(GLenum target) {
		return glUnmapBuffer(target);
	}
	
	inline void Uniform1f(GLint location,
					  GLfloat v0) {
		glUniform1f(location, v0);
	}
	
	inline void Uniform1i(GLint location,
					  GLint v0) {
		glUniform1i(location, v0);
	}
	
	inline void Uniform2f(GLint location,
					  GLfloat v0,
					  GLfloat v1) {
		glUniform2f(location, v0, v1);
	}
	
	inline void Uniform3f(GLint location,
					  GLfloat v0,
					  GLfloat v1,
					  GLfloat v2) {
		glUniform3f(location, v0, v1, v2);
	}
	
	inline void Uniform4f(GLint location,
					  GLfloat v0,
					  GLfloat v1,
					  GLfloat v2,
					  GLfloat v3) {
		glUniform4f(location, v0, v1, v2, v3);
	}
	
	inline void UniformMatrix3fv(GLint location,
						    GLsizei count,
						    GLboolean transpose,
						    const GLfloat* value) {
		glUniformMatrix3fv(location, count, transpose, value);
	}
	
	inline void UniformMatrix4fv(GLint location,
						    GLsizei count,
						    GLboolean transpose,
						    const GLfloat* value) {
		glUniformMatrix4fv(location, count, transpose, value);
	}
	
	inline void UseProgram(GLuint program) {
		glUseProgram(program);
	}
	
	inline void ShaderSource(GLuint shader,
						GLsizei count,
						const GLchar** string,
						const GLint* length) {
		glShaderSource(shader, count, string, length);
	}
	
	inline void VertexAttribPointer(GLuint index,
							  GLint size,
							  GLenum type,
							  GLboolean normalized,
							  GLsizei stride,
							  const GLvoid* pointer) {
		glVertexAttribPointer(index, size, type, normalized, stride, pointer);
	}
	
	inline void Viewport(GLint x,
					 GLint y,
					 GLsizei width,
					 GLsizei height) {
		glViewport(x, y, width, height);
	}
	
}

#endif // GLFUNCTIONS_H
