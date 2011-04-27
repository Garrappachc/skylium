/* 
 * File:   SceneManager.h
 * Author: kist
 *
 * Created on 2 kwiecień 2011, 18:05
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
	std::vector < Scene* > sceneList_;

	Scene * activeScene_;

};

#endif	/* SCENEMANAGER_H */

