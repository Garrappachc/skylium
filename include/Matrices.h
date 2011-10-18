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

template < typename T, int N >
class sMatrix {
	
private:
	T	__data[N * N];
	
public:
	sMatrix() {
		memset(__data, 0, N * sizeof(T));
	}
	
	sMatrix(const T *_orig) {
		memcpy(__data, _orig, N * sizeof(T));
	}
	
	T & operator =(const sMatrix< T, N > &_orig) {
		if (_orig != *this)
			for (int i = 0; i < N; i++)
				__data[i] = _orig[i];
		return *__data;
	}
	
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
		return __data[_pos];
	}
	
	const T& operator [](int _pos) const {
		return __data[_pos];
	}
	
	void loadIdentity() {
		int k = 0;
		for (int i = 0; i < N * N; ++i) {
			if ((i - k) % N == 0)
				__data[i] = 1;
			else
				__data[i] = 0;
			if ((i + 1) % N == 0)
				++k;
		}
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
