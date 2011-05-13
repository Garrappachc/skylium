/* 
 * PolygonGroup.h
 * Copyright (C) 2011 Michał Garapich
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see http://www.gnu.org/licenses/.
 */

#ifndef POLYGONGROUP_H
#define	POLYGONGROUP_H

#include "GL/gl.h"

#include "Vectors.h"
#include "Material.h"

class Object;
class Box;

class PolygonGroup {
	
	friend class Object;
	friend class Box;
	
public:
	
	/**
	 * Konstruktor domyślny, tworzy pustą tablicę wierzchołków, pustą tablicę
	 * współrzędnych tekstury, pustą tablicę normalnych oraz pusty wskaźnik
	 * na używany materiał.
     */
	PolygonGroup();
	
	/**
	 * Konstruktor, który jako argument przyjmuje tylko nazwę.
     * @param name Nazwa grupy.
     */
	PolygonGroup(const std::string&);
	
	/**
	 * Konstruktor, który jako argument przyjmuje tablicę wierzchołków - patrz
	 * konstruktor klasy Object.
	 * @param name Nazwa grupy.
     * @param pointers Tablica współrzędnych (x, y, z) wierzchołków grupy.
     * @param size Rozmiar powyższej tablicy.
     */
	PolygonGroup(const std::string&, const GLfloat*, const int&);
	
	/**
	 * Konstruktor, który jako argument przyjmuje wektor współrzędnych grupy.
	 * @param name Nazwa grupy.
     * @param pointers Wektor współrzędnych grupy ( vector<GLfloat> ).
     */
	PolygonGroup(const std::string&, const sArray&);
	
	~PolygonGroup();
	
	/**
	 * Wyświetla daną grupę poligonów.
     */
	void show();
	
	/* Nazwa grupy */
	std::string name;
	
private:
	
	/* Tablica wierzchołków. */
	sArray __pointers;
	
	/* Koordynaty tekstury. */
	sArray __texCoords;
	
	/* Tablica normalnych. */
	sArray __normals;
	
	/* Mamy w ogóle normalne? */
	bool __hasNormals;
	
	/* Wskaźnik na używany materiał. */
	Material * __material;
	
	/* Jeżeli true, to glShadeModel(GL_SMOOTH) */
	bool __smooth;
	
	/* Prymitywy */
	GLenum __mode;

};

#endif	/* POLYGONGROUP_H */

