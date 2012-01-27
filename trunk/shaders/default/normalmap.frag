in vec3 sVaryingLightDir;
in vec3 sEyeVector;

void main () {
	float distSqr = dot(sVaryingLightDir, sVaryingLightDir);
	float att = clamp(sqrt(distSqr), 0.0, 1.0);
	vec3 lVec = sVaryingLightDir * inversesqrt(distSqr);

	vec3 vVec = normalize(sEyeVector);
	
	vec4 base = texture2D(textureUnit, sVaryingTexCoords.st);
	if (base == (1.0, 1.0, 1.0, 1.0))
		base.a = 0.0;
	
	vec3 bump = normalize(texture2D(normalMap, sVaryingTexCoords.st).xyz * 2.0 - 1.0);

	vec4 vAmbient = sLightSource[0].ambient * sFrontMaterial.ambient;

	float diffuse = max(dot(lVec, bump), 0.0 );
	
	vec4 vDiffuse = sLightSource[0].diffuse * sFrontMaterial.diffuse * 
			diffuse;
					
	vec4 specularAttrib = texture2D(specularMap, sVaryingTexCoords.st);
	if (specularAttrib == vec4(1.0, 1.0, 1.0, 1.0))
			specularAttrib = sFrontMaterial.specular;
			
	float reflectivity = 0.10 * specularAttrib.r +
			0.29 * specularAttrib.g +
			0.01 * specularAttrib.b;

	float specular = pow(clamp(dot(reflect(-lVec, bump), vVec), 0.0, 1.0), 
	                 sFrontMaterial.shininess) * reflectivity;
	                 
	vec4 vSpecular = sLightSource[0].specular * specular;
	
	sFragColor = (vAmbient * base + 
			vDiffuse * base + 
			vSpecular) * att;
}