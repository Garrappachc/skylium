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
	 * Konstruktor domyślny, który jako argument przyjmuje tylko nazwę.
	 * @param name Nazwa mesh'u.
	 */
	Mesh(const std::string& = "");
	
	/**
	 * Konstruktor kopiujący.
	 */
	Mesh(const Mesh&);
	
	virtual ~Mesh();
	
	/**
	 * Renderuje mesh'a.
	 * http://www.opengl.org/sdk/docs/man/xhtml/glShadeModel.xml
	 * http://www.opengl.org/sdk/docs/man/xhtml/glDrawElements.xml
	 */
	void show();
	
	/**
	 * Wrzuca całą tablicę wierzchołków i indeksów do bufora karty graficznej.
	 * http://www.opengl.org/sdk/docs/man/xhtml/glGenBuffers.xml
	 * http://www.opengl.org/sdk/docs/man/xhtml/glBindBuffer.xml
	 * http://www.opengl.org/sdk/docs/man/xhtml/glBufferData.xml
	 * http://www.opengl.org/sdk/docs/man/xhtml/glBufferSubData.xml
	 * http://www.opengl.org/sdk/docs/man/xhtml/glTexCoordPointer.xml
	 * http://www.opengl.org/sdk/docs/man/xhtml/glNormalPointer.xml
	 * http://www.opengl.org/sdk/docs/man/xhtml/glVertexPointer.xml
	 */
	void loadIntoVbo();
	
	/**
	 * Ustawia wskaźnik na materiał na ten podany w argumencie.
	 */
	void useMtl(Material*);
	
	/*
	 * Zwraca wskaźnik na aktywny dla siebie materiał.
	 */
	Material * getMaterialPtr() { return __material; }
	
	/**
	 * Jeżeli true, włącza smooth shading.
	 */
	void smooth(const bool& = true);
	
	/**
	 * Wrzuca do wektora nowy wierzchołek i zwraca jego indeks.
	 */
	unsigned push_back(const Vertex&);
	
	/**
	 * Wrzuca do wektora nowy indeks.
	 */
	void addNewIdx(const int&);
	
	/**
	 * Ustawia __hasNormals na true.
	 */
	void enableNormals() { __hasNormals = true; }
	
	/**
	 * Zwraca rozmiar tablicy wierzchołków.
	 */
	size_t getSizeOfVertices() { return sizeof(Vertex) * __vertices.size(); }
	
	std::string name;
	
private:
	
	void __initGLExtensionsPointers();
	
	/* Wektor wierzchołków */
	std::vector< Vertex > __vertices;
	
	/* Miejsce w buforze dla wierzchołków */
	GLuint __vboID;
	
	/* Indeks wierzchołków */
	std::vector< GLushort > __index;
	
	/* Miejsce w buforze dla indeksów */
	GLuint __vboIndexID;
	
	/* Mamy normalne? */
	bool __hasNormals;
	
	/* Którego materiału używamy? */
	Material * __material;
	
	/* Jeżeli true, to glShadeModel(GL_SMOOTH) */
	bool __smooth;
	
	/* http://www.opengl.org/sdk/docs/man/xhtml/glBufferData.xml */
	GLenum __usage;
	
	/* http://www.opengl.org/sdk/docs/man/xhtml/glDrawElements.xml */
	GLenum __mode;
	
	/* Wskaźniki na rozszerzenia OpenGL'a */
	void	(*glBindBuffer) (GLenum, GLuint);
	void	(*glDeleteBuffers) (GLsizei, const GLuint*);
	void	(*glGenBuffers) (GLsizei, GLuint*);
	void	(*glBufferData) (GLenum, int, const GLvoid*, GLenum);
	void	(*glBufferSubData) (GLenum, GLintptr, GLsizeiptr, GLvoid*);
	void	(*glGetBufferParameteriv) (GLenum, GLenum, GLint*);
	
};

#endif // MESH_H
