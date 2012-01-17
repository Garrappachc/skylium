/*
      ___                           _    
     / __|__ _ _ __  ___ _ _ __ _  | |_  
    | (__/ _` | '  \/ -_) '_/ _` |_| ' \ 
     \___\__,_|_|_|_\___|_| \__,_(_)_||_|

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

#ifndef CAMERA_H
#define CAMERA_H

#include <stack>

#include <GL/gl.h>
#include <GL/glu.h>

#include "Vectors.h"
#include "Matrices.h"

class MatricesManager;

/* Cameras types */
typedef enum {
	FPP		= 1,	// First Person Perspective, moves the "lookAt" point
	TPP,			// up
	SPHERICAL		// First Person Perspective, moves the "eye" point
} cType;


/**
 * WARNING
 * In the FPP camera __center is a vector, not the "lookAt" point.
 * It is normalized vector, which specifies the "lookAt" for the (0,0,0) observer.
 * That is why gluLookAt(..) is being called with center as __center + __eye.
 */

class Camera {

public:
	
	/**
	 * Default ctor, camera in (0,0,0) position.
	 */
	Camera(const cType& = FPP);
	
	/**
	 * Ctor that gets camera's location coords.
	 * @param x X coord.
	 * @param y Y coord.
	 * @param z Z coord.
	 * @param type Camera type. FPP, TPP, SPHERICAL
	 */
	Camera(GLdouble, GLdouble, GLdouble, const cType& = FPP);
	
	/**
	 * Destructor just sends some output.
	 */
	virtual ~Camera();
	
	/**
	 * GL_PROJECTION;
	 * gluPerspective.
	 * Called only when parameters change.
	 * http://www.opengl.org/sdk/docs/man/xhtml/gluPerspective.xml
	 * http://www.felixgers.de/teaching/jogl/gluPerspective.gif
	 */
	void setProjection();

	/**
	 * GL_MODELVIEW;
	 * gluLookAt.
	 * Called only when parameters change.
	 * http://pyopengl.sourceforge.net/documentation/manual/gluLookAt.3G.html
	 * http://www.toldo.info/roberto/LaboratorioGrafica/Slides/images/glulookat.gif
	 */
	void setView();

	/**
	 * Moves camera, 2-dimensional movement.
	 * @param movX X axis.
	 * @param movY Y axis.
	 * @param movZ Z axis.
	 */
	void moveCamera(GLdouble, GLdouble, GLdouble);

	/**
	 * Rotates the camera - mouse support.
	 * @param x X rotation.
	 * @param y Y rotation.
	 * @param z Z rotation.
	 */
	void rotateCamera(GLdouble, GLdouble, GLdouble);

	/**
	 * Sets the "lookAt" point.
	 * @param x X coord.
	 * @param y Y coord.
	 * @param z Z coord.
	 */
	void lookAt(GLdouble, GLdouble, GLdouble);
	
	/**
	 * Sets camera range in SPHERICAL mode.
	 * @param range Range.
	 */
	void setRange(GLdouble _range) { __range = _range; }
	
	/**
	 * @return The __eye's coords.
	 */
	sVector getEye();
	
	/**
	 * @return The __center's coords.
	 */
	sVector getCenter();
	
	/**
	 * @return Range.
	 */
	GLdouble getRange() { return __range; }


private:
	
	/* Camera type */
	cType __type;
	
	/*** setProjection ***/
	GLdouble __fovy;
	GLdouble __zNear;
	GLdouble __zFar;

	/*** setView ***/
	
	/* Camera's position */
	sVector __eye;

	/* LookAt position/vector */
	sVector __center;

	/* Up vector, (0, 1, 0) by  default */
	sVector __up;
	
	/* Angle of the camera */
	sVector __angle;
	
	/* Range of the SPHERICAL camera type */
	GLdouble __range;
	
	/* Window dimensions */
	int __windowHeight;
	int __windowWidth;
	
	MatricesManager& __matrices;
};

#endif // CAMERA_H