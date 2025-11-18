#version 430

in layout(location = 0) vec3 a_position;
in layout(location = 2) vec3 a_normal;

out vec3 v_texcoord;

uniform float u_ior = 1.3;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

void main()
{
	// transform vertex position/normal to world space
	vec3 position = vec3(u_model * vec4(a_position, 1));
	vec3 normal = normalize(mat3(u_model) * a_normal);

	// inverse of view space -> world space
	// last column of mat4 is position
	vec3 viewPosition = inverse(u_view)[3].xyz;
	vec3 viewDir = normalize(position - viewPosition);

	// refract view direction about vertex normal using index of refraction
	v_texcoord = refract(viewDir, normal, 1.0 / u_ior);	

	gl_Position = u_projection * u_view * u_model * vec4(a_position, 1.0);
}