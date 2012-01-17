#version 120

uniform vec4 sDefColor;

void main() {
	gl_Position = gl_Vertex;
	gl_FrontColor = sDefColor;
}