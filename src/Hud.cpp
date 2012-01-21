/*
     _  _         _                
    | || |_  _ __| |  __ _ __ _ __ 
    | __ | || / _` |_/ _| '_ \ '_ \
    |_||_|\_,_\__,_(_)__| .__/ .__/
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


#include "../include/Hud.h"
#include "../include/Skylium.h"
#include "../include/FontBase.h"
#include "../include/HudData.h"
#include "../include/Shader.h"
#include "../include/MatricesManager.h"

#include "../include/defines.h"
#include "../include/config.h"
#include "../include/utils.h"

using namespace std;

Hud::Hud() :
		__vertices({
				-0.8, 1.0, 0.0, 1.0,
				-0.8, 0.6, 0.0, 1.0,
				0.8, 0.6, 0.0, 1.0,
				0.8, 1.0, 0.0, 1.0
			
			}),
		__visible(false),
		__toDisplay(0),
		__background(1.0f, 0.0f, 0.0f, 0.6f),
		__border(1.0f, 0.0f, 0.0f, 0.8f),
		__hudShader(NULL),
		__matrices(MatricesManager::GetSingleton()) {
			
	__initGLExtensionsPointers();
	
	if ((sGlobalConfig::DEBUGGING & D_CONSTRUCTORS) == D_CONSTRUCTORS)
		cout << LOG_INFO << "Hud constructed.";
}

Hud::~Hud() {
	delete __hudShader;
	if ((sGlobalConfig::DEBUGGING & D_DESTRUCTORS) == D_DESTRUCTORS)
		cout << LOG_INFO << "Hud destructed.";
}

void
Hud::draw() {
		//  Hud Mode on
	__hudMode(true);
	
	__hudShader -> toggle();
	__hudShader -> setUniformFloat("sDefColor", __background);
	
	glBindVertexArray(__vaoID);
	glDrawArrays(GL_QUADS, 0, 4);
	checkGLErrors(AT);
	
	__hudShader -> setUniformFloat("sDefColor", __border);
	glLineWidth(2.0f);
	glDrawArrays(GL_LINE_LOOP, 0, 4);
	checkGLErrors(AT);
	
	__hudShader -> toggle();
	checkGLErrors(AT);
	
	__displayList = __toDisplay.begin();
	while (__displayList != __toDisplay.end()) {
		glColor4f(
				(*__displayList) -> color.r,
				(*__displayList) -> color.g,
				(*__displayList) -> color.b,
				(*__displayList) -> color.a
			);
		(*__displayList) -> font -> print(
				(*__displayList) -> position.x,
				(*__displayList) -> position.y,
				(*__displayList) -> text
		);
		__displayList++;
	}
	
	__hudMode(false);
	
	glBindVertexArray(0);
	checkGLErrors(AT);
}

void
Hud::toggle() {
	if (sGlobalConfig::HUD_EXISTS)
		__visible = !__visible;
}

void
Hud::attachData(HudData *_newdata) {
	__toDisplay.push_back(_newdata);
	if ((sGlobalConfig::DEBUGGING & D_PARAMS) == D_PARAMS)
		cout << LOG_INFO << "Hud: attached (\"" << _newdata -> text << "\")";
}

void
Hud::prepare() {
	__hudShader = new Shader("hud_identity.vert", "hud_identity.frag");
	if (!__hudShader -> make(0, "sVertex", 0, "", 0, "")) {
		if ((sGlobalConfig::DEBUGGING & D_ERRORS) == D_ERRORS)
			cout << LOG_ERROR << "Hud: shader compilation error occured; exiting.\n";
		exit(1);
	}
	
	glGenVertexArrays(1, &__vaoID);
	glGenBuffers(1, &__vboID);
	
	glBindVertexArray(__vaoID);
	glBindBuffer(GL_ARRAY_BUFFER, __vboID);
	
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 16, NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * 16, __vertices);
	
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, NULL);
	
	glEnableVertexAttribArray(0);
	
	glBindVertexArray(0);
	checkGLErrors(AT);
}

void
Hud::__hudMode(bool flag) {
	if (flag) {
		/* At this point, we don't manipulate Skylium's matrices,
		   as in the hud shader we don't get any matrices for transform.
		   But we use GL's lists. */
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();

		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	} else {
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
		
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);
	}
	checkGLErrors(AT);
}

void
Hud::__initGLExtensionsPointers() {
	glGenVertexArrays = getProcAddr< decltype(glGenVertexArrays) >("glGenVertexArrays");
	glBindVertexArray = getProcAddr< decltype(glBindVertexArray) >("glBindVertexArray");
	glBindBuffer =	getProcAddr< decltype(glBindBuffer) >("glBindBufferARB");
	glDeleteBuffers = getProcAddr< decltype(glDeleteBuffers) >("glDeleteBuffersARB");
	glDeleteVertexArrays = getProcAddr< decltype(glDeleteVertexArrays) >("glDeleteVertexArrays");
	glGenBuffers = getProcAddr< decltype(glGenBuffers) >("glGenBuffersARB");
	glBufferData = getProcAddr< decltype(glBufferData) >("glBufferDataARB");
	glBufferSubData = getProcAddr< decltype(glBufferSubData) >("glBufferSubDataARB");
	glGetBufferParameteriv = getProcAddr< decltype(glGetBufferParameteriv) >("glGetBufferParameterivARB");
	glVertexAttribPointer = getProcAddr< decltype(glVertexAttribPointer) >("glVertexAttribPointer");
	glEnableVertexAttribArray = getProcAddr< decltype(glEnableVertexAttribArray) >("glEnableVertexAttribArray");
	glDisableVertexAttribArray = getProcAddr< decltype(glDisableVertexAttribArray) >("glDisableVertexAttribArray");
}

