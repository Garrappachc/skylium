/* 
 * File:   functions.h
 * Author: Michał Garapich, Eryk Dykiel
 *
 * Created on 1 april 2011, 19:54
 */

#ifndef FUNCTIONS_H
#define	FUNCTIONS_H

#include <iostream>
#include <sstream>

template < typename T >
T strToNum(const std::string& s) {
	std::stringstream iss(s);
	T v = (T)0;
	iss >> v;
	return v;
}

template < typename T >
std::string numToStr(const T& v) {
	std::ostringstream oss;
	oss << v;
	return oss.str();
}


#endif	/* FUNCTIONS_H */

