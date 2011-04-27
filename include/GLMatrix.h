/* 
 * File:   GLMatrix.h
 * Author: Michał Garapich
 *
 * Created on 13 kwiecień 2011, 00:07
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

