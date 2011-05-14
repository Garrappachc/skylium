/* 
 * Scene.h
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

#ifndef SCENE_H
#define	SCENE_H

#include <iostream>

#include "Object.h"
#include "Box.h"
#include "Camera.h"
#include "Vectors.h"
#include "Light.h"

/**
 * Instancjami tej klasy są sceny. Każda scena ma swojego ObjectManagera. Do sceny
 * należy transformacja jej przed wyświetleniem wszystkich obiektów, ustawienie terenu
 * (podłoża), nieba (oczywiście, tylko wtedy, jeżeli użytkownik tego sobie zażyczy).
 * Scena powinna też sprawdzać, który obiekt jest poza kamerą, by uniknąć niepotrzebnego
 * renderowania.
 */
class Scene {
	
public:
	
	Scene();
	
	/**
	 * Konstruktor.
	 * @param name - nazwa sceny.
	 */
	Scene(const std::string&);

	/**
	 * Destruktor, który powinien zniszczyć wszystkie obiekty.
	 */
	virtual ~Scene();
		
	/**
	 * Renderuje scenę.
	 */
	void show();

#ifndef __NO_OBJECT_MANAGEMENT__ // Zarządzanie obiektami na scenie

	/**
	 * Tworzy nowy oiekt.
	 * @param name nazwa obiektu;
	 * @param pointers współrzędne wierchołków obiektu, rozmiar: 3;
	 * @param size ilość współrzędnych wierchołków.
	 * @return wskaźnik do noewgo obiektu, ewentualnie 0, jeżeli coś poszło nie tak.
	 */
	Object * createObject(const std::string&, const GLfloat*, const int&);

	/**
	 * Tworzy nowy oiekt.
	 * @param name Nazwa obiektu.
	 * @param pointers Tablica wierzchołków, głębokość = 3;
	 * @return wskaźnik do nowego obiektu, ewentualnie 0, jeżeli coś poszło nie tak.
	 */
	Object * createObject(const std::string&, const sArray&);

	/**
	 * Konstruktor, który tworzy pusty obiekt o podanej nazwie.
	 * @param name Nazwa obiektu.
	 * @return wskaźnik do nowo utworzonego obiektu.
	 */
	Object * createObject(const std::string&);

	/**
	 * Tworzy pudełko.
	 * @param name Nazwa pudełka.
	 * @return wskaźnik do boxa.
	 */
	Object * createBox(const std::string&);
	
	/**
	 * Przeszukuje wektor po nazwach, szukając tej właściwej.
	 * @param name nazwa szukanego obiektu;
	 * @return wskaźnik do szukanego obiektu lub 0, jeżeli nic nie znalazł.
	 */
	Object * getObjectByName(const std::string&);
#endif // __NO_OBJECT_MANAGEMENT__
	
#ifndef __NO_CAMERA_MANAGEMENT__ // Zarządzanie kamerami

	/**
	 * Tworzy kamerę.
	 * @param X Pozycja X;
	 * @param Y Pozycja Y;
	 * @param Z Pozycja Z;
	 * @return wskaźnik do kamerki.
	 */
	Camera * createCamera(const GLdouble&, const GLdouble&, const GLdouble&);

	/**
     * @return Wskaźnik na aktywną dla danej sceny kamerę.
	 */
	Camera * getActiveCamera();
#endif // __NO_CAMERA_MANAGEMENT__
	
#ifndef __NO_LIGHTING_MANAGEMENT__ // Zarządzanie kamerami
	
	/**
	 * Kreuje nowe źródło światła i zwraca jego id. Światło jest domyślnie włączone i znajduje się w pozycji (0, 0, 0).
	 * Aby wyłączyć, zobacz metodę toggleLight().
	 * @return Id nowego światła (0 - 7).
	 */
	int createLight();
	
	/**
	 * Kreuje nowe źródło światła i zwraca jego id. Światło jest domyślnie włączone.
	 * Aby wyłączyć, zobacz metodę toggleLight().
	 * @param pos Pozycja źródłą nowego światła.
	 * @return Id nowego światła (0-7).
	 */
	int createLight(const sVec3D< GLfloat >&);
	
	/**
	 * Kreuje nowe źródło światła i zwraca jego id. Światło jest domyślnie włączone.
	 * Aby wyłączyć, zobacz metodę toggleLight().
	 * @param X Współrzędna X pozycji nowego światła.
	 * @param Y Współrzędna Y pozycji nowego światła.
	 * @param Z Współrzędna Z pozycji nowego światła.
	 * @return Id nowego światła (0-7) lub -1, jeżeli mamy próbę utworzenia 9. światła.
	 */
	int createLight(const GLfloat&, const GLfloat&, const GLfloat&);

