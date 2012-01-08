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

template < typename T, int N >
class sMatrix {
	
private:
	T	__data[N * N];
	
public:
	sMatrix() {
		memset(__data, 0, N * N * sizeof(T));
	}
	
	// Let the GCC make the copy ctor
	sMatrix(const sMatrix< T, N >&) = default;
	
	sMatrix< T, N> & operator =(const sMatrix< T, N >&) = default;
	
	operator T*() {
		return __data;
	}
	
	operator const T*() const {
		return __data;
	}
	
	T& operator [](int _pos) {
		assert(_pos < N*N);
		return __data[_pos];
	}
	
	const T& operator [](int _pos) const {
		assert(_pos < N*N);
		return __data[_pos];
	}
	
	T& at(int _a, int _b) {
		return __data[_a * N + _b];
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
			return (__data[0] * __data[3]) - (__data[1] * __data[2]);
		if (N == 3)
			return ((__data[0] * __data[4] * __data[8]) + (__data[1] * __data[5] * __data[6]) + (__data[2] * __data[3] * __data[7]))
				- ((__data[2] * __data[4] * __data[6]) + (__data[5] * __data[7] * __data[0]) + (__data[1] * __data[3] * __data[8]));
		
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
		int k = 0;
		for (int i = 0; i < N * N; ++i, ++k) {
			_result << _orig[i] << "\t";
			if (k == N - 1) {
				k = -1;
				_result << "\n";
			}
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
	sMatrix(const sMatrix< T, 4 >&) = default;
	
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
	
	T& at(int _a, int _b) {
		return __data[_a * 4 + _b];
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
		int k = 0;
		for (int i = 0; i < 16; ++i, ++k) {
			_result << _orig[i] << "\t";
			if (k == 3) {
				k = -1;
				_result << "\n";
			}
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
	
	
};
	

typedef	sMatrix< float, 4 >	sMat16;


#endif // MATRICES_H
