/*
     ___ _            _         ___       _        _  _              _ _                       
    / __| |_  __ _ __| |___ _ _|   \ __ _| |_ __ _| || |__ _ _ _  __| | |___ _ _  __ _ __ _ __ 
    \__ \ ' \/ _` / _` / -_) '_| |) / _` |  _/ _` | __ / _` | ' \/ _` | / -_) '_|/ _| '_ \ '_ \
    |___/_||_\__,_\__,_\___|_| |___/\__,_|\__\__,_|_||_\__,_|_||_\__,_|_\___|_|(_)__| .__/ .__/
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


#include "../include/ShaderDataHandler.h"

#include "../include/Skylium.h"
#include "../include/MatricesManager.h"
#include "../include/Shader.h"

#include "../include/defines.h"
#include "../include/config.h"
#include "../include/utils.h"

using namespace std;

ShaderDataHandler::ShaderDataHandler() :
		__matrices(MatricesManager::GetSingleton()),
		__isStreamOpen(false) {
	if ((sGlobalConfig::DEBUGGING & D_CONSTRUCTORS) == D_CONSTRUCTORS)
		cout << LOG_INFO << "ShaderDataHandler constructed.";
}

void
ShaderDataHandler::updateData(const string& _uniformName, const sVector2D& _value) {
	if (__isStreamOpen) {
		__activeShader->setUniformFloat(_uniformName, _value);
#ifdef __DEBUG__
		cout << "\n  Data (" << _uniformName << ") sent via stream.";
		cout.flush();
#endif // __DEBUG__
	} else {
		auto result = __2Dvectors.find(_uniformName);
		if (result == __2Dvectors.end())
			__2Dvectors.insert(make_pair(_uniformName, _value));
		else
			result -> second = _value;
#ifdef __DEBUG__
		cout << "\nData (" << _uniformName << ") stored.";
		cout.flush();
#endif // __DEBUG__
	}
}

void
ShaderDataHandler::updateData(const string& _uniformName, const sVector3D& _value) {
	if (__isStreamOpen) {
		__activeShader->setUniformFloat(_uniformName, _value);
#ifdef __DEBUG__
		cout << "\n  Data (" << _uniformName << ") sent via stream.";
		cout.flush();
#endif // __DEBUG__
	} else {
		auto result = __3Dvectors.find(_uniformName);
		if (result == __3Dvectors.end())
			__3Dvectors.insert(make_pair(_uniformName, _value));
		else
			result -> second = _value;
#ifdef __DEBUG__
		cout << "\nData (" << _uniformName << ") stored.";
		cout.flush();
#endif // __DEBUG__
	}
}

void
ShaderDataHandler::updateData(const string& _uniformName, const sVector4D& _value) {
	if (__isStreamOpen) {
		__activeShader->setUniformFloat(_uniformName, _value);
#ifdef __DEBUG__
		cout << "\n  Data (" << _uniformName << ") sent via stream.";
		cout.flush();
#endif // __DEBUG__
	} else {
		auto result = __4Dvectors.find(_uniformName);
		if (result == __4Dvectors.end())
			__4Dvectors.insert(make_pair(_uniformName, _value));
		else
			result -> second = _value;
#ifdef __DEBUG__
		cout << "\nData (" << _uniformName << ") stored.";
		cout.flush();
#endif // __DEBUG__
	}
}

void
ShaderDataHandler::updateData(const string& _uniformName, GLfloat _value) {
	if (__isStreamOpen) {
		__activeShader->setUniformFloat(_uniformName, _value);
#ifdef __DEBUG__
		cout << "\n  Data (" << _uniformName << ") sent via stream.";
		cout.flush();
#endif // __DEBUG__
	} else {
		auto result = __values.find(_uniformName);
		if (result == __values.end())
			__values.insert(make_pair(_uniformName, _value));
		else
			result -> second = _value;
#ifdef __DEBUG__
		cout << "\nData (" << _uniformName << ") stored.";
		cout.flush();
#endif // __DEBUG__
	}
}

void
ShaderDataHandler::updateSampler2D(const string& _uniformName, GLint _sampler) {
	if (__isStreamOpen) {
		__activeShader->setUniformInt(_uniformName, _sampler);
#ifdef __DEBUG__
		cout << "\n  Data (" << _uniformName << ") sent via stream.";
		cout.flush();
#endif // __DEBUG__
	} else {
		auto result = __textures.find(_uniformName);
		if (result == __textures.end())
			__textures.insert(make_pair(_uniformName, _sampler));
		else
			result -> second = _sampler;
#ifdef __DEBUG__
		cout << "\nData (" << _uniformName << ") stored.";
		cout.flush();
#endif // __DEBUG__
	}
}

void
ShaderDataHandler::openStream(const Shader* _shader) {
	__isStreamOpen = true;
	__sendDataToShader(_shader);
	__activeShader = _shader;
	
#ifdef __DEBUG__
	cout << "\nStream open.";
	cout.flush();
#endif // __DEBUG__
}

#ifdef __DEBUG__
void
ShaderDataHandler::closeStream() {
	__isStreamOpen = false;
	cout << "\nStream closed.";
	cout.flush();
}
#endif // __DEBUG__

void
ShaderDataHandler::__sendDataToShader(const Shader* _shader) {
	// first, send matrices
	_shader->setMatrixFloat(MODELVIEWMATRIXNAME,	__matrices.getModelViewMatrix());
	_shader->setMatrixFloat(PROJECTIONMATRIXNAME,	__matrices.getProjectionMatrix());
	_shader->setMatrixFloat(NORMALMATRIXNAME,	__matrices.getNormalMatrix());
	_shader->setMatrixFloat(MODELVIEWPROJECTIONMATRIXNAME,
			__matrices.getProjectionMatrix() * __matrices.getModelViewMatrix()
		);
	
	// then send vectors
	for (auto it = __2Dvectors.begin(); it != __2Dvectors.end(); ++it)
		_shader->setUniformFloat(it -> first, it -> second);
	
	for (auto it = __3Dvectors.begin(); it != __3Dvectors.end(); ++it)
		_shader->setUniformFloat(it -> first, it -> second);
	
	for (auto it = __4Dvectors.begin(); it != __4Dvectors.end(); ++it)
		_shader->setUniformFloat(it -> first, it -> second);
	
	// and then single values
	for (auto it = __values.begin(); it != __values.end(); ++it)
		_shader->setUniformFloat(it -> first, it -> second);
	
	for (auto it = __textures.begin(); it != __textures.end(); ++it)
		_shader->setUniformInt(it -> first, it -> second);
	
	checkGLErrors(AT);
}
