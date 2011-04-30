/* 
 * File:   Light.h
 * Author: Michał Garapich
 *
 * Created on 27 april 2011, 23:48
 */

#ifndef LIGHT_H
#define	LIGHT_H

#include <iostream>

#include "Vectors.h"

class Scene;

class Light {
	
	friend class Scene;
	
public:
	Light();

	/**
	 * Konstruktor, który przyjmuje położenie źródła światła w postaci sVec3D.
     * @param położenie źródła światła.
     */
	Light(const sVec3D< GLfloat >&);

	/**
	 * Konstruktor, który przyjmuje położenie źródła światła w postaci x, y, z.
     * @param x Współrzędna x źródła światła.
     * @param y Współrzędna y źródła światła.
     * @param z Współrzędna z źródła światła.
     */
	Light(const GLfloat&, const GLfloat&, const GLfloat&);
	
private:
	void setAmbient(const sColor&);
	
	void setDiffuse(const sColor&);
	
	void setSpecular(const sColor&);
	
	void setSrcPos(const sVec3D< GLfloat >&);
	
	void toggle();
	
	
	/* Czy światło jest aktualnie włączone? */
	bool working_;

	/* Światło otoczenia */
	sColor ambientLight_;

	/* Swiatło rozproszone */
	sColor diffuseLight_;

	/* Swiatło odbite */
	sColor specularLight_;

	/* Definujemy położenie żródła światła */
	sVec3D < GLfloat > lightSrc_;

};

#endif	/* LIGHT_H */

