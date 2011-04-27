/* 
 * File:   main.cpp
 * Authors: Michał Garapich, Eryk Dykiel
 *
 * Created on 1 kwiecień 2011, 18:04
 */

#include <cstdlib>
#include <iostream>
#ifdef _WIN32
#include <GL/GLee.h>
#else
#include <GLee.h>
#endif
#include <SDL/SDL.h>
#include <SOIL/SOIL.h>

#include "include/Camera.h"
#include "include/Skylium.h"
#include "include/Scene.h"
#include "include/Shader.h"

using namespace std;

GLfloat boxTex[] = {
	0, 0,
	1, 0,
	1, 1,
	0, 1,

	1, 0,
	0, 0,
	0, 1,
	1, 1,

	0, 0,
	1, 0,
	1, 1,
	0, 1,

	0, 0,
	1, 0,
	1, 1,
	0, 1,

	1, 0,
	1, 1,
	0, 1,
	0, 0,

	0, 0,
	1, 0,
	1, 1,
	0, 1
};

int
main() {

	Skylium *ikatka = Skylium::instance();

	if (!ikatka -> init()) {
		cout << "Błąd przy Skylium::init(). Przerywam.\n\n";
		return 1;
	}
	
	Shader *cienie = new Shader("shaders/shadow.vert", "shaders/shadow.frag");
	if (!cienie -> make())
		exit(1);

	Scene *scenka = ikatka -> createScene("Scenka");
	//scenka -> setSpecularLight(0.99, 0.99, 0.99, 1.0);
	//scenka -> setDiffuseLight(0.9, 0.9, 0.9, 1.0);
	scenka -> setAmbientLight(0.5, 0.5, 0.5, 1.0);
	//scenka -> setLightPosition(6.0, 6.0, 0.0);
	//scenka -> toggleLight();

	Object *pudelko = scenka -> createBox("Pudelko");
	pudelko -> scale(-3, -3, -3);
	pudelko -> move(-10, 0, 0);
	if (!pudelko -> loadTexture("texture/box.jpg", boxTex, 48))
		cout << "Nie udało się załadować tekstury!";

	Object *malpka = scenka -> createObject("malpka");
	malpka->loadFromObj("objects/monkey.obj", "objects/monkey.mtl", GET_VERTICES | GET_NORMALS);
	if (!malpka) {
		cout << "Malpka nie działa.\n";
	}
	malpka -> move(-10, 7, 0);
	malpka -> scale(3, 3, 3);
	malpka -> rotate(90, 0, -90);
	malpka -> setColor(126, 54, 25, 1);
	
	//Object *kulka = scenka -> createObject("kulka");
	//kulka->loadFromObj("objects/sphere.obj", "objects/sphere.mtl", GET_VERTICES | GET_NORMALS);
	//if (!kulka) {
	//	cout << "Kulka nie działa.\n";
	//}
	//kulka -> move(-5, 2, 0);
	//kulka -> scale(-2, -2, -2);
	//kulka -> setColor(0, 56, 109, 1);
	
	Camera* kamerka = scenka -> createCamera(5.0, 6.0, 0.0);
	kamerka -> lookAt(1, 4, -1);
	
	cienie -> bind(malpka);

	pudelko ->showStats();
	malpka ->showStats();
	
	ikatka -> execute();

	return 0;
}

