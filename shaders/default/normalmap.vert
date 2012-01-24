out vec3 v;

void main() {
	v = vec3(sModelViewMatrix * sVertex);
	
	sVaryingTexCoords = sTexCoords;

	gl_Position = sModelViewProjectionMatrix * sVertex;
}