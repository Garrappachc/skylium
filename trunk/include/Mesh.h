/*
     __  __        _      _    
    |  \/  |___ __| |_   | |_  
    | |\/| / -_|_-< ' \ _| ' \ 
    |_|  |_\___/__/_||_(_)_||_|
                           
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


#ifndef MESH_H
#define MESH_H

#include <vector>

#include <GL/gl.h>

#include "Vertex.h"
#include "Material.h"

#define GL_ARRAY_BUFFER 0x8892
#define GL_BUFFER_SIZE 0x8764
#define GL_ELEMENT_ARRAY_BUFFER 0x8893
#define GL_STATIC_DRAW 0x88E4

class Mesh {
	
public:
	
	/**
	 * Default ctor, gets only name.
	 * @param name Mesh's name.
	 */
	Mesh(const std::string& = "");
	
	/**
	 * Copy ctor.
	 */
	Mesh(const Mesh&);
	
	/**
	 * Empties the VBO (if used), some output.
	 */
	virtual ~Mesh();
	
	/**
	 * Sets shader data.
	 */
	void setAllParams();
	
	/**
	 * Renders the mesh.
	 * http://www.opengl.org/sdk/docs/man/xhtml/glShadeModel.xml
	 * http://www.opengl.org/sdk/docs/man/xhtml/glDrawElements.xml
	 */
	void show();
	
	/**
	 * Sends all of the vertices to GPU's buffer.
	 */
	void loadIntoVbo();
	
	/**
	 * Sets active material to this given.
	 */
	void useMtl(Material*);
	
	/**
	 * Returns pointer to the active material.
	 */
	Material * getMaterialPtr() { return __material; }
	
	/**
	 * If true, the smooth shading is set on.
	 */
	void smooth(bool = true);
	
	/**
	 * Puts into the vector the next Vertex and returns its index.
	 * @param v Vertex to be put.
	 * @return The new vertex's index.
	 */
	unsigned push_back(const Vertex&);
	
	/**
	 * Puts the new index to the vector.
	 */
	void addNewIdx(int);
	
	/**
	 * This is used to add these faces, which have 4 indices - it adds
	 * the first, third and the last indices.
	 */
	void addThreeIdxs(int);
	
	/**
	 * Sets __hasNormals to true.
	 */
	void enableNormals() { __hasNormals = true; }
	
	/**
	 * Returns size of vertices' vector in bytes.
	 */
	size_t getSizeOfVertices() { return sizeof(Vertex) * __vertices.size(); }
	
	std::string name;
	
private:
	
	void __initGLExtensionsPointers();
	
	/* Vertices' vector */
	std::vector< Vertex > __vertices;
	
	/* Vertices' room in VBO */
	GLuint __vboID;
	
	/* Vertices' index */
	std::vector< GLushort > __indices;
	
	/* Indices' room in VBO */
	GLuint __vaoID;
	
	/* Do we have normals? */
	bool __hasNormals;
	
	/* Material that has to be used */
	Material * __material;
	
	/* If true, then glShadeModel(GL_SMOOTH) */
	bool __smooth;
	
	/* http://www.opengl.org/sdk/docs/man/xhtml/glBufferData.xml */
	GLenum __usage;
	
	/* http://www.opengl.org/sdk/docs/man/xhtml/glDrawElements.xml */
	GLenum __mode;
	
	/* GL's extensions' pointers */
	void	(*glGenBuffers) (GLsizei, GLuint*);
	void	(*glBindBuffer) (GLenum, GLuint);
	void	(*glBufferData) (GLenum, int, const GLvoid*, GLenum);
	void	(*glDeleteBuffers) (GLsizei, const GLuint*);
	void	(*glBufferSubData) (GLenum, GLintptr, GLsizeiptr, GLvoid*);
	void	(*glGetBufferParameteriv) (GLenum, GLenum, GLint*);
	
};

#endif // MESH_H
