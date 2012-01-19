#version 120

varying vec3 N;
varying vec3 v;

void main() {
	v = vec3(sModelViewMatrix * gl_Vertex);       
	N = normalize(sNormalMatrix * gl_Normal);

	gl_Position = sProjectionMatrix * sModelViewMatrix * gl_Vertex;  
}
