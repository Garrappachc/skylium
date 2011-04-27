/* 
 * File:   LightManager.h
 * Author: Michał Garapich
 *
 * Created on 28 april 2011, 00:22
 */

#ifndef LIGHTMANAGER_H
#define	LIGHTMANAGER_H

#include <GL/gl.h>

unsigned int lights[] = {
	GL_LIGHT0,
	GL_LIGHT1,
	GL_LIGHT2,
	GL_LIGHT3,
	GL_LIGHT4,
	GL_LIGHT5,
	GL_LIGHT6,
	GL_LIGHT7
};

class LightManager {
public:
	LightManager();
	~LightManager();

	void setLights();
private:

};

#endif	/* LIGHTMANAGER_H */

