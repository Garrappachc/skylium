/* 
 * File:   main.cpp
 * Authors: Michał Garapich, Eryk Dykiel
 *
 * Created on 1 april 2011, 18:04
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

	Skylium *s_main = new Skylium();

	if (!s_main -> init()) {
		cout << "Błąd przy Skylium::init(). Przerywam.\n\n";
		return 1;
	}
	
	Shader *cienie = new Shader("shaders/shadow.vert", "shaders/shadow.frag");
	if (!cienie -> make())
		exit(1);

	Scene *scenka = s_main -> createScene("Scenka");

	Object *pudelko = scenka -> createBox("Pudelko");
	pudelko -> scale(-3, -3, -3);
	pudelko -> move(-10, 0, 0);
	if (!pudelko -> loadTexture("texture/box.jpg", boxTex, 48))
		cout << "Nie udało się załadować tekstury!";

	Object *malpka = scenka -> createObject("malpka");
	malpka->loadFromObj("objects/monkey.obj", GET_VERTICES | GET_NORMALS | GET_MATERIAL);
	if (!malpka) {
		cout << "Malpka nie działa.\n";
	}
	malpka -> move(-10, 7, 0);
	malpka -> scale(3, 3, 3);
	malpka -> rotate(90, 0, -90);
	malpka -> setColor(126, 54, 25, 1);
	
	Camera* kamerka = scenka -> createCamera(5.0, 6.0, 0.0);
	kamerka -> lookAt(1, 4, -1);
	
	cienie -> bind(malpka);

	int swiatelko = scenka -> createLight(6.0, 6.0, 0.0);
	scenka ->setAmbientLight(swiatelko, 0.5, 0.5, 0.5, 1.0);
	scenka ->toggleLight();
	
	s_main -> execute();
	
	delete s_main;

	return 0;
}

