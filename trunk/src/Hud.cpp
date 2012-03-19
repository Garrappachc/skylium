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
		__background({1.0f, 0.0f, 0.0f, 0.6f}),
		__border({1.0f, 0.0f, 0.0f, 0.8f}),
		__hudShader(NULL),
		__matrices(MatricesManager::GetSingleton()) {
			
	log(CONSTRUCTOR, "Hud constructed.");
}

Hud::~Hud() {
	delete __hudShader;
	log(DESTRUCTOR, "Hud destructed.");
}

void
Hud::draw() {
		//  Hud Mode on
	__hudMode(true);
	
	__hudShader -> toggle();
	__hudShader -> setUniformFloat("sDefColor", __background);
	
	gl::BindVertexArray(__vaoID);
	gl::DrawArrays(GL_QUADS, 0, 4);
	checkGLErrors(AT);
	
	__hudShader -> setUniformFloat("sDefColor", __border);
	gl::LineWidth(2.0f);
	gl::DrawArrays(GL_LINE_LOOP, 0, 4);
	checkGLErrors(AT);
	
	__hudShader -> toggle();
	
	__displayList = __toDisplay.begin();
	while (__displayList != __toDisplay.end()) {
		gl::Color4f(
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
	
	gl::BindVertexArray(0);
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
	log(PARAM, "Hud: attached \"%s\".", _newdata -> text.c_str());
}

void
Hud::prepare() {
	string vertexCode = 
		"void main() {"
		"	gl_Position = sVertex;"
		"}";
	
	string fragmentCode =
		"void main() {"
		"	sFragColor = sDefColor;"
		"}";
	
	__hudShader = new Shader(vertexCode, fragmentCode);
	if (!__hudShader -> make(0, "sVertex", 0, "", 0, "")) {
		log(ERROR, "Hud: shader compilation error occured; exiting.\n");
	}
	
	gl::GenVertexArrays(1, &__vaoID);
	gl::GenBuffers(1, &__vboID);
	checkGLErrors(AT);
	
	gl::BindVertexArray(__vaoID);
	gl::BindBuffer(GL_ARRAY_BUFFER, __vboID);
	checkGLErrors(AT);
	
	gl::BufferData(GL_ARRAY_BUFFER, sizeof(gl::Float) * 16, NULL, GL_STATIC_DRAW);
	gl::BufferSubData(GL_ARRAY_BUFFER, 0, sizeof(gl::Float) * 16, __vertices);
	checkGLErrors(AT);
	
	gl::VertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, NULL);
	gl::EnableVertexAttribArray(0);
	checkGLErrors(AT);
	
	gl::BindVertexArray(0);
}

void
Hud::__hudMode(bool flag) {
	if (flag) {
		__matrices.storeProjectionMatrix();
		__matrices.sOrtho(-1.0, 1.0, 1.0, -1.0, 0.0, 1.0);
		
		gl::Enable(GL_BLEND);
		gl::BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	} else {
		gl::Disable(GL_BLEND);
		
		__matrices.restoreProjectionMatrix();
	}
	checkGLErrors(AT);
}