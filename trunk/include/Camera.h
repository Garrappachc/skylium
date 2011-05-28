/*
    Camera.h
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

#ifndef __NO_CAMERA_MANAGEMENT__

#ifndef CAMERA_H
#define CAMERA_H

#include <GL/gl.h>
#include <GL/glu.h>

#include "Vectors.h"

/* Typy kamer */
typedef enum {
	FPP		= 1,	// z widoku pierwszej osoby, myszką ruszamy punkt "lookAt"
	TPP,			// z góry
	SPHERICAL		// z widoku pierwszej osoby, myszką ruszamy punkt "eye"
} cType;

/**
 * UWAGA.
 * Co do obrotu i ruchu kamery.
 * W trybie FPP __center jest _wektorem_, nie punktem na który ma patrzeć kamera.
 * Jest to wektor znormalizowany, który określa punkt lookAt w stosunku do punktu (0, 0, 0).
 * Dlatego przy wywoływaniu gluLookAt(...), jako center określamy __center + __eye.
 */

class Camera {

public:
	Camera(const cType& = FPP);
	Camera(const GLdouble&, const GLdouble&, const GLdouble&, const cType& = FPP);
	
	virtual ~Camera();
	
	/**
	 * GL_PROJECTION;
	 * gluPerspective.
	 * Tylko w razie zmiany parametrów.
	 * http://www.opengl.org/sdk/docs/man/xhtml/gluPerspective.xml
	 * http://www.felixgers.de/teaching/jogl/gluPerspective.gif
	 */
	void setProjection();

	/**
	 * GL_MODELVIEW;
	 * gluLookAt.
	 * Tylko w razie zmiany parametrów.
	 * http://pyopengl.sourceforge.net/documentation/manual/gluLookAt.3G.html
	 * http://www.toldo.info/roberto/LaboratorioGrafica/Slides/images/glulookat.gif
	 */
	void setView();

	/**
	 * Przesuwa kamerę o podane wartości, w trybie 2D.
	 * @param movX oś X;
	 * @param movY oś Y;
	 * @param movZ oś Z.
	 */
	void moveCamera(const GLdouble&, const GLdouble&, const GLdouble&);

	/**
	 * Obraca kamerę w jednym miejscu - obsługa myszy.
	 * @param x;
	 * @param y;
	 * @param z.
	 */
	void rotateCamera(const GLdouble&, const GLdouble&, const GLdouble&);

	/**
	 * Ustawia punkt, na który patrzy się kamera.
	 * Jako argument przyjmuje albo trzy GLdouble, które definiują punkt (x, y, z),
	 * albo tablicę GLdouble (p[0], p[1], p[2]).
	 * @param x Pozycja X.
	 * @param y Pozycja Y.
	 * @param z Pozycja Z.
	 */
	void lookAt(const GLdouble&, const GLdouble&, const GLdouble&);
	
	void printCenter();


private:
	
	/* Typ kamery */
	cType __type;
	
	/*** setProjection ***/
	GLdouble __fovy;
	GLdouble __zNear;
	GLdouble __zFar;

	/*** setView ***/
	
	/* Punkt, w którym znajduje się kamera */
	sVector __eye;

	/* Punkt, na który patrzy się kamera */
	sVector __center;

	/* Wektor zmiany kierunku patrzenia kamery */
	sVector __up;
	
	/* Kąt, pod którym kamera znajduje się w stosunku do osi -z i osi y */
	sVector __angle;
	
	/* Przyda się przy setOrtho() */
	int __windowHeight;
	int __windowWidth;
};

#endif // CAMERA_H
#endif //__NO_CAMERA_MANAGEMENT__