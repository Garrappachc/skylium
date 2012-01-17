#version 150

uniform vec4 sDefColor;
uniform mat4 sModelViewMatrix;
uniform mat4 sProjectionMatrix;

varying vec3 normal;

void main() {
	normal = gl_NormalMatrix * gl_Normal;

	gl_Position = sProjectionMatrix * sModelViewMatrix * gl_Vertex;
	gl_FrontColor = sDefColor;
}