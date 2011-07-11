/*
     ___                  __  __                              _    
    / __| __ ___ _ _  ___|  \/  |__ _ _ _  __ _ __ _ ___ _ _ | |_  
    \__ \/ _/ -_) ' \/ -_) |\/| / _` | ' \/ _` / _` / -_) '_|| ' \ 
    |___/\__\___|_||_\___|_|  |_\__,_|_||_\__,_\__, \___|_|(_)_||_|
                                               |___/    

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


#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include <vector>

#include "Singleton.h"
#include "Scene.h"

class SceneManager : public Singleton< SceneManager > {
	
public:
	
	/**
	 * Konstruktor - nie ma scen, aktywna scena nie istnieje.
	 */
	SceneManager();
	
	/**
	 * Usuwa cały wektor __sceneList.
	 */
	virtual ~SceneManager();
	
	/**
	 * Tworzy nową scenę o podanej nazwie i zwraca wskaźnik do niej.
	 * Jeżeli nie ma aktywnej sceny, ta scena staję się automatycznie aktywną.
	 * @param name Nazwa nowej sceny.
	 * @return Wskaźnik do nowo utworzonej sceny.
	 */
	Scene * createScene(const std::string&);
	
	/**
	 * Renderuje aktywną scenę.
	 */
	void displayActiveScene();
	
	/**
	 * Ustawia aktywną scenę na tą, której wskaźnik poadny został jako argument.
	 * @param toSet Wskaźnik na scenę, która ustawiona zostanie jako aktywna.
	 * @return False, jeżeli nie udało się ustawić aktywnej sceny. Patrz -> log.
	 */
	bool setActive(const Scene*);
	
	/**
	 * Zwraca wskaźnik na aktywną scenę.
	 * @return Wskaźnik na aktywną scenę.
	 */
	Scene * getActiveScene() { return __activeScene; }
	
private:
	
	std::vector< Scene* > __sceneList;
	
	Scene * __activeScene;
	
};

#endif // SCENEMANAGER_H
