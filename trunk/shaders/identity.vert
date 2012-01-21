#version 120

void main() {
	gl_Position = sModelViewProjectionMatrix * gl_Vertex;
	gl_FrontColor = sDefColor;
}