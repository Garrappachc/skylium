/*
     ___ _            _                       
    / __| |_  __ _ __| |___ _ _  __ _ __ _ __ 
    \__ \ ' \/ _` / _` / -_) '_|/ _| '_ \ '_ \
    |___/_||_\__,_\__,_\___|_|(_)__| .__/ .__/
                                   |_|  |_|   
                               
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

#include <iostream>
#include <string>
#include <fstream>

#include <sys/stat.h>
#include <GL/gl.h>

#include "../include/Shader.h"

#include "../include/Vectors.h"
#include "../include/Skylium.h"

#include "../include/defines.h"
#include "../include/config.h"
#include "../include/utils.h"

using namespace std;

static const unsigned int MAX_LOG_SIZE = 4096;


Shader::Shader(const string& _fileName) : 
		__vertFile(_fileName + ".vert"),
		__fragFile(_fileName + ".frag"),
		__vertCode(""),
		__fragCode(""),
		__isRunning(false),
		__isCompiled(false) {

	__initGLExtensionsPointers();
	
	__vertFile = "shaders/" + __vertFile;
	
	__fragFile = "shaders/" + __fragFile;
	
	__vertexShader = glCreateShader(GL_VERTEX_SHADER);
	__fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	
	if ((sGlobalConfig::DEBUGGING & D_CONSTRUCTORS) == D_CONSTRUCTORS)
		cout << LOG_INFO << "Shader (\"" << __vertFile << "\", \"" << __fragFile << "\") constructed.";
}

Shader::Shader(const string& _vertexCode, const string& _fragmentCode) :
		__vertFile(""),
		__fragFile(""),
		__vertCode(_vertexCode),
		__fragCode(_fragmentCode),
		__isRunning(false),
		__isCompiled(false) {
	
	__initGLExtensionsPointers();
	
	__vertexShader = glCreateShader(GL_VERTEX_SHADER);
	__fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	
	if ((sGlobalConfig::DEBUGGING & D_CONSTRUCTORS) == D_CONSTRUCTORS)
		cout << LOG_INFO << "Shader constructed (from source code).";
}

Shader::~Shader() {
	glDetachShader(__shaderProgram, __vertexShader);
	glDetachShader(__shaderProgram, __fragmentShader);

	glDeleteProgram(__shaderProgram);

	glDeleteShader(__vertexShader);
	glDeleteShader(__fragmentShader);
	checkGLErrors(AT);
	if ((sGlobalConfig::DEBUGGING & D_DESTRUCTORS) == D_DESTRUCTORS)
		cout << LOG_INFO << "Shader (\"" << __vertFile << "\", \"" << __fragFile << "\") destructed.";

}

bool
Shader::make(GLuint _var1, const string& _param1,
		GLuint _var2, const string& _param2,
		GLuint _var3, const string& _param3) {
	const char *vert;
	const char *frag;
	
	static const string HEADER = 
		"#version " GLSLLANGVERSION "\n"
		"struct sMaterialParams {"
		" vec4 emission;"
		" vec4 ambient;"
		" vec4 diffuse;"
		" vec4 specular;"
		" float shininess;"
		"};"
		
		"struct sLightParams {"
		" vec4 ambient;"
		" vec4 diffuse;"
		" vec4 specular;"
		" vec4 position;"
		" float constantAttenuation;"
		" float linearAttenuation;"
		" float quadraticAttenuation;"
		"};"
		
		"struct sLightModelParameters {"
		" vec4 ambient;"
		"};"
		
		"uniform vec4 sDefColor;"
		"uniform mat4 sModelViewMatrix;"
		"uniform mat4 sProjectionMatrix;"
		"uniform mat4 sModelViewProjectionMatrix;"
		"uniform mat3 sNormalMatrix;"
		"uniform sMaterialParams sFrontMaterial;"
		"uniform sLightParams sLightSource[7];"
		"uniform sLightModelParameters sLightModel;\n";
	
	static const string VERTEX_HEADER =
		"in vec4 sVertex;"
		"in vec3 sNormal;"
		"in vec2 sTexCoords;"
		"smooth out vec2 sVaryingTexCoords;\n";
	
	static const string FRAGMENT_HEADER =
		"out vec4 sFragColor;"
		"smooth in vec2 sVaryingTexCoords;"
		"uniform sampler2D textureUnit;"
		"uniform sampler2D normalMap;"
		"uniform sampler2D specularMap;";
	
	if (!__vertFile.empty() && !__fragFile.empty()) {
		if (!__fileExists(__vertFile)) {
			if ((sGlobalConfig::DEBUGGING & D_ERRORS) == D_ERRORS)
				cout << LOG_ERROR << "File \"" << __vertFile << "\" not found!";
			return false;
		}
		if (!__fileExists(__fragFile)) {
			if ((sGlobalConfig::DEBUGGING & D_ERRORS) == D_ERRORS)
				cout << LOG_ERROR << "File \"" << __fragFile << "\" not found!";
			return false;
		}
	
		if ((sGlobalConfig::DEBUGGING & D_SHADERS) == D_SHADERS)
			cout << LOG_INFO << "Reading shaders' sources... ";
		ifstream vertFile(__vertFile.c_str());
		__vertCode = HEADER + VERTEX_HEADER;
		while (!vertFile.eof()) {
			string temp = "";
			getline(vertFile, temp);
			__vertCode += temp;
			__vertCode += "\n";	
		}
		vertFile.close();

		ifstream fragFile(__fragFile.c_str());
		__fragCode = HEADER + FRAGMENT_HEADER;
		while (!fragFile.eof()) {
			string temp = "";
			getline(fragFile, temp);
			
			__fragCode += temp;
			__fragCode += "\n";
		}
		fragFile.close();
		
			
		if ((sGlobalConfig::DEBUGGING & D_SHADERS) == D_SHADERS)
			cout << "Done. ";
	} else if (!__vertCode.empty() && !__fragCode.empty()) {
		__vertCode = HEADER + VERTEX_HEADER + __vertCode;
		__fragCode = HEADER + FRAGMENT_HEADER + __fragCode;
	} else {
		if ((sGlobalConfig::DEBUGGING & D_ERRORS) == D_ERRORS)
			cout << LOG_ERROR << "Shader source could not be obtained!\n";
		exit(1);
	}
	
	if (sGlobalConfig::DEBUGGING & D_SHADERS) {
		cout << LOG_INFO << "Compiling shaders' sources... ";
	}

	vert = __vertCode.c_str();
	frag = __fragCode.c_str();

	GLint vlength = __vertCode.length();
	GLint flength = __fragCode.length();
	
	glShaderSource(__vertexShader, 1, (const GLchar**)&vert, &vlength);
	checkGLErrors(AT);
	glShaderSource(__fragmentShader, 1, (const GLchar**)&frag, &flength);
	checkGLErrors(AT);

	int result;

	glCompileShader(__vertexShader);
	checkGLErrors(AT);
	glGetShaderiv(__vertexShader, GL_COMPILE_STATUS, &result);
	checkGLErrors(AT);
	if (!result) {
		char msg[MAX_LOG_SIZE];
		glGetShaderInfoLog(__vertexShader, MAX_LOG_SIZE, NULL, msg);
		if ((sGlobalConfig::DEBUGGING & D_ERRORS) == D_ERRORS)
			cout << LOG_ERROR << "Error compiling vertex shader! Compilation log:\n" << msg << endl;
		return 0;
	}

	glCompileShader(__fragmentShader);
	checkGLErrors(AT);
	glGetShaderiv(__fragmentShader, GL_COMPILE_STATUS, &result);
	checkGLErrors(AT);
	if (!result) {
		char msg[MAX_LOG_SIZE];
		glGetShaderInfoLog(__fragmentShader, MAX_LOG_SIZE, NULL, msg);
		if ((sGlobalConfig::DEBUGGING & D_ERRORS) == D_ERRORS)
			cout << LOG_ERROR << "Error compiling fragment shader! Compilation log:\n" << msg << endl;
		return 0;
	}

	__shaderProgram = glCreateProgram();
	checkGLErrors(AT);
	if (!__shaderProgram) {
		if ((sGlobalConfig::DEBUGGING & D_ERRORS) == D_ERRORS)
			cout << LOG_ERROR << "Error creating the shader program!" << endl;
		return false;
	} 
	
	glAttachShader(__shaderProgram, __vertexShader);
	checkGLErrors(AT);
	glAttachShader(__shaderProgram, __fragmentShader);
	checkGLErrors(AT);
	
	if (!_param1.empty())
		glBindAttribLocation(__shaderProgram, _var1, _param1.c_str());
	if (!_param2.empty())
		glBindAttribLocation(__shaderProgram, _var2, _param2.c_str());
	if (!_param3.empty())
		glBindAttribLocation(__shaderProgram, _var3, _param3.c_str());

	glLinkProgram(__shaderProgram);
	checkGLErrors(AT);

	glGetProgramiv(__shaderProgram, GL_LINK_STATUS, &result);
	checkGLErrors(AT);
	if (!result) {
		char msg[MAX_LOG_SIZE];
		glGetProgramInfoLog(__shaderProgram, MAX_LOG_SIZE, NULL, msg);
		if ((sGlobalConfig::DEBUGGING & D_ERRORS) == D_ERRORS)
			cout << LOG_ERROR << "Error linking shader program! Link log:\n" << msg << endl;
		return false;
	}
	checkGLErrors(AT);
	
	if (glIsProgram(__shaderProgram) == GL_FALSE) {
		if ((sGlobalConfig::DEBUGGING & D_ERRORS) == D_ERRORS)
			cout << LOG_ERROR << "Shader program could not be created!";
		return false;
	}

	if ((sGlobalConfig::DEBUGGING & D_SHADERS) == D_SHADERS)
		cout << "Done. No errors reported.";
	
	__isCompiled = true;
	
	return true;

}

void
Shader::toggle() {
	if (!__isCompiled)
		if ((sGlobalConfig::DEBUGGING & D_WARNINGS) == D_WARNINGS)
			cout << LOG_WARN << "\nWarning: this shader is not compiled!";
		
	if (!__isRunning) {
		glUseProgram(__shaderProgram);
		checkGLErrors(AT);
		__isRunning = true;
#ifdef __DEBUG__
		cout << "\nShader on.";
		cout.flush();
#endif // __DEBUG__
	} else {
		glUseProgram(0);
		checkGLErrors(AT);
		__isRunning = false;
#ifdef __DEBUG__
		cout << "\nShader off.";
		cout.flush();
#endif // __DEBUG__
	}
}

void
Shader::bind(Object *_dest) {
	_dest -> __shader = this;
	if ((sGlobalConfig::DEBUGGING & D_SHADERS) == D_SHADERS)
		cout << LOG_INFO << "Shader bound to " << _dest -> name << ".";
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

void
Shader::setUniformFloat(const string& _name, const sVectorBase< GLfloat >& _params) const {
	GLint location = glGetUniformLocation(__shaderProgram, _name.c_str());
	checkGLErrors(AT);
	
	switch (_params.size()) {
		case 2:
			glUniform2f(location, _params[0], _params[1]);
		case 3:
			glUniform3f(location, _params[0], _params[1], _params[2]);
		case 4:
			glUniform4f(location, _params[0], _params[1], _params[2], _params[3]);
	}
	checkGLErrors(AT);
}

void
Shader::setUniformFloat(const string& _name, GLfloat _param) const {
	GLint location = glGetUniformLocation(__shaderProgram, _name.c_str());
	checkGLErrors(AT);
	
	glUniform1f(location, _param);
	checkGLErrors(AT);
}

void
Shader::setUniformInt(const string& _name, GLint _value) const {
	GLint location = glGetUniformLocation(__shaderProgram, _name.c_str());
	checkGLErrors(AT);
	
	glUniform1i(location, _value);
	checkGLErrors(AT);
}

void
Shader::setMatrixFloat(const string& _name, const sMat16& _matrix) const {
	checkGLErrors(AT);
	GLint location = glGetUniformLocation(__shaderProgram, _name.c_str());
	checkGLErrors(AT);
	
	glUniformMatrix4fv(location, 1, GL_FALSE, _matrix);
	checkGLErrors(AT);
}

void
Shader::setMatrixFloat(const string& _name, const sMat9& _matrix) const {
	GLint location = glGetUniformLocation(__shaderProgram, _name.c_str());
	checkGLErrors(AT);
	
	glUniformMatrix3fv(location, 1, GL_FALSE, _matrix);
	checkGLErrors(AT);
}

void
Shader::__initGLExtensionsPointers() {
	glCreateShader = getProcAddr< decltype(glCreateShader) >("glCreateShader");
	glDetachShader = getProcAddr< decltype(glDetachShader) >("glDetachShader");
	glDeleteProgram = getProcAddr< decltype(glDeleteProgram) >("glDeleteProgram");
	glDeleteShader = getProcAddr< decltype(glDeleteShader) >("glDeleteShader");
	glShaderSource = getProcAddr< decltype(glShaderSource) >("glShaderSource");
	glCompileShader = getProcAddr< decltype(glCompileShader) >("glCompileShader");
	glGetShaderiv = getProcAddr< decltype(glGetShaderiv) >("glGetShaderiv");
	glGetShaderInfoLog = getProcAddr< decltype(glGetShaderInfoLog) >("glGetShaderInfoLog");
	glIsProgram = getProcAddr< decltype(glIsProgram) >("glIsProgram");
	glCreateProgram = getProcAddr< decltype(glCreateProgram) >("glCreateProgram");
	glAttachShader = getProcAddr< decltype(glAttachShader) >("glAttachShader");
	glBindAttribLocation = getProcAddr< decltype(glBindAttribLocation) >("glBindAttribLocation");
	glLinkProgram = getProcAddr< decltype(glLinkProgram) >("glLinkProgram");
	glGetProgramiv = getProcAddr< decltype(glGetProgramiv) >("glGetProgramiv");
	glGetProgramInfoLog = getProcAddr< decltype(glGetProgramInfoLog) >("glGetProgramInfoLog");
	glUseProgram = getProcAddr< decltype(glUseProgram) >("glUseProgram");
	glGetUniformLocation = getProcAddr< decltype(glGetUniformLocation) >("glGetUniformLocation");
	glUniform1i = getProcAddr< decltype(glUniform1i) >("glUniform1i");
	glUniform1f = getProcAddr< decltype(glUniform1f) >("glUniform1f");
	glUniform2f = getProcAddr< decltype(glUniform2f) >("glUniform2f");
	glUniform3f = getProcAddr< decltype(glUniform3f) >("glUniform3f");
	glUniform4f = getProcAddr< decltype(glUniform4f) >("glUniform4f");
	glUniformMatrix4fv = getProcAddr< decltype(glUniformMatrix4fv) >("glUniformMatrix4fv");
	glUniformMatrix3fv = getProcAddr< decltype(glUniformMatrix3fv) >("glUniformMatrix3fv");
}

bool
Shader::__fileExists(const std::string &_fileName) {
	struct stat buf;
	if (stat(_fileName.c_str(), &buf) == 0)
		return 1;
	else
		return 0;
}