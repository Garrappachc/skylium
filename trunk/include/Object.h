/*
    Object.h
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

#ifndef __NO_OBJECT_MANAGEMENT__

#ifndef OBJECT_H
#define OBJECT_H

#include <iostream>
#include <vector>

#include "Vectors.h"
#include "Material.h"
#include "Mesh.h"

class Shader;
class SkyliumConfig;

enum {
	GET_VERTICES	= 1,
	GET_TEXTURE	= 2,
	GET_NORMALS	= 4,
	GET_MATERIAL	= 8
};

class Object {
	
	friend class Shader;

public:
	
	/**
	 * Konstruktor domyślny.
	 */
	Object(const std::string& = "");
	
	/**
	 * Konstruktor kopiujący.
	 */
	Object(const Object&);
	
	/**
	 * Niszczy wszystkie meshe oraz materiały.
	 */
	virtual ~Object();
	
	/**
	 * Obraca układ współrzędnych o zadane wartości, wyświetla obiekt.
	 */
	virtual void show();
	
	/**
	 * Przesuwa obiekt o dany wektor.
	 * @param x;
	 * @param y;
	 * @param z;
	 */
	void move(const GLdouble&, const GLdouble&, const GLdouble&);
	
	/**
	 * Skaluje obiekt o podane wartości.
	 * @param x;
	 * @param y;
	 * @param z;
	 */
	void scale(const GLdouble&, const GLdouble&, const GLdouble&);
	
	/**
	 * Obraca obiekt.
	 * @param rotX Kąt obrotu wokół osi X.
	 * @param rotY Kąt obrotu wokół osi Y.
	 * @param rotZ Kąt obrotu wokół osi Z.
	 */
	void rotate(const GLdouble&, const GLdouble&, const GLdouble&);
	
	/**
	 * Ustawia ogólny kolor obiektu.
	 * @param R Składnik czerwony.
	 * @param G Składnik zielony.
	 * @param B Składnik niebieski.
	 * @param T Przezroczystość obiektu.
	 * @return false, jeżeli wartość R, G, B lub A jest większa niż 1 i mniejsza niż 0.
	 */
	bool setColor(const GLfloat&, const GLfloat&, const GLfloat&, const GLfloat& = 1.0);
	
	/**
	 * Ustawia ogólny kolor obiektu.
	 * @param R Składnik czerwony.
	 * @param G Składnik zielony.
	 * @param B Składnik niebieski.
	 * @param T Przezroczystość obiektu.
	 * @return false, jeżeli wartość R, G lub B jest większa niż 255 i mniejsza niż 0 lub A jest spoza przedziału <0; 1>.
	 */
	bool setColor(const int&, const int&, const int&, const GLfloat& = 1.0);
	
	/**
	 * Wczytuje dane obiektu z pliku .obj i .mtl.
	 * @param objFile Lokalizacja pliku .obj.
	 * @param whatToLoad Definiuje, co ma się załadować. I tak:
	 *		&bull; GET_VERTICES - ładuje tylko współrzędne wierzchołków. Jest to domyślna opcja, jeżeli nie zostanie nadmieniona, to i tak będzie dodana;
	 *		&bull; GET_TEXTURE  - ładuje koordynaty tekstury oraz samą teksturę;
	 *		&bull; GET_NORMALS  - ładuje normalne;
	 *		&bull; GET_MATERIAL - ładuje dane materiału - z pliku .mtl.
	 * @return false, jeżeli cokolwiek poszło nie tak.
	 */
	bool loadFromObj(const std::string&, const unsigned&);
	
	/**
	 * Ładuje cały obiekt do bufora karty graficznej.
	 */
	virtual void loadIntoVBO();
	
	/**
	 * Szuka materiału po nazwie i zwraca do niego wskaźnik.
	 * @param name Nazwa szukanego materiału.
	 * @return Wskaźnik do instancji reprezentującej szukany materiał, lub 0, jeżeli nie znaleziono.
	 */
	Material * getMaterialByName(const std::string&);
	
	std::string name;
	
protected:
	
	sColor __defColor;
	
	sVector __mov;
	
	sVector __rot;
	
	sVector __scale;
	
	Shader * __shader;
	
private:
	
	std::vector< Mesh* > __meshes;
	
	std::vector< Mesh* >::const_iterator __meshesIterator;
	
	std::vector< Material* > __materials;
	
	bool __fileExists(const std::string&);
	
	void __parseObj(const std::string&, const unsigned&);
	
	void __parseMtl(const std::string&);
	
	SkyliumConfig * __config;
	
};

#endif // OBJECT_H
#endif // __NO_OBJECT_MANAGEMENT__