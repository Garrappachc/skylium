/* 
 * shapes.h
 * Copyright (C) 2011 Michał Garapich
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see http://www.gnu.org/licenses/.
 */

#ifndef SHAPES_H
#define	SHAPES_H

#include <GL/gl.h>

GLfloat boxPointers[] = {
	// przednia ściana
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,

	// tylna ściana
	-1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,

	// górna ściana
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f, -1.0f,

	// dolna ściana
	-1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	// prawa ściana
	 1.0f, -1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f,

	// lewa ściana
	-1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f, -1.0f
};

GLfloat boxNormals[] = {
	 0.0f,  0.0f,  1.0f,
	 0.0f,  0.0f,  1.0f,
	 0.0f,  0.0f,  1.0f,
	 0.0f,  0.0f,  1.0f,
	 
	 0.0f,  0.0f, -1.0f,
	 0.0f,  0.0f, -1.0f,
	 0.0f,  0.0f, -1.0f,
	 0.0f,  0.0f, -1.0f,
	 
	 0.0f,  1.0f,  0.0f,
	 0.0f,  1.0f,  0.0f,
	 0.0f,  1.0f,  0.0f,
	 0.0f,  1.0f,  0.0f,
	 
	 0.0f, -1.0f,  0.0f,
	 0.0f, -1.0f,  0.0f,
	 0.0f, -1.0f,  0.0f,
	 0.0f, -1.0f,  0.0f,
	 
	 1.0f,  0.0f,  0.0f,
	 1.0f,  0.0f,  0.0f,
	 1.0f,  0.0f,  0.0f,
	 1.0f,  0.0f,  0.0f,
	 
	-1.0f,  0.0f,  0.0f,
	-1.0f,  0.0f,  0.0f,
	-1.0f,  0.0f,  0.0f,
	-1.0f,  0.0f,  0.0f
};

#endif	/* SHAPES_H */

