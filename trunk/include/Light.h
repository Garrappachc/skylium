/*
     _    _      _   _     _    
    | |  (_)__ _| |_| |_  | |_  
    | |__| / _` | ' \  _|_| ' \ 
    |____|_\__, |_||_\__(_)_||_|
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

#ifndef __NO_LIGHTING_MANAGEMENT__

#ifndef LIGHT_H
#define LIGHT_H

#include "Vectors.h"

class Scene;

class Light {
	
	friend class Scene; // coby prościej było
	
public:
	Light();

	/**
	 * Konstruktor, który przyjmuje położenie źródła światła w postaci sVec3D.
	 * @param pos Położenie źródła światła.
	 */
	Light(const sPosition&);

	/**
	 * Konstruktor, który przyjmuje położenie źródła światła w postaci x, y, z.
	 * @param x Współrzędna x źródła światła.
	 * @param y Współrzędna y źródła światła.
	 * @param z Współrzędna z źródła światła.
	 */
	Light(const GLfloat&, const GLfloat&, const GLfloat&);
	
	virtual ~Light();
	
	void setAmbient(const sColor&);
	
	void setDiffuse(const sColor&);
	
	void setSpecular(const sColor&);
	
	void setSrcPos(const sPosition&);
	
	void toggle();
	
private:
	
	/* Czy światło jest aktualnie włączone? */
	bool __working;

	/* Światło otoczenia */
	sColor __ambientLight;

	/* Swiatło rozproszone */
	sColor __diffuseLight;

	/* Swiatło odbite */
	sColor __specularLight;

	/* Definujemy położenie żródła światła */
	sVec3D < GLfloat > __lightSrc;

};
#endif // LIGHT_H
#endif // __NO_LIGHTING_MANAGEMENT__