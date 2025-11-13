#version 460 core

#define MAX_LIGHTS  5
#define Point       0
#define Directional 1
#define Spot        2

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec2 a_texcoord;
layout (location = 2) in vec3 a_normal;

out VS_OUT {
    vec2 texcoord;
    vec3 color;
}vs_out;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

uniform vec3 u_ambient_light;
struct Light {
	vec3 position;
	vec3 color;
    float intensity;
    float range;
    int type;
};

uniform int u_numLights = 5;
uniform Light u_lights[5];
uniform sampler2D u_baseMap;
uniform sampler2D u_spcularMap;
uniform sampler2D u_emissiveMap;

uniform struct Material {
	vec3 baseColor;
    vec3 emissiveColor;

	float shininess;
	vec2 tiling;
	vec2 offset;
    uint parameters;
} u_material;

float calculateAttenuation(in float l_distance, in float range) {
    float attenuation = max(0 ,(1 - (l_distance / range)));

    return pow(attenuation, 2);
}

vec3 calculateLight(in Light light,in vec3 position, in vec3 normal) {
	// Diffuse lighting
	vec3 light_dir = normalize(light.position - position);
	float intensity = max(dot(light_dir, normal), 0);
	vec3 diffuse = light.color * u_material.baseColor * intensity;

	// specular
	vec3 reflection = reflect(-light_dir, normal);
	vec3 view_dir = normalize(-position);
	intensity = max(dot(reflection, view_dir), 0);
	intensity = pow(intensity, u_material.shininess);
	vec3 specular = vec3(intensity);

    float l_distance = length(light.position - position);
    float attenuation = calculateAttenuation(l_distance, light.range);

	return (diffuse + specular) * light.intensity * attenuation;
}

void main() {
	vs_out.texcoord = a_texcoord * u_material.tiling + u_material.offset;

	mat4 model_view = u_view * u_model;
	vec3 position = vec3(model_view * vec4(a_position, 1.0));
	vec3 normal =normalize( mat3(model_view) * a_normal);

    vs_out.color = u_ambient_light;
    for (int i = 0; i < u_numLights; i++) {
	    vs_out.color += calculateLight(u_lights[i],position, normal);
    }

	gl_Position = u_projection * u_view * u_model * vec4(a_position, 1.0);
}