#version 150

uniform vec4 sDefColor;
uniform mat4 sModelViewMatrix;
uniform mat4 sProjectionMatrix;

varying vec3 N; /* Normal vector */
varying vec3 v;

void main() {
	/** Diffuse light **/
	v = vec3(sModelViewMatrix * gl_Vertex);
	N = normalize(gl_NormalMatrix * gl_Normal);
	
	gl_Position = sProjectionMatrix * sModelViewMatrix * gl_Vertex;
	gl_FrontColor = sDefColor;
}