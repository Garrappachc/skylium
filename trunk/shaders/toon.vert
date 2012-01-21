#version 330

out vec3 normal;

void main() {
	normal = sNormalMatrix * sNormal;

	gl_Position = sModelViewProjectionMatrix * sVertex;
}