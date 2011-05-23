/* 
 * File:   sizes.cpp
 * Authors: Michał Garapich
 *
 * Created on 20 may 2011, 00:15
 */

#include <iostream>

#include "include/Vectors.h"
#include "include/Vertex.h"

using namespace std;

int main() {
	cout << "sColor size: " << sizeof(sColor) << " B" << endl << endl;
	cout << "Vertex size: " << sizeof(Vertex) << " B" << endl
		<< "	Position: " << sizeof(Position) << " B" << endl
		<< "	TexCoords: " << sizeof(TexCoords) << " B" << endl
		<< "	Normal: " << sizeof(Normal) << " B" << endl << endl;
	cout << "GLushort size: " << sizeof(GLushort) << " B" << endl;
}
