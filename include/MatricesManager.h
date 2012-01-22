/*
	 __  __      _       _           __  __                              _    
	|  \/  |__ _| |_ _ _(_)__ ___ __|  \/  |__ _ _ _  __ _ __ _ ___ _ _ | |_  
	| |\/| / _` |  _| '_| / _/ -_|_-< |\/| / _` | ' \/ _` / _` / -_) '_|| ' \ 
	|_|  |_\__,_|\__|_| |_\__\___/__/_|  |_\__,_|_||_\__,_\__, \___|_|(_)_||_|
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


#ifndef MATRICESMANAGER_H
#define MATRICESMANAGER_H

#include <stack>

#include "Singleton.h"
#include "Matrices.h"
#include "Vectors.h"

enum Axis { X, Y, Z };

class MatricesManager : public Singleton< MatricesManager > {
public:
	// default ctor
     MatricesManager();
	
	/* Access to matrices is sometimes useful */
	sMat16 & getModelViewMatrix() { return __modelViewMatrix; }
	const sMat16 & getModelViewMatrix() const { return __modelViewMatrix; }
	
	sMat16 & getProjectionMatrix() { return __projectionMatrix; }
	const sMat16 & getProjectionMatrix() const { return __projectionMatrix; }
	
	sMat9 & getNormalMatrix() { return __normalMatrix; }
	const sMat9 & getNormalMatrix() const { return __normalMatrix; }
	
	/**
	 * Produces a viewing transform.
	 * @param eye Eye point.
	 * @param center The reference point position.
	 * @param up The UP Vector.
	 */
	void sLookAt(const sVector&, const sVector&, const sVector&);
	
	/**
	 * Sets a perspective projection matrix.
	 * @param fovy The field of view angle, in degrees, in the 'y' direction.
	 * @param aspect Species the aspect ratio x:y.
	 * @param zNear Specifies the distance from the viewer to the near clipping plane (>0).
	 * @param zFar Specifies the distance from the viewer to the far clipping plane (>0).
	 */
	void sPerspective(GLdouble, GLdouble, GLdouble, GLdouble);
	
	/**
	 * Generates a pararell projection.
	 * @param left Specify the coordinates for the left vertical clipping planes.
	 * @param right Specify the coordinates for the right vertical clipping planes.
	 * @param bottom Specify the coordinates for the bottom horizontal clipping planes.
	 * @param top Specify the coordinates for the top horizontal clipping planes.
	 * @param nearVal Specify the distances to the nearer depth clipping planes.
	 * @param farVal Specify the distances to the farther depth clipping planes.
	 */
	void sOrtho(GLdouble, GLdouble, GLdouble, GLdouble, GLdouble, GLdouble);
	
	/**
	 * Generates the Normal Matrix.
	 * Normal matrix is the transpose of the inverse of the modelView Matrix.
	 */
	void produceNormalMatrix();
	
	/**
	 * Translate by (x, y, z). The current ModelView Matrix is being multiplied
	 * by the translation matrix.
	 * http://www.opengl.org/sdk/docs/man/xhtml/glTranslate.xml
	 * @param trans Translation vector.
	 */
	void translate(const sVector&);
	
	/**
	 * Scale along the x, y and z axes. The current ModelView Matrix is being
	 * multiplied by this scale matrix.
	 * http://www.opengl.org/sdk/docs/man/xhtml/glScale.xml
	 * @param scale Scale vector.
	 */
	void scale(const sVector&);
	
	/**
	 * Procudes a rotation of {x,y,z}-angle along {x,y,z}-axis. The current
	 * ModelView Matrix is being multiplied.
	 * http://www.opengl.org/sdk/docs/man/xhtml/glRotate.xml
	 * @param rotation Rotation vector.
	 */
	void rotate(GLdouble, Axis);
	
	/**
	 * Pushes the current matrix on the stack.
	 */
	void storeModelViewMatrix();
	void storeProjectionMatrix();
	
	/**
	 * Pops the matrix from the stack.
	 */
	void restoreModelViewMatrix();
	void restoreProjectionMatrix();
	
	
	
private:
	
	sMat16 __modelViewMatrix;
	
	sMat16 __projectionMatrix;
	
	sMat9 __normalMatrix;
	
	std::stack< sMat16 > __MVStack;
	
	std::stack< sMat16 > __PStack;
	
	std::stack< sMat9 > __NStack;
	
	
	
};

#endif // MATRICESMANAGER_H
