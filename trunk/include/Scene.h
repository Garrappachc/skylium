/* 
 * File:   Scene.h
 * Author: Michał Garapich
 *
 * Created on 2 kwiecień 2011, 18:07
 */

#ifndef SCENE_H
#define	SCENE_H

#include <iostream>

#include "ObjectManager.h"
#include "Camera.h"
#include "Vectors.h"

/**
 * Instancjami tej klasy są sceny. Każda scena ma swojego ObjectManagera. Do sceny
 * należy transformacja jej przed wyświetleniem wszystkich obiektów, ustawienie terenu
 * (podłoża), nieba (oczywiście, tylko wtedy, jeżeli użytkownik tego sobie zażyczy).
 * Scena powinna też sprawdzać, który obiekt jest poza kamerą, by uniknąć niepotrzebnego
 * renderowania.
 */
class Scene {
public:
	/**
	 * Konstruktor. Tworzy nową instancję ObjectManagera.
	 * @param name - nazwa sceny.
	 */
	Scene(const std::string&);

	/**
	 * Destruktor, który powinien zniszczyć wszystkie obiekty i ObjectManagera.
	 */
	virtual ~Scene();

	/**
	 * Nakazuje ObjectManagerowi pokazać wszystkie obiekty.
	 * @return false, jeżeli na scenie nic nie ma.
	 */
	bool showAllObjects();

	/**
	 * Tworzy nowy obiekt.
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
	 * Tworzy kamerę.
	 * @param pozycja X;
	 * @param pozycja Y;
	 * @param pozycja Z;
	 * @return wskaźnik do kamerki.
	 */
	Camera * createCamera(const GLdouble&, const GLdouble&, const GLdouble&);

	Camera * getActiveCamera();

	/**
	 * Ustawia parametry światła.
	 * @param ambient Ambient light.
	 * @param diffuse Diffuse light.
	 * @param specular Specular light.
	 */
	bool setLights(const sColor&, const sColor&, const sColor&);
	
	/**
	 * Ustawia parametr światła ambient.
     */
	bool setAmbientLight(const GLfloat&, const GLfloat&, const GLfloat&, const GLfloat&);
	
	/**
	 * Ustawia parametr światła diffuse.
     */
	bool setDiffuseLight(const GLfloat&, const GLfloat&, const GLfloat&, const GLfloat&);
	
	/**
	 * Ustawia parametr światła specular.
     */
	bool setSpecularLight(const GLfloat&, const GLfloat&, const GLfloat&, const GLfloat&);
	
	/**
	 * Ustawia współrzędne źródła światła.
	 */
	bool setLightPosition(const GLfloat&, const GLfloat&, const GLfloat&);
	
	/**
	 * Włącza lub wyłącza oświetlenie sceny.
     * @return zawsze true.
     */
	bool toggleLight();

	std::string name;

private:

	/**
	 * Wskaźnik do instancji ObjectManagera.
	 */
	ObjectManager * objectManagement_;

	/**
	 * Wektor, który przechowuje wkaźniki do wszystkich kamer.
	 */
	std::vector < Camera* > cameraList_;

	/**
	 * Wskaźnik do aktywnej kamery.
	 */
	Camera * activeCamera_;

	bool isLightOn_;
	sColor ambientLight_;
	sColor diffuseLight_;
	sColor specularLight_;
	sVec3D < GLfloat > lightSrc_;

};

#endif	/* SCENE_H */

