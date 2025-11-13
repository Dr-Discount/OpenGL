#version 460 core

#define BaseMap     (1 << 0)
#define SpecularMap (1 << 1)
#define EmissiveMap (1 << 2)

in VS_OUT {
    vec2 texcoord;
    vec3 color;
} fs_in;

out vec4 f_color;

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


void main() {
     vec4 emissive = ((u_material.parameters & EmissiveMap) != 0u)
  ? texture(u_emissiveMap, fs_in.texcoord) * vec4(u_material.emissiveColor, 1)
  : vec4(u_material.emissiveColor, 1);

	f_color = texture(u_baseMap, fs_in.texcoord) * vec4(fs_in.color, 1) + emissive;
}