out vec3 sVaryingLightDir;
out vec3 sEyeVector;
out float sAttenuation;

void main() {
	sVaryingTexCoords = sTexCoords;
	
	vec3 tangent;
	vec3 c1 = cross(sNormal, vec3(0.0, 0.0, 1.0)); 
	vec3 c2 = cross(sNormal, vec3(0.0, 1.0, 0.0)); 
	
	if (length(c1) > length(c2)) {
		tangent = normalize(c1);	
	} else {
		tangent = normalize(c2);
	}
	
	vec3 n = normalize(sNormalMatrix * sNormal);
	vec3 t = normalize(sNormalMatrix * tangent);
	vec3 b = cross(n, t);
	
	vec4 pos4 = sModelViewMatrix * sVertex;
	vec3 pos3 = pos4.xyz / pos4.w;
	
	mat3 rotmat = mat3(t, b, n);
	
	vec3 lightVec = sLightSource[0].position.xyz - pos3;
	sVaryingLightDir = rotmat * lightVec;

	vec3 vVertex = pos4.xyz;
	vec3 tmpVec = -vVertex;
	sEyeVector.x = dot(tmpVec, t);
	sEyeVector.y = dot(tmpVec, b);
	sEyeVector.z = dot(tmpVec, n);
	
	float d = length(sVaryingLightDir);
	
	sAttenuation = 1.0 / ( sLightSource[0].constantAttenuation + 
	(sLightSource[0].linearAttenuation*d) + 
	(sLightSource[0].quadraticAttenuation*d*d) );
	
	gl_Position = sModelViewProjectionMatrix * sVertex;
}