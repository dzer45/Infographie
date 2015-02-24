#version 120

uniform vec4 ogf_uniform_0;
varying vec3 position;
varying float radius;

void main(void) {
	gl_FragColor = gl_Color;
	
	float dist = distance(position,gl_FragCoord.xyz); 
	if(dist > radius ) discard;	

	float z = sqrt(pow(radius,2)-pow(dist,2));
	gl_FragDepth = (1/z+position.z);	
	vec3 b = gl_FragCoord.xyz;
	b.z = z;
	
	vec3 ob = b - position;
	
	vec3 n = normalize(ob);
	vec3 l = normalize(gl_LightSource[0].position.xyz);

	float diffuse  = .2+clamp(dot(l,n), 0, 1);	
	vec3 r = normalize(2*n*dot(l,n)-l);

	float spec = pow(max(r.z,0.),5);
	gl_FragColor = gl_Color*diffuse+spec;
	
}

