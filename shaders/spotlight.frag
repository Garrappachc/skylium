#version 120

uniform vec4 sDefColor;
uniform mat4 sModelViewMatrix;
uniform mat4 sProjectionMatrix;
uniform mat3 sNormalMatrix;

varying vec3 normal;
varying vec3 lightDir;
varying vec3 eyeVec;

void main() {
	vec4 finalColor =
			(gl_FrontLightModelProduct.sceneColor * gl_FrontMaterial.ambient) +
			(gl_LightSource[0].ambient * gl_FrontMaterial.ambient);
	
	vec3 L = normalize(lightDir);
	vec3 D = normalize(gl_LightSource[0].spotDirection);
	
	if (dot(-L, D) > gl_LightSource[0].spotCosCutoff) {
		vec3 N = normalize(normal);
		
		float lambertTerm = max(dot(N, L), 0.0);
		if (lambertTerm > 0.0) {
			finalColor += gl_LightSource[0].diffuse *
					gl_FrontMaterial.diffuse *
					lambertTerm;
					
			vec3 E = normalize(eyeVec);
			vec3 R = reflect(-L, N);
			
			float specular = pow(max(dot(R, E), 0.0), gl_FrontMaterial.shininess);
			
			finalColor += gl_LightSource[0].specular *
					gl_FrontMaterial.specular *
					specular;
		}
	}
	
	gl_FragColor = finalColor * sDefColor;
}
