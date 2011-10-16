/*
     ___                    _    
    / __| __ ___ _ _  ___  | |_  
    \__ \/ _/ -_) ' \/ -_)_| ' \ 
    |___/\__\___|_||_\___(_)_||_|
                  
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


#ifndef SCENE_H
#define SCENE_H

#include <vector>

#include "Object.h"
#include "Camera.h"
#include "Light.h"
#include "Vectors.h"

class Scene {
	
public:
	
	/**
	 * Konstruktor domyślny.
	 * Puste wektory, puste wskaźniki.
	 * @param name Nazwa nowego obiektu.
	 */
	Scene(const std::string& = "");
	
	/**
	 * Niszczy wszystkie obiekty, kamery i światła.
	 */
	virtual ~Scene();
	
	/**
	 * Renderuje scenę.
	 * Kamerą jest aktywna kamera.
	 */
	void show();
	
//============ OBJECTS ============//
	
	/**
	 * Tworzy nowy obiekt o podanej nazwie i zwraca wskaźnik do niego.
	 * @param name Nazwa nowego obiektu.
	 * @param orig Wskaźnik na obiekt, który ma zostać skopiowany.
	 * @param parent Wskaźnik na obiekt, który ma być rodzicem.
	 * @return Wskaźnik do nowo utworzonego obiektu.
	 */
	Object * createObject(const std::string&, const Object* = (Object*)NULL, Object* = (Object*)NULL);
	
	/**
	 * Przeczesuje wektor w poszukiwaniu obiektu o zadanej nazwie.
	 * @param name Nazwa szukanego obiektu.
	 * @return Wskaźnik do znalezionego obiektu lub NULL, jeżeli nic nie znalazł.
	 */
	Object * getObjectByName(const std::string&);
	
//============ CAMERAS ============//
	
	/**
	 * Tworzy nową kamerę. Jeżeli nie ma jeszcze ustawionej aktywnej kamery,
	 * nowa kamera zostaje ustawiona jako aktywna.
	 * @param x Współrzędna x położenia kamery.
	 * @param y Współrzędna y położenia kamery.
	 * @param z Współrzędna z położenia kamery.
	 * @return Wskaźnik do nowo utworzonej kamery.
	 */
	Camera * createCamera(GLdouble, GLdouble, GLdouble, const cType& = FPP);
	
	/**
	 * Ustawia aktywną kamerę na tą podaną w argumencie.
	 * @param camera Wskaźnik na kamerę, która ma stać się aktywną kamerą.
	 * @param checking Sprawdza, czy kamera znajduje się w wektorze. Jeżeli false, sprawdzanie jest wyłączone, 
	 * 				co przy częstych zmianach kamery pozwala na zwiększenie wydajności.
	 * @return False, jeżeli checking = true i nie znaleziono danej kamery w wektorze.
	 */
	bool setActiveCamera(Camera*, bool = true);
	
	/**
	 * Zwraca wskaźnik na aktywną kamerę.
	 * @return Wskaźnik na aktywną kamerę.
	 */
	Camera * getActiveCamera() { return __activeCamera; }
	
