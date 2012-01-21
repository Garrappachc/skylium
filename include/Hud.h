/*
     _  _         _   _    
    | || |_  _ __| | | |_  
    | __ | || / _` |_| ' \ 
    |_||_|\_,_\__,_(_)_||_|

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


#ifndef HUD_H
#define HUD_H

#include <vector>

#include <GL/gl.h>

#include "Singleton.h"
#include "Vectors.h"

class Skylium;
class HudData;
class Shader;
class MatricesManager;


class Hud : public Singleton< Hud > {

public:
	
	/**
	 * Ctor. Sends some output.
	 */
	Hud();
	
	/**
	 * Sends output.
	 */
	virtual ~Hud();
	
	/**
	 * Draws the hud.
	 */
	void draw();
	
	/**
	 * Toggle hud on/off.
	 */
	void toggle();
	
	/**
	 * @return True, if the Hud is on.
	 */
	bool visible() { return __visible; }
	
	/**
	 * Send the Hud more data to be displayed.
	 * @param newData New data to be attached.
	 */
	void attachData(HudData*);
	
	/**
	 * Sets the Hud's color.
	 */
	void setColor(const sColor& _bg, const sColor& _br) { __background = _bg; __border = _br; }
	
	void prepare();
	
private:
	
	/**
	 * Prepares the Projection Matrix for the Hud to be displayed.
	 */
	void __hudMode(bool);
	
	void __initGLExtensionsPointers();
	
	/* Hud vertices for rendering (GL_QUADS) */
	GLfloat __vertices[16];
	
	bool __visible;
	
	GLuint __vaoID, __vboID;
	
	/* Contains data */
	std::vector< HudData* > __toDisplay;
	
	std::vector< HudData* >::const_iterator __displayList;
	
	sColor __background;
	sColor __border;
	
	/* Identity shader for skylium */
	Shader* __hudShader;
	
	MatricesManager& __matrices;
	
	/* GL's extensions' pointers */
	void (*glGenVertexArrays) (GLsizei, GLuint*);
	void	(*glGenBuffers) (GLsizei, GLuint*);
	void (*glBindVertexArray) (GLuint);
	void	(*glBindBuffer) (GLenum, GLuint);
	void	(*glBufferData) (GLenum, int, const GLvoid*, GLenum);
	void	(*glDeleteBuffers) (GLsizei, const GLuint*);
	void (*glDeleteVertexArrays) (GLsizei, const GLuint*);
	void	(*glBufferSubData) (GLenum, GLintptr, GLsizeiptr, GLvoid*);
	void	(*glGetBufferParameteriv) (GLenum, GLenum, GLint*);
	void (*glVertexAttribPointer) (GLuint, GLint, GLenum, GLboolean, GLsizei, const GLvoid*);
	void (*glEnableVertexAttribArray) (GLuint);
	void (*glDisableVertexAttribArray) (GLuint);
	
};

#endif // HUD_H
