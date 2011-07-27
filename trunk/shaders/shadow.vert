#version 330

uniform vec4 sDefColor;

varying vec3 N; /* Normalna */
varying vec3 v;

void main() {
	/** Diffuse light **/
	v = vec3(gl_ModelViewMatrix * gl_Vertex);
	N = normalize(gl_NormalMatrix * gl_Normal);
	
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	gl_FrontColor = sDefColor;
}