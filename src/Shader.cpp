/* 
 * Shader.cpp
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

#ifdef _WIN32
#include <GL/GLee.h>
#else
#include <GLee.h>
#endif

#include <iostream>
#include <string>
#include <fstream>
#include <sys/stat.h>

#include "../include/Shader.h"
#include "../include/defines.h"

using namespace std;

static const unsigned int MAX_LOG_SIZE = 4096;

Shader::Shader(const std::string &_vertFileName, const std::string &_fragFileName) : vertFile_(_vertFileName), fragFile_(_fragFileName), isRunning_(false) {
	vertexShader_ = glCreateShader(GL_VERTEX_SHADER_ARB);
	fragmentShader_ = glCreateShader(GL_FRAGMENT_SHADER_ARB);
}

Shader::~Shader() {
	glDetachShader(shaderProgram_, vertexShader_);
	glDetachShader(shaderProgram_, fragmentShader_);

	glDeleteProgram(shaderProgram_);

	glDeleteShader(vertexShader_);
	glDeleteShader(fragmentShader_);

}

bool
Shader::make() {
	if (!fileExists(vertFile_)) {
#ifdef __DEBUG__
		cout << LOG_ERROR << "Nie znaleziono pliku z kodem źródłowym shadera! Szukano: " << vertFile_ << ".\n";
#endif
		return false;
	}
	if (!fileExists(fragFile_)) {
#ifdef __DEBUG__
		cout << LOG_ERROR << "Nie znaleziono pliku z kodem źródłowym shadera! Szukano: " << fragFile_ << ".\n";
#endif
		return false;
	}
	
#ifdef __DEBUG__
	cout << LOG_INFO << "Odczytywanie źródeł shaderów... ";
#endif
	ifstream vertFile(vertFile_.c_str());
	string vertData = "";
	while (!vertFile.eof()) {
		string temp = "";
		getline(vertFile, temp);
		vertData += temp;
	}
	vertFile.close();

	ifstream fragFile(fragFile_.c_str());
	string fragData = "";
	while (!fragFile.eof()) {
		string temp = "";
		getline(fragFile, temp);
		fragData += temp;
	}
	fragFile.close();
#ifdef __DEBUG__
	cout << "Odczytano.\n" << LOG_INFO << "Kompilowanie programów shaderów... ";
#endif

	const char *vert = vertData.c_str();
	const char *frag = fragData.c_str();

	GLint vlength = vertData.length();
	GLint flength = fragData.length();
	
	glShaderSource(vertexShader_, 1, (const GLchar**)&vert, &vlength);
	glShaderSource(fragmentShader_, 1, (const GLchar**)&frag, &flength);

	int result;

	glCompileShader(vertexShader_);
	glGetShaderiv(vertexShader_, GL_COMPILE_STATUS, &result);
	if (!result) {
		char msg[MAX_LOG_SIZE];
		glGetShaderInfoLog(vertexShader_, MAX_LOG_SIZE, NULL, msg);
#ifdef __DEBUG__
		cout << endl << LOG_ERROR << "Błąd kompilacji vertex shadera! Log kompilacji:\n" << msg << endl;
#endif
		return 0;
	}

	glCompileShaderARB(fragmentShader_);
	glGetShaderiv(fragmentShader_, GL_COMPILE_STATUS, &result);
	if (!result) {
		char msg[MAX_LOG_SIZE];
		glGetShaderInfoLog(fragmentShader_, MAX_LOG_SIZE, NULL, msg);
#ifdef __DEBUG__
		cout << endl << LOG_ERROR << "Błąd kompilacji fragment shadera! Log kompilacji:\n" << msg << endl;
#endif
		return 0;
	}

	shaderProgram_ = glCreateProgram();
	glAttachShader(shaderProgram_, vertexShader_);
	glAttachShader(shaderProgram_, fragmentShader_);

	glLinkProgram(shaderProgram_);

	glGetProgramiv(shaderProgram_, GL_LINK_STATUS, &result);
	if (!result) {
		char msg[MAX_LOG_SIZE];
		glGetProgramInfoLog(shaderProgram_, MAX_LOG_SIZE, NULL, msg);
#ifdef __DEBUG__
		cout << endl << LOG_ERROR << "Błąd przy linkowaniu shadera! Log linkera:\n" << msg << endl;
#endif
		return false;
	}

#ifdef __DEBUG__
	cout << "Zrobione!\n";
#endif
	return true;

}

void
Shader::toggle() {
	if (!isRunning_) {
		glUseProgram(shaderProgram_);
		isRunning_ = true;
	} else {
		glUseProgram(0);
		isRunning_ = false;
	}
}

void
Shader::bind(Object *_dest) {
	_dest -> shader_ = this;
}

bool
Shader::fileExists(const std::string &_fileName) {
	struct stat buf;
	if (stat(_fileName.c_str(), &buf) == 0)
		return 1;
	else
		return 0;
}
