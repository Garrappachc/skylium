/* 
 * SceneManager.h
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

#ifndef SCENEMANAGER_H
#define	SCENEMANAGER_H

#include <cstdlib>
#include <vector>

#include "Scene.h"
#include "Singleton.h"

/**
 * Menedżer scen. Tylko jedna instancja tej klasy jest możliwa.
 * Zarządza scenami, ustawia jedną jako aktywną.
 */
class SceneManager : public Singleton < SceneManager > {
public:
	SceneManager();
	virtual ~SceneManager();

	/**
	 * Tworzy nową scenę. Parametry użytkownik sobie ustawi później,
	 * nie trzeba tego zwalać na konstruktor sceny.
	 * Jeżeli jest to pierwsza scena, to przypiszę ją jako aktywną.
	 * @param name nazwa nowej sceny;
	 * @return wskaźnik do nowo utworzonej sceny.
	 */
	Scene * createScene(const std::string&);

	/**
	 * Wyświetla aktywną scenę.
	 * @return false, jeżeli nie ma żadnej sceny.
	 */
	bool displayActiveScene();

	/**
	 * Ustawia daną scenę jako aktywną. Ustawia jej wskaźnik
	 * na *activeScene_.
	 * @param nazwa sceny lub wskaźnik do niej;
	 * @return false, jeżeli dana scena nie istnieje.
	 */
	bool setActive(const std::string&);
	bool setActive(const Scene*);

	/**
	 * Szuka sceny po nazwie.
	 * @param name nazwa sceny, którą ma znaleźć;
	 * @return wskaźnik do tej sceny lub 0, jeżeli takiej nie ma.
	 */
	Scene * getSceneByName(const std::string&);

	Scene * getActiveScene();

private:
	std::vector < Scene* > __sceneList;

	Scene * __activeScene;

};

#endif	/* SCENEMANAGER_H */

