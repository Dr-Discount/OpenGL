#version 460 core

in vec3 v_color;
out vec4 fcolor;

uniform float u_time;

void main() {
	//float offset = sin(u_time + gl_FragCoord.y);
	fcolor = vec4(v_color, 1);
}