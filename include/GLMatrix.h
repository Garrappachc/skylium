/* 
 * GLMatrix.h 
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

#ifndef GLMATRIX_H
#define	GLMATRIX_H

#include <cstdarg>
#include <cmath>

static const short unsigned int DEFAULT_SIZE = 9;

template < typename T >
class GLMatrix {
	T *matrix_;
	int size_;
	
public:
	/**
	 * Konstruktor domyślny - przyjmuje rozmiar 3x3.
     */
	GLMatrix();
	
	/**
	 * Konstruktor, który przyjmuje już gotową tablicę.
     * @param Tablica T.
     */
	GLMatrix(const T*);
	
	/**
	 * Konstruktor, który przyjmuje rozmiar tablicy oraz gotową tablicę.
	 * Szyciej działa.
     * @param _size Rozmiar tablicy.
     * @param Tablica T.
     */
	GLMatrix(const int&, const T*);
	
	/**
	 * Konstruktor, który przyjmuje tablicę dowolną ilość elementów macierzy.
     * @param _size Ilość elementów.
     * @param ... Elementy, po kolei.
     */
	GLMatrix(const int&, const T* ...);
	
	~GLMatrix();
	
	/**
	 * Zwraca tablicę - do wywołań OpenGLa.
     * @return Cała macierz.
     */
	T *get() const;
};

template < typename T >
GLMatrix<T>::GLMatrix() {
	matrix_ = new T[DEFAULT_SIZE]; // standardowa macierz 3x3
	size_ = DEFAULT_SIZE;
}

template < typename T >
GLMatrix<T>::GLMatrix(const T* _matrix) {
	int size = 0;
	
	// sprawdzamy rozmiar tablicy
	while (_matrix[size] != 0)
		size++;
	
	matrix_ = new T[size];

	size_ = size;
	
	for (int i = 0; i < size; i++) {
		matrix_[i] = _matrix[i];
	}
}

template < typename T >
GLMatrix<T>::GLMatrix(const int &_size, const T* _matrix) {
	matrix_ = new T[_size];
	for (int i = 0; i < _size; i++) {
		matrix_[i] = _matrix[i];
	}
}

template < typename T >
GLMatrix<T>::GLMatrix(const int &_size, const T* args ...) {
	if (_size < 0)
		throw;
	matrix_ = new T[_size];
	va_list ap;
	
	va_start(ap, args);

	int i;
	for (i = 0; i < _size; i++) {
		matrix_[i] = va_arg(ap, T);
	}
	va_end(ap);

	size_ = i;
}

template < typename T >
GLMatrix<T>::~GLMatrix() {
	delete [] matrix_;
}

template < typename T >
T *
GLMatrix<T>::get() const {
	return matrix_;
}


#endif	/* GLMATRIX_H */

