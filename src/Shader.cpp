/* 
 * File:   Shader.cpp
 * Author: Michał Garapich
 * 
 * Created on 20 april 2011, 20:34
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

using namespace std;

static const unsigned int MAX_LOG_SIZE = 4096;

Shader::Shader(const std::string &_vertFileName, const std::string &_fragFileName) : vertFile_(_vertFileName), fragFile_(_fragFileName), isRunning_(false) {
	vertexShader_ = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
	fragmentShader_ = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);
}

Shader::~Shader() {
	glDetachObjectARB(shaderProgram_, vertexShader_);
	glDetachObjectARB(shaderProgram_, fragmentShader_);

	glDeleteProgram(shaderProgram_);

	glDeleteObjectARB(vertexShader_);
	glDeleteObjectARB(fragmentShader_);

}

bool
Shader::make() {
	if (!fileExists(vertFile_)) {
		cout << "Nie znaleziono pliku z kodem źródłowym shadera! Szukano: " << vertFile_ << ".\n";
		return false;
	}
	if (!fileExists(fragFile_)) {
		cout << "Nie znaleziono pliku z kodem źródłowym shadera! Szukano: " << fragFile_ << ".\n";
		return false;
	}

	cout << "Odczytywanie źródeł shaderów... ";
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

	cout << "Odczytano.\nKompilowanie programów shaderów... ";

	const char *vert = vertData.c_str();
	const char *frag = fragData.c_str();

	GLint vlength = vertData.length();
	GLint flength = fragData.length();
	
	glShaderSourceARB(vertexShader_, 1, (const GLchar**)&vert, &vlength);
	glShaderSourceARB(fragmentShader_, 1, (const GLchar**)&frag, &flength);

	int result;

	glCompileShaderARB(vertexShader_);
	glGetShaderiv(vertexShader_, GL_COMPILE_STATUS, &result);
	if (!result) {
		char msg[MAX_LOG_SIZE];
		glGetShaderInfoLog(vertexShader_, MAX_LOG_SIZE, NULL, msg);
		cout << "\n   Błąd kompilacji vertex shadera! Log kompilacji:\n" << msg << endl;
		return 0;
	}

	glCompileShaderARB(fragmentShader_);
	glGetShaderiv(fragmentShader_, GL_COMPILE_STATUS, &result);
	if (!result) {
		char msg[MAX_LOG_SIZE];
		glGetShaderInfoLog(fragmentShader_, MAX_LOG_SIZE, NULL, msg);
		cout << "\n   Błąd kompilacji fragment shadera! Log kompilacji:\n" << msg << endl;
		return 0;
	}

	shaderProgram_ = glCreateProgram();
	glAttachObjectARB(shaderProgram_, vertexShader_);
	glAttachObjectARB(shaderProgram_, fragmentShader_);

	glLinkProgramARB(shaderProgram_);

	glGetProgramiv(shaderProgram_, GL_LINK_STATUS, &result);
	if (!result) {
		char msg[MAX_LOG_SIZE];
		glGetProgramInfoLog(shaderProgram_, MAX_LOG_SIZE, NULL, msg);
		cout << "\n   Błąd przy linkowaniu shadera! Log linkera:\n" << msg << endl;
		return false;
	}

	cout << "Zrobione!\n";
	return true;

}

bool
Shader::toggle() {
	if (!isRunning_) {
		glUseProgram(shaderProgram_);
		isRunning_ = true;
	} else {
		glUseProgram(0);
		isRunning_ = false;
	}
	return true;
}

bool
Shader::bind(Object *_dest) {
	_dest -> shader_ = this;
	return true;
}

bool
Shader::fileExists(const std::string &_fileName) {
	struct stat buf;
	if (stat(_fileName.c_str(), &buf) == 0)
		return 1;
	else
		return 0;
}
