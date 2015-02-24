#version 120
uniform vec4 ogf_uniform_0; // viewport
attribute float  attr;
varying vec3 position;
varying float radius;


void main() {
	gl_PointSize  = 550*attr;
	radius = gl_PointSize/2;
	gl_FrontColor = gl_Color;
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	vec3 pos = (gl_Position.xyz/gl_Position.w);

	position.x = ogf_uniform_0.z/2*pos.x + (ogf_uniform_0.x+ogf_uniform_0.z/2);
	position.y = ogf_uniform_0.w/2*pos.y + (ogf_uniform_0.y+ogf_uniform_0.w/2);
	position.z = (gl_DepthRange.far-gl_DepthRange.near)/2*pos.z + (gl_DepthRange.far+gl_DepthRange.near)/2;
}

