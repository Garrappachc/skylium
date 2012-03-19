/*
         _  ___      _ _                   
    __ _| |/ __|__ _| | |___  __ _ __ _ __ 
   / _` | | (__/ _` | | (_-<_/ _| '_ \ '_ \
   \__, |_|\___\__,_|_|_/__(_)__| .__/ .__/
   |___/                        |_|  |_|   

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


#include "../include/utils.h"

namespace gl {

void			(* glAttachShader)				(GLuint, GLuint);
void			(* glBindAttribLocation)			(GLuint, GLuint, const GLchar*);
void			(* glBindBuffer)				(GLenum, GLuint);
void			(* glBindVertexArray)			(GLuint);
void			(* glBufferData)				(GLenum, int, const GLvoid*, GLenum);
void			(* glBufferSubData)				(GLenum, GLintptr, GLsizeiptr, const GLvoid*);
void			(* glCompileShader)				(GLuint);
GLuint		(* glCreateProgram)				(void);
GLuint		(* glCreateShader)				(GLenum);
void			(* glDeleteBuffers)				(GLsizei, const GLuint*);
void			(* glDeleteProgram)				(GLuint);
void			(* glDeleteShader)				(GLuint);
void			(* glDeleteVertexArrays)			(GLsizei, const GLuint*);
void			(* glDetachShader)				(GLuint, GLuint);
void			(* glDisableVertexAttribArray)	(GLuint);
void			(* glEnableVertexAttribArray)		(GLuint);
void			(* glGenBuffers)				(GLsizei, GLuint*);
void			(* glGenVertexArrays)			(GLsizei, GLuint*);
void			(* glGetBufferParameteriv)		(GLenum, GLenum, GLint*);
void			(* glGetProgramInfoLog)			(GLuint, GLsizei, GLsizei*, GLchar*);
void			(* glGetProgramiv)				(GLuint, GLenum, GLint*);
void			(* glGetShaderInfoLog)			(GLuint, GLsizei, GLsizei*, GLchar*);
void			(* glGetShaderiv)				(GLuint, GLenum, GLint*);
const GLubyte *	(*glGetStringi)			(GLenum, GLuint);
GLint		(* glGetUniformLocation)			(GLuint, const GLchar*);
GLboolean		(* glIsProgram)				(GLuint);
void			(* glLinkProgram)				(GLuint);
void *		(* glMapBuffer)				(GLenum, GLenum);
GLboolean		(* glUnmapBuffer)				(GLenum);
void			(* glUniform1i)				(GLint, GLint);
void			(* glUniform1f)				(GLint, GLfloat);
void			(* glUniform2f)				(GLint, GLfloat, GLfloat);
void			(* glUniform3f)				(GLint, GLfloat, GLfloat, GLfloat);
void			(* glUniform4f)				(GLint, GLfloat, GLfloat, GLfloat, GLfloat);
void			(* glUniformMatrix3fv)			(GLint, GLsizei, GLboolean, const GLfloat*);
void			(* glUniformMatrix4fv)			(GLint, GLsizei, GLboolean, const GLfloat*);
void			(* glUseProgram)				(GLuint);
void			(* glShaderSource)				(GLuint, GLsizei, const GLchar**, const GLint*);
void			(* glVertexAttribPointer)		(GLuint, GLint, GLenum, GLboolean, GLsizei, const GLvoid*);

void
initGLExtensionsPointers() {
	glAttachShader = getProcAddr< decltype(glAttachShader) >("glAttachShader");
	glBindAttribLocation = getProcAddr< decltype(glBindAttribLocation) >("glBindAttribLocation");
	glBindBuffer =	getProcAddr< decltype(glBindBuffer) >("glBindBufferARB");
	glBindVertexArray = getProcAddr< decltype(glBindVertexArray) >("glBindVertexArray");
	glBufferData = getProcAddr< decltype(glBufferData) >("glBufferDataARB");
	glBufferSubData = getProcAddr< decltype(glBufferSubData) >("glBufferSubDataARB");
	glCompileShader = getProcAddr< decltype(glCompileShader) >("glCompileShader");
	glCreateProgram = getProcAddr< decltype(glCreateProgram) >("glCreateProgram");
	glCreateShader = getProcAddr< decltype(glCreateShader) >("glCreateShader");
	glDeleteBuffers = getProcAddr< decltype(glDeleteBuffers) >("glDeleteBuffersARB");
	glDeleteProgram = getProcAddr< decltype(glDeleteProgram) >("glDeleteProgram");
	glDeleteShader = getProcAddr< decltype(glDeleteShader) >("glDeleteShader");
	glDeleteVertexArrays = getProcAddr< decltype(glDeleteVertexArrays) >("glDeleteVertexArrays");
	glDetachShader = getProcAddr< decltype(glDetachShader) >("glDetachShader");
	glDisableVertexAttribArray = getProcAddr< decltype(glDisableVertexAttribArray) >("glDisableVertexAttribArray");
	glEnableVertexAttribArray = getProcAddr< decltype(glEnableVertexAttribArray) >("glEnableVertexAttribArray");
	glGenBuffers = getProcAddr< decltype(glGenBuffers) >("glGenBuffersARB");
	glGenVertexArrays = getProcAddr< decltype(glGenVertexArrays) >("glGenVertexArrays");
	glGetBufferParameteriv = getProcAddr< decltype(glGetBufferParameteriv) >("glGetBufferParameterivARB");
	glGetProgramInfoLog = getProcAddr< decltype(glGetProgramInfoLog) >("glGetProgramInfoLog");
	glGetProgramiv = getProcAddr< decltype(glGetProgramiv) >("glGetProgramiv");
	glGetShaderInfoLog = getProcAddr< decltype(glGetShaderInfoLog) >("glGetShaderInfoLog");
	glGetShaderiv = getProcAddr< decltype(glGetShaderiv) >("glGetShaderiv");
	glGetStringi = getProcAddr< decltype(glGetStringi) >("glGetStringi");
	glGetUniformLocation = getProcAddr< decltype(glGetUniformLocation) >("glGetUniformLocation");
	glIsProgram = getProcAddr< decltype(glIsProgram) >("glIsProgram");
	glLinkProgram = getProcAddr< decltype(glLinkProgram) >("glLinkProgram");
	glMapBuffer = getProcAddr< decltype(glMapBuffer) >("glMapBuffer");
	glUnmapBuffer = getProcAddr< decltype(glUnmapBuffer) >("glUnmapBuffer");
	glUniform1i = getProcAddr< decltype(glUniform1i) >("glUniform1i");
	glUniform1f = getProcAddr< decltype(glUniform1f) >("glUniform1f");
	glUniform2f = getProcAddr< decltype(glUniform2f) >("glUniform2f");
	glUniform3f = getProcAddr< decltype(glUniform3f) >("glUniform3f");
	glUniform4f = getProcAddr< decltype(glUniform4f) >("glUniform4f");
	glUniformMatrix4fv = getProcAddr< decltype(glUniformMatrix4fv) >("glUniformMatrix4fv");
	glUniformMatrix3fv = getProcAddr< decltype(glUniformMatrix3fv) >("glUniformMatrix3fv");
	glUseProgram = getProcAddr< decltype(glUseProgram) >("glUseProgram");
	glShaderSource = getProcAddr< decltype(glShaderSource) >("glShaderSource");
	glVertexAttribPointer = getProcAddr< decltype(glVertexAttribPointer) >("glVertexAttribPointer");
}

} // namespace gl