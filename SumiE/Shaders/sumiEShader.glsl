#version 330

in vec3 n_eye;
in vec3 pos_eye;
in vec3 vs_normals;
in vec3 vs_position; // vs_position
in vec3 vs_position_iso; // zwvEcVertex
in vec2 a_texture_coordinate;

uniform sampler2D brush_texture_map;
uniform sampler2D mesh_texture_map;
uniform sampler2D tone_texture_map;

uniform samplerCube cube_texture;


out vec4 frag_color;

uniform mat4 model, ortho, proj, view;
uniform vec3 lightPos, viewPos;
uniform float ambientStrength, specularStrength;
	// Turn to uniforms later
uniform float lit_outline_thickness, unlit_outline_thickness, solid_outline_color, wobble_distortion, texture_luminance, paper_alpha_threshold, paper_alpha_div;
uniform bool outline_selection, texture_selection, cel_shader_selection;
// Rename/delete these
uniform float diffuse_factor, dry_brush_granulation, dry_brush_density; // Make this uniform variable (?)

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
	float intensity;
	intensity = dot(light_dir, normalize(norm));
	// Sumi-E Edges
	vec3 vee = viewPos - vs_position;
	vec3 vee_norm = normalize(vee);
	vec3 norm_view = (view * vec4(vs_normals, 1.0)).xyz; 
	vec3 r = 2*(dot(norm_view, vee_norm))*(norm_view-vee_norm);
	float m = 2 * sqrt(pow(r.x, 2)+pow(r.y, 2) + pow(r.z+1,2));
	float tu = r.x/m + 1/2;
	float tv = r.y/m + 1/2;


	// if (dot(viewDir, normal_dir) < 1.0) {
	if (dot(viewDir, normal_dir) < mix(unlit_outline_thickness, lit_outline_thickness, max(0.0, dot(normal_dir, light_dir)))) {
		if(outline_selection == true){
			color = clamp(texture(brush_texture_map, vec2(tu, tv) + vec2(tu, tv)*wobble_distortion), 0, 1);
		}else{
			color = vec4(solid_outline_color, solid_outline_color, solid_outline_color, 1.0);
		}
		
	} else {
		if(cel_shader_selection == true){
			// Main color W
			if (intensity > 0.6) {
				color = vec4(1.0, 1.0, 1.0, 1.0);
			}
			// Transition from secondary colors 3
			// Secondary color LG-W2
			else if (intensity > 0.525) {
				color = vec4(0.89, 0.89, 0.89, 1.0);
			}
			// Secondary color LG-W1
			else if (intensity > 0.475) {
				color = vec4(0.8, 0.8, 0.8, 1.0);
			}
			// Main color LG
			else if (intensity > 0.4) {
				color = vec4(0.737, 0.737, 0.737, 1.0);
			}
			// Transition from secondary colors 2
			// Secondary color DG-LG2
			else if (intensity > 0.325) {
				color = vec4(0.6235, 0.6235, 0.6235, 1.0);
			}
			// Secondary color DG-LG1
			else if (intensity > 0.275) {
				color = vec4(0.43, 0.43, 0.43, 1.0);
			}
			// Main color DG
			else if (intensity > 0.2) {
				color = vec4(0.321, 0.321, 0.321, 1.0);
			}
			// Transition from secondary colors 1
			// Secondary color B-DG2
			else if (intensity > 0.125) {
				color = vec4(0.2313, 0.2313, 0.2313, 1.0);
			}
			// Secondary color B-DG1
			else if (intensity > 0.075) {
				color = vec4(0.098, 0.098, 0.098, 1.0);
			}
			// Main color B
			else {
				color = vec4(0.0, 0.0, 0.0, 1.0);
			}
		}else{
			vec3 diffuse_texture_color = texture(mesh_texture_map, a_texture_coordinate).rgb;
			float tone_texture_u = min(intensity*(0.3*diffuse_texture_color.r + 
											 	 0.59*diffuse_texture_color.g +
											 	 0.11*diffuse_texture_color.b),
									   1);
			color = texture(tone_texture_map, vec2(tone_texture_u, 0.5));
		}
	}
	if(texture_selection==true){
		// Normal Mapping Part
		vec3 diffuse_texture_color = texture(mesh_texture_map, a_texture_coordinate).rgb;
		// Limunance
		float diffuse_texture_bw = texture_luminance*dot(diffuse_texture_color, vec3(0.2126729, 0.7151522, 0.0721750));

		// cel shader color * diffuse texture BW color
		color = color*vec4(diffuse_texture_bw, diffuse_texture_bw, diffuse_texture_bw, 1.0);
	}

	// Average color used to obtain alpha value
	float avg_color = clamp((color.x + color.y + color.z)/3, 0.0, 1.0);
	if(avg_color <= paper_alpha_threshold){
		color.a = clamp((1 - avg_color), 0.0, 1.0);
	} 

	// Threshold values
	if(color.a < paper_alpha_div){
		discard;
	}
	frag_color = color;
}