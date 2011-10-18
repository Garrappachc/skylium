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
	
	operator T**() {
		T** ptr = new T*[N];
			for (int i = 0; i < N; i++)
				ptr[i] = &__data[N * i];
		return ptr;
	}
	
	operator const T**() const {
		const T** ptr = new const T*[N];
			for (int i = 0; i < N; i++)
				ptr[i] = &__data[N * i];
		return ptr;
	}
	
	T& operator [](int _pos) {
		assert(_pos < N*N);
		return __data[_pos];
	}
	
	const T& operator [](int _pos) const {
		assert(_pos < N*N);
		return __data[_pos];
	}
	
	void loadIdentity() {
		memset(__data, 0, N * N * sizeof(T));
		for (int i = 0; i < N; ++i)
			__data[i * N + i] = 1;
	}
	
	friend std::ostream& operator <<(std::ostream &_result, sMatrix< T, N > &_orig) {
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
	
	
};

typedef	sMatrix< float, 4 >	sMat16;


#endif // MATRICES_H
