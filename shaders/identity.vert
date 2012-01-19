#version 120

void main() {
	gl_Position = sProjectionMatrix * sModelViewMatrix * gl_Vertex;
	gl_FrontColor = sDefColor;
}