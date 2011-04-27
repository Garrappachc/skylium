/* 
 * File:   Box.cpp
 * Author: Michał Garapich
 * 
 * Created on 3 april 2011, 17:59
 */

#include "../include/Box.h"
#include "../include/Object.h"
#include "../include/Shader.h"
#include "../include/shapes.h"

using namespace std;

Box::Box(const string &_name) {
	name = _name;
	pointers_ = vector < GLfloat >(boxPointers, boxPointers + sizeof(boxPointers) / sizeof(GLfloat));
	normals_ = vector < GLfloat >(boxNormals, boxNormals + sizeof(boxNormals) / sizeof(GLfloat));

}

Box::~Box() {
	
}

bool
Box::show() {
	glPushMatrix();
	if (!texture_)
		glColor4f(defColor_[0], defColor_[1], defColor_[2], defColor_[3]);
	else {
		glColor4f(1.0, 1.0, 1.0, 1.0);
		glBindTexture(GL_TEXTURE_2D, texture_);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	}

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnableClientState(GL_NORMAL_ARRAY);

	glEnableClientState(GL_VERTEX_ARRAY);

	if (texture_) {
		glEnable(GL_TEXTURE_2D);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	}

	glTranslated(mov_[0], mov_[1], mov_[2]);

	glScaled(scale_[0], scale_[1], scale_[2]);

	glRotated(rot_[0], 1.0, 0, 0);
	glRotated(rot_[1], 0, 1.0, 0);
	glRotated(rot_[2], 0, 0, 1.0);
	
	if (shader_)
		shader_ -> toggle();

	glVertexPointer(3, GL_FLOAT, 0, (GLvoid*)&pointers_[0]);
	if (texture_)
		glTexCoordPointer(2, GL_FLOAT, 0, (GLvoid*)&texCoords_[0]);
	glNormalPointer(GL_FLOAT, 0, (GLvoid*)&normals_[0]);
	glDrawArrays(GL_QUADS, 0, (int)(pointers_.size() / 3));

	if (texture_)
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	
	if (shader_)
		shader_ -> toggle();

	glPopMatrix();

	return true;
}