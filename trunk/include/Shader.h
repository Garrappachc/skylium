/*
    Shader.h
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


#ifndef SHADER_H
#define SHADER_H

#include <string>

#include "Object.h"
#include "Vectors.h"

/**
 * Program shadera wysokopoziomowego.
 */
class Shader {

public:
	/**
	 * Konstruktor, przyjmuje dwa argumenty.
	 * @param vertFileName Nazwa pliku z kodem vertex shadera.
	 * @param fragFileName Nazwa pliku z kodem fragment shadera.
	 */
	Shader(const std::string&, const std::string&);
	
	virtual ~Shader();

	/**
	 * Kompiluje i linkuje program.
	 * @return False, jeżeli coś poszło nie tak.
	 */
	bool make();

	/**
	 * Włącza program, jeżeli jest wyłączony i wyłącza, jeżeli jest włączony.
	 * @return Zawsze true.
	 */
	void toggle();
	
	/**
	 * Przypina shadera do danego obiektu.
	 * @param dest Wskaźnik obiektu, do którego ma być przypięty shader.
	 * @return false, jeżeli coś poszło nie tak.
	 */
	void bind(Object*);
	
	/**
	 * Odpina shadera od obiektu.
	 */
	void unbind(Object*);
	
	/**
	 * Zwraca true, jeżeli shader jest przypięty do danego obiektu.
	 */
	bool isBound(Object*);
	
	/**
	 * Wysyła jakieś zmienne do shadera.
	 * @param name Nazwa zmiennej w shaderze.
	 * @param params Wektor 4 floatów do wysłania.
	 * @return False, jeżeli coś poszło nie tak. Patrz log.
	 */
	bool setUniform4f(const std::string&, const sVec4D< GLfloat >&);

private:
	std::string __vertFile;
	std::string __fragFile;

	GLint __vertexShader;
	GLint __fragmentShader;

	GLint __shaderProgram;

	GLboolean __isRunning;

	/**
	 * Funkcja pomocnicza - sprawdza, czy istnieje podany plik.
	 * @param Nazwa pliku.
	 * @return True, jeżeli istnieje, w przeciwnym przypadku false.
	 */
	bool __fileExists(const std::string&);

};

#endif // SHADER_H
