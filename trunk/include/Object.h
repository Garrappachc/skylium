/* 
 * Object.h
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

#ifndef OBJECT_H
#define	OBJECT_H

#include <iostream>
#include <GL/gl.h>
#include <vector>

#include "Vectors.h"
#include "PolygonGroup.h"

enum {
	GET_VERTICES = 1,
	GET_TEXTURE = 2,
	GET_NORMALS = 4,
	GET_MATERIAL = 8
};

class Shader;

/**
 * Klasa, której instancjami są osobne obiekty na scenie.
 * Każdym obiektem można przesuwać, obracać, etc, etc.
 * Każdy też musi mieć swój kształt, nazwę, czasami teksturę.
 */
class Object {
	
	friend class Shader;
	
public:
	/**
	 * Nazwa obiektu - musi być unikalna.
	 */
	std::string name;

	/**
	 * Konstruktor obiektu. Przymuje już gotowe tablice wierzchołków.
     * @param name Nazwa obiektu;
     * @param pointers[] Tablica wierzchołków;
	 * @param size Liczba współrzędnych wierzchołków.
     */
	Object(const std::string&, const GLfloat*, const int&);

	/**
	 * Konstruktor obiektu. Przymuje już gotowe tablice wierzchołków.
     * @param name Nazwa obiektu.
     * @param pointers Tablica wierzchołków.
     */
	Object(const std::string&, const sArray&);

	/**
	 * Konstruktor, który jako argument przyjmuje tylko nazwę obiektu.
     * @param name Nazwa obiektu.
     */
	Object(const std::string&);
	
	Object();
	
	~Object();

	/**
	 * Renderuje obiekt.
     */
	void show();

	/**
	 * Przesuwa obiekt o dany wektor.
     * @param x;
     * @param y;
     * @param z;
	 * @return zawsze true.
     */
	bool move(const GLdouble&, const GLdouble&, const GLdouble&);

	/**
	 * Skaluje obiekt o podane wartości.
     * @param x;
     * @param y;
     * @param z;
     * @return zawsze true.
     */
	bool scale(const GLdouble&, const GLdouble&, const GLdouble&);

	/**
	 * Obraca obiekt.
     * @param rotX Kąt obrotu wokół osi X.
     * @param rotY Kąt obrotu wokół osi Y.
     * @param rotZ Kąt obrotu wokół osi Z.
     * @return zawsze true.
     */
	bool rotate(const GLdouble&, const GLdouble&, const GLdouble&);

	/**
	 * Ładuje teksturę na obiekt.
     * @param textureFile lokalizacja pliku z teksturą;
     * @param texturePointers tablica koordynat tekstury;
	 * @param size liczba koordynat tekstury.
     * @return false, jeżeli coś poszło nie tak.
     */
	bool loadTexture(const std::string&, const GLfloat*, const int&);

	/**
	 * Ładuje teksturę na obiekt.
     * @param textureFile Lokalizacja pliku z teksturą;
     * @param texturePointers Tablica koordynat tekstury;
     * @return false, jeżeli coś poszło nie tak.
     */
	bool loadTexture(const std::string&, const sArray&);
	
	/**
	 * Ustawia ogólny kolor obiektu.
     * @param R Składnik czerwony.
     * @param G Składnik zielony.
     * @param B Składnik niebieski.
     * @param T Przezroczystość obiektu.
     * @return false, jeżeli wartość R, G lub B jest większa niż 255 i mniejsza niż 0 lub T jest spoza przedziału <0; 1>.
     */
	bool setColor(const GLfloat&, const GLfloat&, const GLfloat&, const GLfloat&);
	
	bool setColor(const int&, const int&, const int&, const GLfloat&);

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
	bool loadFromObj(const std::string&, const unsigned int&);
	
	void printPointers();


protected:
	
	/* Wektor grup poligonów */
	std::vector< PolygonGroup* > pGroups_;
	
	/* Iterator */
	std::vector< PolygonGroup* >::iterator pGroupsIterator_;
	
	/**
	 * Ogólny kolor obiektu.
	 */
	sColor defColor_;

	/**
	 * Wektor przesunięcia obiektu.
	 * http://www.opengl.org/sdk/docs/man/xhtml/glTranslate.xml
	 */
	sVector mov_;

	/**
	 * Kąt obrotu obiektu.
	 * http://www.opengl.org/sdk/docs/man/xhtml/glRotate.xml
	 */
	sVector rot_;

	/**
	 * Wektor przybliżenia obiektu.
	 * http://www.opengl.org/sdk/docs/man/xhtml/glScale.xml
	 */
	sVector scale_;
	
	/**
	 * Wskaźnik na shader, którego obiekt ma używać.
	 */
	Shader * shader_;

private:

	/**
	 * Funkcja pomocnicza - sprawdza, czy istnieje podany plik.
     * @param Nazwa pliku.
     * @return True, jeżeli istnieje, w przeciwnym przypadku false.
     */
	bool fileExists(const std::string&);
	
	/**
	 * Parsuje plik .obj i zwraca true, jeżeli parsowanie się udało.
     * @param fileName Nazwa pliku .obj.
	 * @param whatToLoad Definiuje, co ma się załadować. I tak:
	 *		&bull; GET_VERTICES - ładuje tylko współrzędne wierzchołków. Jest to domyślna opcja, jeżeli nie zostanie nadmieniona, to i tak będzie dodana;
	 *		&bull; GET_TEXTURE  - ładuje koordynaty tekstury oraz samą teksturę;
	 *		&bull; GET_NORMALS  - ładuje normalne;
	 *		&bull; GET_MATERIAL - ładuje dane materiału - z pliku .mtl.
     */
	void parseObj(const std::string&, const unsigned int&);
	
	/**
	 * Parsuje plik .mtl i zwraca true, jeżeli parsowanie się udało.
     * @param fileName Nazwa pliku .mtl.
     */
	void parseMtl(const std::string&);
	
	/* Wektor materiałów */
	std::vector< Material* > materials_;

};

#endif	/* OBJECT_H */

