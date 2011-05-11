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
class sVec3D {
private:
	T data_[3];
	
public:
	/* Konstruktory */
	sVec3D();
	sVec3D(const T&, const T&, const T&);
	sVec3D(const T*);
	
	/* Operatory */
	operator T*();
	operator const T*() const;

	const T operator[](const int&) const;
	T & operator [](const int&);
	
	bool operator == (const sVec3D&) const;
	bool operator != (const sVec3D&) const;
	
	void operator = (const sVec3D&);
	
	void operator += (const sVec3D&);
	void operator -= (const sVec3D&);
};

template < typename T >
class sVec4D {
private:
	T data_[4];
	
public:
	/* Konstruktory */
	sVec4D();
	sVec4D(const T&, const T&, const T&, const T&);
	sVec4D(const T*);
	
	/* Operatory */
	operator T*();
	operator const T*() const;
	
	const T operator [](const int&) const;
	T & operator [](const int&);
	
	bool operator == (const sVec4D&) const;
	bool operator != (const sVec4D&) const;
	
	void operator = (const sVec4D&);
	
	void operator += (const sVec4D&);
	void operator -= (const sVec4D&);
};




/*****   sVec3D   *****/

template < typename T >
sVec3D<T>::sVec3D() {
	memset(data_, 0, 3*sizeof(T));
}

template < typename T >
sVec3D<T>::sVec3D(const T &_v1, const T &_v2, const T &_v3) {
	data_[0] = _v1;
	data_[1] = _v2;
	data_[2] = _v3;
}

template < typename T >
sVec3D<T>::sVec3D(const T *_orig) {
	memcpy(data_, _orig, sizeof(T*)*3);
}

template < typename T >
sVec3D<T>::operator T*() {
	return data_;
}

template < typename T >
sVec3D<T>::operator const T*() const {
	return data_;
}

template < typename T >
const T
sVec3D<T>::operator [](const int &_pos) const {
	return data_[_pos];
}

template < typename T >
T &
sVec3D<T>::operator [](const int &_pos) {
	return data_[_pos];
}

template < typename T >
void
sVec3D<T>::operator =(const sVec3D &_orig) {
	data_[0] = _orig[0];
	data_[1] = _orig[1];
	data_[2] = _orig[2];
}

template < typename T >
bool
sVec3D<T>::operator ==(const sVec3D &_orig) const {
	return (data_[0] == _orig[0] && data_[1] == _orig[1] && data_[2] == _orig[2]);
}

template < typename T >
bool
sVec3D<T>::operator !=(const sVec3D &_orig) const {
	return !(data_[0] == _orig[0] && data_[1] == _orig[1] && data_[2] == _orig[2]);
}

template < typename T >
void
sVec3D<T>::operator +=(const sVec3D &_orig) {
	data_[0] += _orig[0];
	data_[1] += _orig[1];
	data_[2] += _orig[2];
}

template < typename T >
void
sVec3D<T>::operator -=(const sVec3D &_orig) {
	data_[0] -= _orig[0];
	data_[1] -= _orig[1];
	data_[2] -= _orig[2];
}


/*****   sVec4D   *****/
template < typename T >
sVec4D<T>::sVec4D() {
	memset(data_, 0, 4*sizeof(T));
}

template < typename T >
sVec4D<T>::sVec4D(const T &_v1, const T &_v2, const T &_v3, const T &_v4) {
	data_[0] = _v1;
	data_[1] = _v2;
	data_[2] = _v3;
	data_[3] = _v4;
}

template < typename T >
sVec4D<T>::sVec4D(const T *_orig) {
	memcpy(data_, _orig, sizeof(T*)*4);
}

template < typename T >
sVec4D<T>::operator T*() {
	return data_;
}

template < typename T >
sVec4D<T>::operator const T*() const {
	return data_;
}

template < typename T >
const T
sVec4D<T>::operator [](const int &_pos) const {
	return data_[_pos];
}

template < typename T >
T &
sVec4D<T>::operator [](const int &_pos) {
	return data_[_pos];
}

template < typename T >
void
sVec4D<T>::operator =(const sVec4D &_orig) {
	data_[0] = _orig[0];
	data_[1] = _orig[1];
	data_[2] = _orig[2];
	data_[3] = _orig[3];
}

template < typename T >
bool
sVec4D<T>::operator ==(const sVec4D &_orig) const {
	return (data_[0] == _orig[0] && data_[1] == _orig[1] && data_[2] == _orig[2] && data_[3] == _orig[3]);
}

template < typename T >
bool
sVec4D<T>::operator !=(const sVec4D &_orig) const {
	return !(data_[0] == _orig[0] && data_[1] == _orig[1] && data_[2] == _orig[2] && data_[3] == _orig[3]);
}

template < typename T >
void
sVec4D<T>::operator +=(const sVec4D& _orig) {
	data_[0] += _orig[0];
	data_[1] += _orig[1];
	data_[2] += _orig[2];
	data_[3] += _orig[3];
}

template < typename T >
void
sVec4D<T>::operator -=(const sVec4D& _orig) {
	data_[0] -= _orig[0];
	data_[1] -= _orig[1];
	data_[2] -= _orig[2];
	data_[3] -= _orig[3];
}


typedef sVec3D< GLdouble > sVector;

typedef sVec4D< GLfloat > sColor;

typedef sVec3D< GLfloat > sVertex;

typedef sVec3D< int > sCoords;

/* Some other typedefs */
typedef std::vector< GLfloat > sArray;



#endif	/* VECTORS_H */

