#version 120

varying vec3 N;
varying vec3 v;

void main () {  
	vec3 L = normalize((sLightSource[0].position * sModelViewMatrix).xyz - v);
	vec3 E = normalize(-v);
	vec3 R = normalize(-reflect(L,N));

	//calculate Ambient Term:  
	vec4 Iamb = sFrontMaterial.ambient * sLightSource[0].ambient;

	//calculate Diffuse Term:  
	vec4 Idiff = sFrontMaterial.diffuse * sLightSource[0].diffuse * max(dot(N,L), 0.0);
	Idiff = clamp(Idiff, 0.0, 1.0);     
   
	// calculate Specular Term:
	vec4 Ispec = (sFrontMaterial.specular * sLightSource[0].specular)
			* pow(max(dot(R,E),0.0),0.3 * sFrontMaterial.shininess);
	Ispec = clamp(Ispec, 0.0, 1.0); 
	
	// write Total Color:
	gl_FragColor = ((sFrontMaterial.emission + sFrontMaterial.ambient * sLightModel.ambient) + Iamb + Idiff + Ispec) * sDefColor;
}