	/**
	 * Ustawia parametry światła otoczenia.
	 * @param id Numer światła. Przedział [0; 7]. Kolory podajemy z przedziału 0-1.
	 * @param R Współczynnik <i>red</i> światła.
	 * @param G Współczynnik <i>green</i> światła.
	 * @param B Współczynnik <i>blue</i> światła.
	 * @param A Natężenie światła. 
	 * @return False, jeżeli nie znaleziono światła o podanym ID.
	 */
	bool setAmbientLight(const int&, const GLfloat&, const GLfloat&, const GLfloat&, const GLfloat&);

	/**
	 * Ustawia parametry światła rozproszonego.
	 * @param id Numer światła. Przedział [0; 7]. Kolory podajemy z przedziału 0-1.
	 * @param R Współczynnik <i>red</i> światła.
	 * @param G Współczynnik <i>green</i> światła.
	 * @param B Współczynnik <i>blue</i> światła.
	 * @param A Natężenie światła. 
	 * @return False, jeżeli nie znaleziono światła o podanym ID.
	 */
	bool setDiffuseLight(const int&, const GLfloat&, const GLfloat&, const GLfloat&, const GLfloat&);

	/**
	 * Ustawia parametry światła odbicia.
	 * @param id Numer światła. Przedział [0; 7]. Kolory podajemy z przedziału 0-1.
	 * @param R Współczynnik <i>red</i> światła.
	 * @param G Współczynnik <i>green</i> światła.
	 * @param B Współczynnik <i>blue</i> światła.
	 * @param A Natężenie światła. 
	 * @return False, jeżeli nie znaleziono światła o podanym ID.
	 */
	bool setSpecularLight(const int&, const GLfloat&, const GLfloat&, const GLfloat&, const GLfloat&);
	

	/**
	 * Ustawia położenie źródła światła.
	 * @param id Numer światła.
	 * @param X <i>X</i>.
	 * @param Y <i>Y</i>.
	 * @param Z <i>Z</i>.
	 * @return False, jeżeli nie znaleziono światła o podanym ID.
	 */
	bool setLightPosition(const int&, const GLfloat&, const GLfloat&, const GLfloat&);
	
	/**
	 * Włącza lub wyłącza oświetlenie sceny.
	 */
	void toggleLight();
	
	/**
	 * Włącza lub wyłącza konkretne światło.
	 * @param id ID światła.
	 * @return False, jeżeli światła nie znaleziono.
	 */
	bool toggleLight(const int&);
	
	/**
	 * Usuwa światło ze sceny. Jeżeli usunięte zostaje światło ze środka, id pozostałych
	 * świateł nie zmienia się, ale nowe światło zostanie umieszczone w miejscu tego,
	 * które zostało tutaj usunięte. Na przykład mamy trzy światła: 0, 1, 2. Usuwamy
	 * światło 1 - mamy teraz światła 0, 2. Tworzymy nowe światło i jego id to 1,
	 * jako że 1 nie jest tutaj używane.
	 * @param id ID światła do usunięcia. Domyślna wartość to ostatnie znane światło.
	 * @return False, jeżeli nie ma światła o podanym id.
	 */
	bool removeLight(const int&);
	
#endif // __NO_LIGHTING_MANAGEMENT__

	/* Nazwa sceny. */
	std::string name;

private:

#ifndef __NO_OBJECT_MANAGEMENT__
	
	/**
	 * Wektor wskaźników na obiekty, które są na scenie.
	 */
	std::vector < Object* > __objectList;

	/**
	 * Iterator po obiektach sceny.
	 */
	std::vector < Object* >::const_iterator __objectIterator;
	
	/**
	 * Wyświetla wszystkie obiekty.
     */
	void __setObjects();
#endif

#ifndef __NO_CAMERA_MANAGEMENT__
	/**
	 * Wektor, który przechowuje wkaźniki do wszystkich kamer.
	 */
	std::vector < Camera* > __cameraList;

	/**
	 * Wskaźnik do aktywnej kamery.
	 */
	Camera * __activeCamera;
#endif
	
#ifndef __NO_LIGHTING_MANAGEMENT__
		
	/**
	 * Wektor wskaźników na światła sceny. Ma osiem elementów, przy tworzeniu sceny
	 * każdy wskaźnik otrzymuje adres 0.
	 */
	std::vector < Light* > __lightList;
	
	/**
	 * Iterator po światłach.
	 */
	std::vector < Light* >::const_iterator __lightIterator;
	
	/**
	 * Jeżeli jest true, to przy renderowaniu mamy wywołanie glEnable(GL_LIGHTING).
	 */
	bool __isLightOn;
	
	/**
	 * Ustawia światła na scenie.
     */
	void __setLights();
#endif

};

#endif	/* SCENE_H */

