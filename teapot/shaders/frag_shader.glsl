#version 120

varying vec3 normal;
varying vec3 camera;

void main(void) {
	vec3 n = normalize(normal);
	vec3 l = normalize(gl_LightSource[0].position.xyz);

	float diffuse  = .2+clamp(dot(l,n), 0, 1);	
	vec3 r = normalize(2*n*dot(l,n)-l);

	float spec = pow(max(r.z,0.),5);
	
	gl_FragColor = gl_Color*diffuse+spec;
	
	return;
}

