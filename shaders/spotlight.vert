#version 120

uniform vec4 sDefColor;
uniform mat4 sModelViewMatrix;
uniform mat4 sProjectionMatrix;
uniform mat3 sNormalMatrix;

varying vec3 normal;
varying vec3 lightDir;
varying vec3 eyeVec;

void main() {
	normal = normalize(sNormalMatrix * gl_Normal);
	vec3 vVertex = vec3(sModelViewMatrix * gl_Vertex);
	
	lightDir = vec3((gl_LightSource[0].position * sModelViewMatrix).xyz - vVertex);
	eyeVec = -vVertex;
	
	gl_Position = sProjectionMatrix * sModelViewMatrix * gl_Vertex;
	gl_FrontColor = sDefColor;
}