//============ LIGHTS ============//
	
	/**
	 * Tworzy nowe światło. Parametry określają położenie źródła światła, domyślne wartości do 0, 0, 0.
	 * Światła są indeksowane, jak pozwala OpenGL - od 0 do 7.
	 * Funkcja szuka pierwszego wolnego miejsca na światło. Jeżeli mamy na przykład światła 0, 1, 2,
	 * potem wywołamy removeLight(1), a następnie utworzymy nowe światło, funkcja zwróci nam 1 jako
	 * indeks nowego światła.
	 * @param x Współrzędna x źródła światła.
	 * @param y Współrzędna y źródła światła.
	 * @param z Współrzędna z źródła światła.
	 * @return Indeks nowego światła.
	 */
	short createLight(GLfloat = 0, GLfloat = 0, GLfloat = 0);
	
	/**
	 * Ustawia parametry światła otoczenia.
	 * @param id Numer światła. Przedział [0; 7]. Kolory podajemy z przedziału 0-1.
	 * @param R Współczynnik <i>red</i> światła.
	 * @param G Współczynnik <i>green</i> światła.
	 * @param B Współczynnik <i>blue</i> światła.
	 * @param A Natężenie światła. 
	 * @return False, jeżeli nie znaleziono światła o podanym ID.
	 */
	bool setAmbientLight(int, GLfloat, GLfloat, GLfloat, GLfloat);
	
	/**
	 * Ustawia parametry światła rozproszonego.
	 * @param id Numer światła. Przedział [0; 7]. Kolory podajemy z przedziału 0-1.
	 * @param R Współczynnik <i>red</i> światła.
	 * @param G Współczynnik <i>green</i> światła.
	 * @param B Współczynnik <i>blue</i> światła.
	 * @param A Natężenie światła. 
	 * @return False, jeżeli nie znaleziono światła o podanym ID.
	 */
	bool setDiffuseLight(int, GLfloat, GLfloat, GLfloat, GLfloat);
	
	/**
	 * Ustawia parametry światła odbicia.
	 * @param id Numer światła. Przedział [0; 7]. Kolory podajemy z przedziału 0-1.
	 * @param R Współczynnik <i>red</i> światła.
	 * @param G Współczynnik <i>green</i> światła.
	 * @param B Współczynnik <i>blue</i> światła.
	 * @param A Natężenie światła. 
	 * @return False, jeżeli nie znaleziono światła o podanym ID.
	 */
	bool setSpecularLight(int, GLfloat, GLfloat, GLfloat, GLfloat);
	
	/**
	 * Ustawia położenie źródła światła.
	 * @param id Numer światła.
	 * @param X <i>X</i>.
	 * @param Y <i>Y</i>.
	 * @param Z <i>Z</i>.
	 * @return False, jeżeli nie znaleziono światła o podanym ID.
	 */
	bool setLightPosition(int, GLfloat, GLfloat, GLfloat);
	
	/**
	 * Przesuwa dane światło o podany wektor.
	 * @param id Numer światła.
	 * @param X Składowa X wektora przesunięcia.
	 * @param Y Składowa Y wektora przesunięcia.
	 * @param Z Składowa Z wektora przesunięcia.
	 * @return False, jeżeli nie znaleziono światła o podanym ID.
	 */
	bool moveLight(int, GLfloat, GLfloat, GLfloat);
	
	/**
	 * Włącza lub wyłącza oświetlenie całej sceny.
	 */
	void toggleLight();
	
	/**
	 * Włącza lub wyłącza konkretne światło.
	 * @param id ID światła.
	 * @return False, jeżeli światła nie znaleziono.
	 */
	bool toggleLight(int);
	
	/**
	 * Usuwa światło ze sceny. Jeżeli usunięte zostaje światło ze środka, id pozostałych
	 * świateł nie zmienia się, ale nowe światło zostanie umieszczone w miejscu tego,
	 * które zostało tutaj usunięte. Na przykład mamy trzy światła: 0, 1, 2. Usuwamy
	 * światło 1 - mamy teraz światła 0, 2. Tworzymy nowe światło i jego id to 1,
	 * jako że 1 nie jest tutaj używane.
	 * @param id ID światła do usunięcia. Domyślna wartość to ostatnie znane światło.
	 * @return False, jeżeli nie ma światła o podanym id.
	 */
	bool removeLight(int = -1);
	
	/* Nazwa sceny */
	std::string name;
	
private:
	/* Wyświetla wszystkie obiekty. */
	void __setObjects();
	
	/* Ustawia światła na scenie. 
	 * http://www.opengl.org/sdk/docs/man/xhtml/glLight.xml */
	void __setLights();
	
	/* Kończy renderowanie ramki, ustawia wasShown = false
	 * dla wszystkich obiektów. */
	void __endFrame();
	
	/* Wektor obiektów - rodziców, które przynależą do sceny.*/
	std::vector< Object* > __objectList;
	
	/* Iterator po wszystkich obiektach.
	 * Wykorzystywany w metodzie __setObjects() */
	std::vector< Object* >::const_iterator __objectIterator;
	
	/* Wektor kamer. */
	std::vector< Camera* > __cameraList;
	
	/* Wskaźnik na aktywną kamerę. */
	Camera * __activeCamera;
	
	/* Wektor świateł sceny. */
	std::vector< Light* > __lightList;
	
	/* Iterator po wszystkich światłach, które przynależą do sceny.
	 * Patrz metoda __setLights(). */
	std::vector< Light* >::const_iterator __lightIterator;
	
	/* Czy oświetlenie sceny jest włączone? */
	bool __isLightOn;

};

#endif // SCENE_H
