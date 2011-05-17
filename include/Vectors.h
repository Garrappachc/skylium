/* 
 * Vectors.h
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

#ifndef VECTORS_H
#define	VECTORS_H

#include <GL/gl.h>
#include <cstring>
#include <vector>

template < typename T >
class sVec2D {
	
private:
	T __data[2];
	
public:
	
	T &x;
	T &y;
	
	T &u;
	T &v;
	
	sVec2D() :
			x(__data[0]),
			y(__data[1]),
			u(__data[0]),
			v(__data[1]) {
		memset(__data, 0, 2*sizeof(T));
	}
	
	sVec2D(const T &_v1, const T &_v2) :
			x(__data[0]),
			y(__data[1]),
			u(__data[0]),
			v(__data[1]) {
		__data[0] = _v1;
		__data[1] = _v2;
	}
	
	sVec2D(const T *_orig) :
			x(__data[0]),
			y(__data[1]),
			u(__data[0]),
			v(__data[1]) {
		memcpy(__data, _orig, sizeof(T*)*2);
	}
	
	operator T*() {
		return __data;
	}
	
	operator const T*() const {
		return __data;
	}

	const T & operator [](const int &_pos) const {
		return __data[_pos];
	}
	
	 T & operator [](const int &_pos) {
		return __data[_pos];
	}
	
	bool operator ==(const sVec2D &_orig) const {
		return (__data[0] == _orig[0] && __data[1] == _orig[1]);
	}
	
	bool operator !=(const sVec2D &_orig) const {
		return !(__data[0] == _orig[0] && __data[1] == _orig[1]);
	}
	
	void operator =(const sVec2D &_orig) {
		__data[0] = _orig[0];
		__data[1] = _orig[1];
	}
	
	void operator +=(const sVec2D &_orig) {
		__data[0] += _orig[0];
		__data[1] += _orig[1];
	}
	void operator -=(const sVec2D &_orig) {
		__data[0] -= _orig[0];
		__data[1] -= _orig[1];
	}
	
};

template < typename T >
class sVec3D {
	
private:
	T __data[3];
	
public:
	
	T &x;
	T &y;
	T &z;
	
	/* Konstruktory */
	sVec3D() :
			x(__data[0]),
			y(__data[1]),
			z(__data[2]) {
		memset(__data, 0, 3*sizeof(T));
	}
	
	sVec3D(const T &_v1, const T &_v2, const T &_v3) :
			x(__data[0]),
			y(__data[1]),
			z(__data[2]) {
		__data[0] = _v1;
		__data[1] = _v2;
		__data[2] = _v3;
	}
	
	sVec3D(const T *_orig) :
			x(__data[0]),
			y(__data[1]),
			z(__data[2]) {
		memcpy(__data, _orig, sizeof(T*)*3);
	}
	
	/* Operatory */
	operator T*() {
		return __data;
	}
	
	operator const T*() const {
		return __data;
	}

	const T & operator [](const int &_pos) const {
		return __data[_pos];
	}
	
	 T & operator [](const int &_pos) {
		return __data[_pos];
	}
	
	bool operator ==(const sVec3D &_orig) const {
		return (__data[0] == _orig[0] && __data[1] == _orig[1] && __data[2] == _orig[2]);
	}
	
	bool operator !=(const sVec3D &_orig) const {
		return !(__data[0] == _orig[0] && __data[1] == _orig[1] && __data[2] == _orig[2]);
	}
	
	void operator =(const sVec3D &_orig) {
		__data[0] = _orig[0];
		__data[1] = _orig[1];
		__data[2] = _orig[2];
	}
	
	void operator +=(const sVec3D &_orig) {
		__data[0] += _orig[0];
		__data[1] += _orig[1];
		__data[2] += _orig[2];
	}
	void operator -=(const sVec3D &_orig) {
		__data[0] -= _orig[0];
		__data[1] -= _orig[1];
		__data[2] -= _orig[2];
	}
};



template < typename T >
class sVec4D {
	
private:
	T __data[4];
	
public:
	/* Konstruktory */
	sVec4D() {
		memset(__data, 0, 4*sizeof(T));
	}
	
	sVec4D(const T &_v1, const T &_v2, const T &_v3, const T &_v4) {
		__data[0] = _v1;
		__data[1] = _v2;
		__data[2] = _v3;
		__data[3] = _v4;
	}
	
	sVec4D(const T *_orig) {
		memcpy(__data, _orig, sizeof(T*)*4);
	}
	
	/* Operatory */
	operator T*() {
		return __data;
	}
	
	operator const T*() const {
		return __data;
	}
	
	const T & operator [](const int &_pos) const {
		return __data[_pos];
	}
	
	T & operator [](const int &_pos) {
		return __data[_pos];
	}
	
	bool operator ==(const sVec4D &_orig) const {
		return (__data[0] == _orig[0] && __data[1] == _orig[1] && __data[2] == _orig[2] && __data[3] == _orig[3]);
	}
	bool operator !=(const sVec4D &_orig) const {
		return !(__data[0] == _orig[0] && __data[1] == _orig[1] && __data[2] == _orig[2] && __data[3] == _orig[3]);
	}
	
	void operator =(const sVec4D &_orig) {
		__data[0] = _orig[0];
		__data[1] = _orig[1];
		__data[2] = _orig[2];
		__data[3] = _orig[3];
	}
	
	void operator +=(const sVec4D& _orig) {
		__data[0] += _orig[0];
		__data[1] += _orig[1];
		__data[2] += _orig[2];
		__data[3] += _orig[3];
	}
	void operator -=(const sVec4D& _orig) {
		__data[0] -= _orig[0];
		__data[1] -= _orig[1];
		__data[2] -= _orig[2];
		__data[3] -= _orig[3];
	}
};

typedef	sVec2D< GLdouble >		sSingleCoord;	// used for Hud

typedef	sVec3D< GLdouble >		sVector;

typedef	sVec4D< GLfloat >		sColor;

typedef	sVec3D< GLfloat >		sVertex;

typedef	sVec3D< int >			sCoords;

/* Some other typedefs */
typedef	std::vector< GLfloat >	sArray;



#endif	/* VECTORS_H */

