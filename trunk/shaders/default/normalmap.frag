in vec3 sVaryingLightDir;
in vec3 sEyeVector;
in float sAttenuation;

void main () {
	vec3 normal = normalize(texture2D(normalMap, sVaryingTexCoords.st).xyz * 2.0 - 1.0);
	sFragColor = ((sFrontMaterial.emission + sFrontMaterial.ambient * sLightModel.ambient)
			* sFrontMaterial.ambient) + (sLightSource[0].ambient * sFrontMaterial.ambient) * sAttenuation;
	
	vec3 N = normalize(normal);
	vec3 L = normalize(sVaryingLightDir);
	
	float lambertTerm = dot(N,L);
	
	if(lambertTerm > 0.0) {
		sFragColor += sLightSource[0].diffuse *
				sFrontMaterial.diffuse *
				lambertTerm * sAttenuation;
		
		vec3 E = normalize(sEyeVector);
		vec3 R = reflect(-L, N);
		
		float specular = pow(max(dot(R, E), 0.0),
				sFrontMaterial.shininess );
		
		sFragColor += sLightSource[0].specular *
				sFrontMaterial.specular * specular * sAttenuation;
	}

	sFragColor *= texture2D(textureUnit, sVaryingTexCoords.st);
}