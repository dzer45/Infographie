#version 120

varying vec3 normal;
varying vec3 camera;

void main() {
	normal = gl_NormalMatrix * gl_Normal;
	camera = normalize(-(gl_ModelViewMatrix*gl_Vertex).xyz);
	gl_Position = ftransform();
	gl_FrontColor = gl_Color;
	
}
