#version 150

uniform vec4 sDefColor;
uniform mat4 sModelViewMatrix;
uniform mat4 sProjectionMatrix;

void main() {
	gl_Position = sProjectionMatrix * sModelViewMatrix * gl_Vertex;
	gl_FrontColor = sDefColor;
}