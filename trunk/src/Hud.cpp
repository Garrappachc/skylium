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
		__vertices({-0.8, 1.0, -0.8, 0.6, 0.8, 0.6, 0.8, 1.0}),
		__visible(false),
		__toDisplay(0),
		__background(1.0f, 0.0f, 0.0f, 0.6f),
		__border(1.0f, 0.0f, 0.0f, 0.8f),
		__hudShader(NULL),
		__matrices(MatricesManager::GetSingleton()) {
	
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

	glEnableClientState(GL_VERTEX_ARRAY);
	checkGLErrors(AT);
	
	__hudShader -> toggle();
	__hudShader -> setUniformFloat("sDefColor", __background);
	
	glVertexPointer(2, GL_FLOAT, 0, __vertices);
	checkGLErrors(AT);
	glDrawArrays(GL_QUADS, 0, 4);
	checkGLErrors(AT);
	
	__hudShader -> setUniformFloat("sDefColor", __border);
	glVertexPointer(2, GL_FLOAT, 0, __vertices);
	glLineWidth(2.0f);
	glDrawArrays(GL_LINE_LOOP, 0, 4);
	glDisableClientState(GL_VERTEX_ARRAY);
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
	if (!__hudShader -> make()) {
		if ((sGlobalConfig::DEBUGGING & D_ERRORS) == D_ERRORS)
			cout << LOG_ERROR << "Hud: shader compilation error occured; exiting.\n";
		exit(1);
	}
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
		glDisable(GL_COLOR_MATERIAL);
		glDisable(GL_LIGHTING);
	} else {
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
		
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_COLOR_MATERIAL);
		glEnable(GL_LIGHTING);
	}
	checkGLErrors(AT);
}

