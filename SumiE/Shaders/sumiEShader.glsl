#version 330

in vec3 n_eye;
in vec3 pos_eye;
in vec3 vs_normals;
in vec3 vs_position; // vs_position
in vec3 vs_position_iso; // zwvEcVertex
in vec2 a_texture_coordinate;

uniform sampler2D texture_map;
uniform sampler2D normal_map;
uniform sampler2D height_map;


out vec4 frag_color;

uniform mat4 model, ortho, proj, view;
uniform vec3 lightPos, viewPos;
uniform float ambientStrength, specularStrength;
	// Turn to uniforms later
uniform float lit_outline_thickness, unlit_outline_thickness, wobble_distortion, paper_roughness, diffuse_factor, dry_brush_granulation, dry_brush_density; // Make this uniform variable (?)

const vec3 ambientColor = vec3(0.0, 1.0, 0.0);
const vec3 diffuseColor = vec3(1.0, 1.0, 1.0);
const vec3 specColor = vec3(0.5, 0.5, 0.5);
const vec2 resolution  = vec2(800, 600);

void main() {
	// Global Lighting Variables
	vec4 color;
	vec3 light_dir = normalize(lightPos - pos_eye);
	vec3 norm = normalize(vs_normals);
	vec3 viewDir = normalize(viewPos - pos_eye);
	// Toon
	vec3 normal_dir = normalize(vs_normals);
	vec3 outLineColor = vec3(0.0, 0.0, 0.0);
	vec3 lightColor = vec3(1.0, 1.0, 1.0);
	// Sumi-E Edges
	vec3 vee = viewPos - vs_position;
	vec3 vee_norm = normalize(vee);
	vec3 norm_view = (view * vec4(vs_normals, 1.0)).xyz; 
	vec3 r = 2*(dot(norm_view, vee_norm))*(norm_view-vee_norm);
	float m = 2 * sqrt(pow(r.x, 2)+pow(r.y, 2) + pow(r.z+1,2));
	float tu = r.x/m + 1/2;
	float tv = r.y/m + 1/2;

	if (dot(viewDir, normal_dir) < mix(unlit_outline_thickness, lit_outline_thickness, max(0.0, dot(normal_dir, light_dir)))) {
		// color = vec4(vec3(outLineColor) * vec3(lightColor), 1.0);
		color = texture(texture_map, vec2(tu, tv)*wobble_distortion);
	} else {
		color = vec4(vec3(1.0, 1.0, 1.0), 1.0);
	}
	frag_color = color;
}