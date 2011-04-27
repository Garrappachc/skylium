/* 
 * File:   Object.h
 * Author: Michał Garapich
 *
 * Created on 2 april 2011, 11:17
 */

#ifndef OBJECT_H
#define	OBJECT_H

#include <iostream>
#include <GL/gl.h>
#include <vector>

#include "Vectors.h"

enum {
	GET_VERTICES = 1,
	GET_TEXTURE = 2,
	GET_NORMALS = 4,
	GET_MATERIAL = 8
};

typedef std::vector < GLfloat > sArray;

typedef std::vector < GLfloat > sVertex;

typedef std::vector < int > sCoords;

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
     * @param name nazwa obiektu;
     * @param vertex[] tablica wierzchołków;
	 * @param size liczba współrzędnych wierzchołków.
     */
	Object(const std::string&, const GLfloat*, const int&);

	/**
	 * Konstruktor obiektu. Przymuje już gotowe tablice wierzchołków.
     * @param name Nazwa obiektu.
     * @param vertex[] Tablica wierzchołków.
     */
	Object(const std::string&, const sArray&);

	/**
	 * Konstruktor, który jako argument przyjmuje tylko nazwę obiektu.
     * @param name Nazwa obiektu.
     */
	Object(const std::string&);
	
	Object();
	
	virtual ~Object();

	/**
	 * Renderuje obiekt.
     * @return false, jeżeli coś poszło nie tak.
     */
	virtual bool show();

	/**
	 * Przesuwa obiekt o dany wektor.
     * @param x;
     * @param y;
     * @param z;
	 * @return zawsze true.
     */
	virtual bool move(const GLdouble&, const GLdouble&, const GLdouble&);

	/**
	 * Skaluje obiekt o podane wartości.
     * @param x;
     * @param y;
     * @param z;
     * @return zawsze true.
     */
	virtual bool scale(const GLdouble&, const GLdouble&, const GLdouble&);

	/**
	 * Obraca obiekt.
     * @param rotX Kąt obrotu wokół osi X.
     * @param rotY Kąt obrotu wokół osi Y.
     * @param rotZ Kąt obrotu wokół osi Z.
     * @return zawsze true.
     */
	virtual bool rotate(const GLdouble&, const GLdouble&, const GLdouble&);

	/**
	 * Ładuje teksturę na obiekt.
     * @param textureFile lokalizacja pliku z teksturą;
     * @param texturePointers tablica koordynat tekstury;
	 * @param size liczba koordynat tekstury.
     * @return false, jeżeli coś poszło nie tak.
     */
	virtual bool loadTexture(const std::string&, const GLfloat*, const int&);

	/**
	 * Ładuje teksturę na obiekt.
     * @param textureFile Lokalizacja pliku z teksturą;
     * @param texturePointers Tablica koordynat tekstury;
     * @return false, jeżeli coś poszło nie tak.
     */
	virtual bool loadTexture(const std::string&, const sArray&);

	/**
	 * Ładuje teksturę na obiekt.
     * @param textureFile Lokalizacja w pliku z teksturą.
     * @return false, jeżeli coś poszło nie tak.
     */
	virtual bool loadTexture(const std::string&);
	
	/**
	 * Ustawia ogólny kolor obiektu.
     * @param R Składnik czerwony.
     * @param G Składnik zielony.
     * @param B Składnik niebieski.
     * @param T Przezroczystość obiektu.
     * @return false, jeżeli wartość R, G lub B jest większa niż 255 i mniejsza niż 0 lub T jest spoza przedziału <0; 1>.
     */
	virtual bool setColor(const GLfloat&, const GLfloat&, const GLfloat&, const GLfloat&);
	
	virtual bool setColor(const int&, const int&, const int&, const GLfloat&);

	/**
	 * Wczytuje dane obiektu z pliku .obj i .mtl.
     * @param objFile Lokalizacja pliku .obj.
     * @param mtlFile Lokalizacja pliku .mtl.
	 * @param whatToLoad Definiuje, co ma się załadować. I tak:
	 *		GET_VERTICES - ładuje tylko współrzędne wierzchołków. Jest to domyślna opcja, jeżeli nie zostanie nadmieniona, to i tak będzie dodana;
	 *		GET_TEXTURE  - ładuje koordynaty tekstury oraz samą teksturę;
	 *		GET_NORMALS  - ładuje normalne;
	 *		GET_MATERIAL - ładuje dane materiału - z pliku .mtl.
     * @return false, jeżeli cokolwiek poszło nie tak.
     */
	virtual bool loadFromObj(const std::string&, const std::string&, const unsigned int&);
	
	/**
	 * Pokazuje statystyki obiektu - przydatne przy debugowaniu.
     */
	virtual void showStats();


protected:

	/**
	 * Tablica wierzchołków. Wymiary 3 x X.
	 */
	sArray pointers_;

	/**
	 * Koordynaty tekstury.
	 */
	sArray texCoords_;

	/**
	 * Zmienna, która przechowuje wygenerowaną przez SOILa teksturę.
	 */
	GLuint texture_;
	
	/**
	 * Tablica normalnych.
	 */
	sArray normals_;
	
	bool hasNormals_;

	/**
	 * Tablica kolorów.
	 */
	sArray colors_;
	
	bool hasColors_;
	
	/**
	 * Ogólny kolor obiektu.
	 */
	sColor defColor_;

	/**
	 * Wektor przesunięcia obiektu.
	 * http://www.opengl.org/sdk/docs/man/xhtml/glTranslate.xml
	 */
	sVec3D < GLdouble > mov_;

	/**
	 * Kąt obrotu obiektu.
	 * http://www.opengl.org/sdk/docs/man/xhtml/glRotate.xml
	 */
	sVec3D < GLdouble > rot_;

	/**
	 * Wektor przybliżenia obiektu.
	 * http://www.opengl.org/sdk/docs/man/xhtml/glScale.xml
	 */
	sVec3D < GLdouble > scale_;
	
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

};

#endif	/* OBJECT_H */

