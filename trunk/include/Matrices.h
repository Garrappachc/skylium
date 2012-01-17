/*
     __  __      _       _             _    
    |  \/  |__ _| |_ _ _(_)__ ___ ___ | |_  
    | |\/| / _` |  _| '_| / _/ -_|_-<_| ' \ 
    |_|  |_\__,_|\__|_| |_\__\___/__(_)_||_|
                                    
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


#ifndef MATRICES_H
#define MATRICES_H

#include <iostream>
#include <cstring>

#include <assert.h>

#include <skylium/Vectors.h>

/*
 * This matrix is a column-order matrix, as OpenGL prefers.
 * It is continuous in the memory.
 */
template < typename T, int N >
class sMatrix {
	
private:
	T	__data[N * N];
	
public:
	// default ctor
	sMatrix() {
		memset(__data, 0, N * N * sizeof(T));
	}
	
	// Let the GCC make the copy ctor
	sMatrix(const sMatrix< T, N >&) = default;
	
	// and the operator =
	sMatrix< T, N> & operator =(const sMatrix< T, N >&) = default;
	
	// pointer
	operator T*() {
		return __data;
	}
	
	operator const T*() const {
		return __data;
	}
	
	T& operator [](int _pos) {
		assert(_pos < N * N);
		return __data[_pos];
	}
	
	const T& operator [](int _pos) const {
		assert(_pos < N * N);
		return __data[_pos];
	}
	
	// not like in minor in maths in general - we count from 0.
	// i - row, j - column
	T& at(int _i, int _j) {
		return __data[_j * N + _i];
	}
	
	const T& at(int _i, int _j) const {
		return __data[_j * N + _i];
	}
	
	void loadIdentity() {
		memset(__data, 0, N * N * sizeof(T));
		for (int i = 0; i < N; ++i)
			__data[i * N + i] = 1;
	}
	
	T determinant() {
		if (N == 1)
			return __data[0];
		if (N == 2)
			return (__data[0] * __data[3]) - (__data[2] * __data[1]);
		if (N == 3)
			return ((__data[0] * __data[4] * __data[8]) + (__data[3] * __data[7] * __data[2]) + (__data[6] * __data[1] * __data[5]))
				- ((__data[6] * __data[4] * __data[2]) + (__data[3] * __data[1] * __data[8]) + (__data[0] * __data[7] * __data[5]));
		
	}
	
	/* Counting form 0 - not like in minors in general */
	sMatrix< T, N - 1 > getMinor(int _i, int _j) {
		sMatrix< T, N - 1 > result;
		int idx = 0;
		
		for (int i = 0; i < N; ++i) {
			if (i == _i)
				continue;
			for (int j = 0; j < N; ++j) {
				if (j == _j)
					continue;
				result[idx++] = at(i, j);
			}
		}
		
		return result;
	}
	
	friend std::ostream& operator <<(std::ostream& _result, const sMatrix< T, N >& _orig) {
		for (int i = 0; i < N; ++i) {
			for (int j = 0; j < N; ++j)
				_result << _orig.at(i, j) << "\t";
			_result << "\n";
		}
		return _result;
	}
	
	friend sMatrix< T, N > operator *(const sMatrix< T, N >& _a, const sMatrix< T, N >& _b) {
		sMatrix< T, N > result;
		for (int i = 0; i < N; ++i)
			for (int j = 0; j < N; ++j) 
				for (int idx = 0; idx < N; ++idx)
					result.at(i, j) += _a.at(i, idx) * _b.at(idx, j);
		
		return result;	
	}
	
	sMatrix< T, N >& operator *=(const sMatrix< T, N >& _b) {
		*this = *this * _b;
		return *this;
	}
	
	/**
	 * Sets the whole column in the matrix.
	 * @param c Column to be replaced, counting from 0.
	 * @param vector Vector with data.
	 * @param offset Which row to start from. Default 0.
	 */
	template < typename D >
	void setColumn(int _c, const sVec< D >& _vector, unsigned _offset = 0) {
		assert(_c < N);
		int v = 0;
		for (unsigned i = _offset; i < _vector.size(); ++i, ++v)
			at(i, _c) = _vector[v];
	}
	
