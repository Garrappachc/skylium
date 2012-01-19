#version 120

varying vec3 normal;

void main() {
	normal = sNormalMatrix * gl_Normal;

	gl_Position = sProjectionMatrix * sModelViewMatrix * gl_Vertex;
	gl_FrontColor = sDefColor;
}