/* 
 * File:   ObjectManager.h
 * Author: Michał Garapich
 *
 * Created on 2 april 2011, 12:10
 */

#ifndef OBJECTMANAGER_H
#define	OBJECTMANAGER_H

#include <iostream>
#include <cstdlib>
#include <vector>

#include "Object.h"

/**
 * Instancjami tej klasy są menedżery obiektów. Każda scena powinna mieć swojego jednego
 * menedżera, który umożliwia wyświetlenie wszystkich obiektów, dostęp do konkretnego
 * obiektu.
 */
class ObjectManager {
public:
	ObjectManager();
	~ObjectManager();

	/**
	 * Tworzy nowy oiekt.
     * @param name nazwa obiektu;
	 * @param pointers współrzędne wierchołków obiektu, rozmiar: 3;
	 * @param size ilość współrzędnych wierchołków.
     * @return wskaźnik do noewgo obiektu, ewentualnie 0, jeżeli coś poszło nie tak.
     */
	Object * newObject(const std::string&, const GLfloat*, const int&);

	/**
	 * Tworzy nowy oiekt.
     * @param vertex tablica wierzchołków, głębokość = 3;
	 * @param texture_file puste, jeżeli obiektowi nie przypisujemy tekstury;
     * @return wskaźnik do noewgo obiektu, ewentualnie 0, jeżeli coś poszło nie tak.
     */
	Object * newObject(const std::string&, const sArray&);

	/**
	 * Konstruktor, który wczytuje wierzchołki obiektu z pliku .obj.
     * @param name Nazwa obiektu.
     * @return wskaźnik do nowo utworzonego obiektu.
     */
	Object * newObject(const std::string&);

	/**
	 * Tworzy sobie on pudełko.
     * @param name nazwa pudełeczka;.
     * @return wskaźnik do pudełka.
     */
	Object * newBox(const std::string&);

	/**
	 * Wyświetla wszystkie obiekty na scenie.
     * @return false, jeżeli nie ma jeszcze żadnego obiektu.
     */
	bool showAll();

	/**
	 * Przeszukuje wektor po nazwach, szukając tej właściwej.
     * @param name nazwa szukanego obiektu;
     * @return wskaźnik do szukanego obiektu lub NULL, jeżeli nic nie znalazł.
     */
	Object * getObjectByName(const std::string&);
	
private:
	std::vector < Object* > objectList_;

	std::vector < Object* >::iterator objectIterator_;


};

#endif	/* OBJECTMANAGER_H */

