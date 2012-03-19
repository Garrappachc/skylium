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
	
	__vertFile = "shaders/" + __vertFile;
	
	__fragFile = "shaders/" + __fragFile;
	
	__vertexShader = gl::CreateShader(GL_VERTEX_SHADER);
	__fragmentShader = gl::CreateShader(GL_FRAGMENT_SHADER);
	
	log(CONSTRUCTOR, "Shader (\"%s\", \"%s\") constructed.", __vertFile.c_str(), __fragFile.c_str());
}

Shader::Shader(const string& _vertexCode, const string& _fragmentCode) :
		__vertFile(""),
		__fragFile(""),
		__vertCode(_vertexCode),
		__fragCode(_fragmentCode),
		__isRunning(false),
		__isCompiled(false) {
			
	__vertexShader = gl::CreateShader(GL_VERTEX_SHADER);
	__fragmentShader = gl::CreateShader(GL_FRAGMENT_SHADER);
	
	log(CONSTRUCTOR, "Shader constructed (from source code).");
}

Shader::~Shader() {
	gl::DetachShader(__shaderProgram, __vertexShader);
	gl::DetachShader(__shaderProgram, __fragmentShader);

	gl::DeleteProgram(__shaderProgram);

	gl::DeleteShader(__vertexShader);
	gl::DeleteShader(__fragmentShader);
	checkGLErrors(AT);
	
	log(DESTRUCTOR, "Shader (\"%s\", \"%s\") destructed.", __vertFile.c_str(), __fragFile.c_str());
}

bool
Shader::make(gl::Uint _var1, const string& _param1,
		   gl::Uint _var2, const string& _param2,
		   gl::Uint _var3, const string& _param3) {
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
			log(ERROR, "File \"%s\" not found!", __vertFile.c_str());
		}
		if (!__fileExists(__fragFile)) {
			log(ERROR, "File \"%s\" not found!", __fragFile.c_str());
		}

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
	} else if (!__vertCode.empty() && !__fragCode.empty()) {
		__vertCode = HEADER + VERTEX_HEADER + __vertCode;
		__fragCode = HEADER + FRAGMENT_HEADER + __fragCode;
	} else {
		log(ERROR, "Shader source could not be obtained!");
	}

	vert = __vertCode.c_str();
	frag = __fragCode.c_str();

	gl::Int vlength = __vertCode.length();
	gl::Int flength = __fragCode.length();
	
	gl::ShaderSource(__vertexShader, 1, (const GLchar**)&vert, &vlength);
	checkGLErrors(AT);
	gl::ShaderSource(__fragmentShader, 1, (const GLchar**)&frag, &flength);
	checkGLErrors(AT);

	int result;

	gl::CompileShader(__vertexShader);
	checkGLErrors(AT);
	gl::GetShaderiv(__vertexShader, GL_COMPILE_STATUS, &result);
	checkGLErrors(AT);
	if (!result) {
		char msg[MAX_LOG_SIZE];
		gl::GetShaderInfoLog(__vertexShader, MAX_LOG_SIZE, NULL, msg);
		
		log(ERROR, "Error compiling vertex shader! Compilation log:\n%s", msg);
	}

	gl::CompileShader(__fragmentShader);
	checkGLErrors(AT);
	gl::GetShaderiv(__fragmentShader, GL_COMPILE_STATUS, &result);
	checkGLErrors(AT);
	if (!result) {
		char msg[MAX_LOG_SIZE];
		gl::GetShaderInfoLog(__fragmentShader, MAX_LOG_SIZE, NULL, msg);
		
		log(ERROR, "Error compiling fragment shader! Compilation log:\n%s", msg);
	}

	__shaderProgram = gl::CreateProgram();
	checkGLErrors(AT);
	if (!__shaderProgram)
		log(ERROR, "Error creating shader program!");
	
	gl::AttachShader(__shaderProgram, __vertexShader);
	checkGLErrors(AT);
	gl::AttachShader(__shaderProgram, __fragmentShader);
	checkGLErrors(AT);
	
	if (!_param1.empty())
		gl::BindAttribLocation(__shaderProgram, _var1, _param1.c_str());
	if (!_param2.empty())
		gl::BindAttribLocation(__shaderProgram, _var2, _param2.c_str());
	if (!_param3.empty())
		gl::BindAttribLocation(__shaderProgram, _var3, _param3.c_str());

	gl::LinkProgram(__shaderProgram);
	checkGLErrors(AT);

	gl::GetProgramiv(__shaderProgram, GL_LINK_STATUS, &result);
	checkGLErrors(AT);
	if (!result) {
		char msg[MAX_LOG_SIZE];
		gl::GetProgramInfoLog(__shaderProgram, MAX_LOG_SIZE, NULL, msg);
		
		log(ERROR, "Error linking shader program! Linking log:\n%s", msg);
	}
	checkGLErrors(AT);
	
	if (gl::IsProgram(__shaderProgram) == GL_FALSE)
		log(ERROR, "Error creating shader program!");

	log(SHADER, "Shader compiled. No errors reported.");
	
	__isCompiled = true;
	
	return true;

}

void
Shader::toggle() {
	if (!__isRunning) {
		gl::UseProgram(__shaderProgram);
		checkGLErrors(AT);
		__isRunning = true;
	} else {
		gl::UseProgram(0);
		checkGLErrors(AT);
		__isRunning = false;
	}
}

void
Shader::bind(Object *_dest) {
	if (!__isCompiled)
		make();
	
	_dest -> __shader = this;
	
	log(SHADER, "Shader bound to %s.", _dest -> name.c_str());
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
Shader::setUniformFloat(const string& _name, const sVectorBase< gl::Float >& _params) const {
	gl::Int location = gl::GetUniformLocation(__shaderProgram, _name.c_str());
	checkGLErrors(AT);
	
	switch (_params.size()) {
		case 2:
			gl::Uniform2f(location, _params[0], _params[1]);
		case 3:
			gl::Uniform3f(location, _params[0], _params[1], _params[2]);
		case 4:
			gl::Uniform4f(location, _params[0], _params[1], _params[2], _params[3]);
	}
	checkGLErrors(AT);
}

void
Shader::setUniformFloat(const string& _name, gl::Float _param) const {
	gl::Uniform1f(gl::GetUniformLocation(__shaderProgram, _name.c_str()), _param);
	checkGLErrors(AT);
}

void
Shader::setUniformInt(const string& _name, gl::Int _value) const {
	gl::Uniform1i(gl::GetUniformLocation(__shaderProgram, _name.c_str()), _value);
	checkGLErrors(AT);
}

void
Shader::setMatrixFloat(const string& _name, const sMat16& _matrix) const {
	gl::UniformMatrix4fv(gl::GetUniformLocation(__shaderProgram, _name.c_str()), 1, GL_FALSE, _matrix);
	checkGLErrors(AT);
}

void
Shader::setMatrixFloat(const string& _name, const sMat9& _matrix) const {
	gl::UniformMatrix3fv(gl::GetUniformLocation(__shaderProgram, _name.c_str()), 1, GL_FALSE, _matrix);
	checkGLErrors(AT);
}

bool
Shader::__fileExists(const std::string &_fileName) {
	struct stat buf;
	if (stat(_fileName.c_str(), &buf) == 0)
		return 1;
	else
		return 0;
}