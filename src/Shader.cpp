/*
    Shader.cpp
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

#include <GLee.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sys/stat.h>

#include "../include/Shader.h"
#include "../include/Vectors.h"
#include "../include/defines.h"

using namespace std;

static const unsigned int MAX_LOG_SIZE = 4096;

Shader::Shader(const std::string &_vertFileName, const std::string &_fragFileName) : 
		__vertFile(_vertFileName),
		__fragFile(_fragFileName),
		__isRunning(false) {
	__vertexShader = glCreateShader(GL_VERTEX_SHADER_ARB);
	__fragmentShader = glCreateShader(GL_FRAGMENT_SHADER_ARB);
}

Shader::~Shader() {
	glDetachShader(__shaderProgram, __vertexShader);
	glDetachShader(__shaderProgram, __fragmentShader);

	glDeleteProgram(__shaderProgram);

	glDeleteShader(__vertexShader);
	glDeleteShader(__fragmentShader);
#ifdef __DEBUG__
	cout << LOG_INFO << "Destruktor: Shader(\"" << __vertFile << "\", \"" << __fragFile << "\")";
#endif

}

bool
Shader::make() {
	if (!__fileExists(__vertFile)) {
#ifdef __DEBUG__
		cout << LOG_ERROR << "Nie znaleziono pliku z kodem źródłowym shadera! Szukano: " << __vertFile << ".";
#endif
		return false;
	}
	if (!__fileExists(__fragFile)) {
#ifdef __DEBUG__
		cout << LOG_ERROR << "Nie znaleziono pliku z kodem źródłowym shadera! Szukano: " << __fragFile << ".";
#endif
		return false;
	}
	
#ifdef __DEBUG__
	cout << LOG_INFO << "Odczytywanie źródeł shaderów... ";
#endif
	ifstream vertFile(__vertFile.c_str());
	string vertData = "";
	while (!vertFile.eof()) {
		string temp = "";
		getline(vertFile, temp);
		vertData += temp;
	}
	vertFile.close();

	ifstream fragFile(__fragFile.c_str());
	string fragData = "";
	while (!fragFile.eof()) {
		string temp = "";
		getline(fragFile, temp);
		fragData += temp;
	}
	fragFile.close();
#ifdef __DEBUG__
	cout << "Odczytano." << LOG_INFO << "Kompilowanie programów shaderów... ";
#endif

	const char *vert = vertData.c_str();
	const char *frag = fragData.c_str();

	GLint vlength = vertData.length();
	GLint flength = fragData.length();
	
	glShaderSource(__vertexShader, 1, (const GLchar**)&vert, &vlength);
	glShaderSource(__fragmentShader, 1, (const GLchar**)&frag, &flength);

	int result;

	glCompileShader(__vertexShader);
	glGetShaderiv(__vertexShader, GL_COMPILE_STATUS, &result);
	if (!result) {
		char msg[MAX_LOG_SIZE];
		glGetShaderInfoLog(__vertexShader, MAX_LOG_SIZE, NULL, msg);
#ifdef __DEBUG__
		cout << LOG_ERROR << "Błąd kompilacji vertex shadera! Log kompilacji:\n" << msg << endl;
#endif
		return 0;
	}

	glCompileShaderARB(__fragmentShader);
	glGetShaderiv(__fragmentShader, GL_COMPILE_STATUS, &result);
	if (!result) {
		char msg[MAX_LOG_SIZE];
		glGetShaderInfoLog(__fragmentShader, MAX_LOG_SIZE, NULL, msg);
#ifdef __DEBUG__
		cout << LOG_ERROR << "Błąd kompilacji fragment shadera! Log kompilacji:\n" << msg << endl;
#endif
		return 0;
	}

	__shaderProgram = glCreateProgram();
	glAttachShader(__shaderProgram, __vertexShader);
	glAttachShader(__shaderProgram, __fragmentShader);

	glLinkProgram(__shaderProgram);

	glGetProgramiv(__shaderProgram, GL_LINK_STATUS, &result);
	if (!result) {
		char msg[MAX_LOG_SIZE];
		glGetProgramInfoLog(__shaderProgram, MAX_LOG_SIZE, NULL, msg);
#ifdef __DEBUG__
		cout << LOG_ERROR << "Błąd przy linkowaniu shadera! Log linkera:\n" << msg << endl;
#endif
		return false;
	}

#ifdef __DEBUG__
	cout << "Zrobione!";
#endif
	return true;

}

void
Shader::toggle() {
	if (!__isRunning) {
		glUseProgram(__shaderProgram);
		__isRunning = true;
	} else {
		glUseProgram(0);
		__isRunning = false;
	}
}

void
Shader::bind(Object *_dest) {
	_dest -> __shader = this;
}

void
Shader::unbind(Object *_dest) {
	_dest -> __shader = NULL;
}

bool
Shader::isBound(Object *_dest) {
	if (_dest -> __shader == NULL)
		return false;
	else
		return true;
}

bool
Shader::setUniform4f(const string &_name, const sVec4D< GLfloat > &_params) {
	GLint location = glGetUniformLocation(__shaderProgram, _name.c_str());
	glUniform4f(location, _params[0], _params[1], _params[2], _params[3]);
	GLenum err = glGetError(); // pobieramy błędy
	while (err != GL_NO_ERROR) {
		cout << "Błąd! Numer błędu: " << err << endl;
		err = glGetError();
	}
	return true;
}

bool
Shader::__fileExists(const std::string &_fileName) {
	struct stat buf;
	if (stat(_fileName.c_str(), &buf) == 0)
		return 1;
	else
		return 0;
}