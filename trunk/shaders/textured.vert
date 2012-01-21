#version 330

/* Textured shading */

out vec3 N;
out vec3 v;

void main() {
	v = vec3(sModelViewMatrix * sVertex);
	N = normalize(sNormalMatrix * sNormal);
	
	sVaryingTexCoords = sTexCoords;

	gl_Position = sModelViewProjectionMatrix * sVertex;
}