	/**
	 * Sets the while row in the matrix.
	 * @param r Row to be replaced, counting from 0.
	 * @param vector Vector with data.
	 * @param offset Which column to start from. Default 0.
	 */
	template < typename D >
	void setRow(int _r, const sVec< D >& _vector, unsigned _offset = 0) {
		assert(_r < N);
		int v = 0;
		for (unsigned i = _offset; i < _vector.size(); ++i, ++v)
			at(_r, i) = _vector[v];
	}
	
	
};

template< typename T >
class sMatrix< T, 4 > {

private:
	T	__data[16];
	
public:
	sMatrix() {
		memset(__data, 0, 4 * 4 * sizeof(T));
	}
	
	// Let the GCC make the copy ctor
	sMatrix(const sMatrix< T, 4 >& _orig) = default;
	
	sMatrix< T, 4 > & operator =(const sMatrix< T, 4 >&) = default;
	
	operator T*() {
		return __data;
	}
	
	operator const T*() const {
		return __data;
	}
	
	T& operator [](int _pos) {
		assert(_pos < 16);
		return __data[_pos];
	}
	
	const T& operator [](int _pos) const {
		assert(_pos < 16);
		return __data[_pos];
	}
	
	T& at(int _i, int _j) {
		return __data[_j * 4 + _i];
	}
	
	const T& at(int _i, int _j) const {
		return __data[_j * 4 + _i];
	}
	
	void loadIdentity() {
		memset(__data, 0, 16 * sizeof(T));
		for (int i = 0; i < 4; ++i)
			__data[i * 4 + i] = 1;
	}
	
	T determinant() {
		int det = 0, i = 0;
		for (int j = 0; j < 4; ++j) {
			if ((i + j) % 2 == 0) 
				det += at(i, j) * getMinor(i, j).determinant();
			else
				det -= at(i, j) * getMinor(i, j).determinant();
		}
		return det;
		
	}
	
	/* Counting form 0 - not like in minors in general */
	sMatrix< T, 3 > getMinor(int _i, int _j) {
		sMatrix< T, 3 > result;
		int idx = 0;
		
		for (int i = 0; i < 4; ++i) {
			if (i == _i)
				continue;
			for (int j = 0; j < 4; ++j) {
				if (j == _j)
					continue;
				result[idx++] = at(i, j);
			}
		}
		
		return result;
	}
	
	friend std::ostream& operator <<(std::ostream& _result, const sMatrix< T, 4 >& _orig) {
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j)
				_result << _orig.at(i, j) << "\t";
			_result << "\n";
		}
		return _result;
	}
	
	friend sMatrix< T, 4 > operator *(const sMatrix< T, 4 >& _a, const sMatrix< T, 4 >& _b) {
		sMatrix< T, 4 > result;
		for (int i = 0; i < 4; ++i)
			for (int j = 0; j < 4; ++j) 
				for (int idx = 0; idx < 4; ++idx)
					result.at(i, j) += _a.at(i, idx) * _b.at(idx, j);
		
		
		return result;	
	}
	
	sMatrix< T, 4 >& operator *=(const sMatrix< T, 4 >& _b) {
		*this = *this * _b;
		return *this;
	}
	
	/**
	 * Sets the whole column in the matrix.
	 * @param c Column to be replaced, counting from 0.
	 * @param vector Vector with data.
	 * @param offset Which row to start from. Default 0.
	 */
	template < typename D >
	void setColumn(int _c, const sVec< D >& _vector, unsigned _offset = 0) {
		assert(_c < 4);
		int v = 0;
		for (unsigned i = _offset; i < _vector.size(); ++i, ++v)
			at(i, _c) = _vector[v];
	}
	
	/**
	 * Sets the while row in the matrix.
	 * @param r Row to be replaced, counting from 0.
	 * @param vector Vector with data.
	 * @param offset Which column to start from. Default 0.
	 */
	template < typename D >
	void setRow(int _r, const sVec< D >& _vector, unsigned _offset = 0) {
		assert(_r < 4);
		int v = 0;
		for (unsigned i = _offset; i < _vector.size(); ++i, ++v)
			at(_r, i) = _vector[v];
	}
};
	

typedef	sMatrix< float, 4 >	sMat16;


#endif // MATRICES_H
