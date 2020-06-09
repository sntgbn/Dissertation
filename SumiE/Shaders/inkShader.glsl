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

uniform vec3 lightPos, viewPos;
uniform float ambientStrength, specularStrength;
	// Turn to uniforms later
uniform float lit_outline_thickness, unlit_outline_thickness, wobble_distortion, paper_roughness, diffuse_factor, dry_brush_granulation, dry_brush_density; // Make this uniform variable (?)

const vec3 ambientColor = vec3(0.0, 1.0, 0.0);
const vec3 diffuseColor = vec3(1.0, 1.0, 1.0);
const vec3 specColor = vec3(0.5, 0.5, 0.5);
const vec2 resolution  = vec2(800, 600);


int edge_detect() 
{
	// Get current pixel
	vec2 position = ( gl_FragCoord.xy / resolution.xy );
	vec4 mapped_pixel = texture2D(texture_map, position);
	vec4 edge_pixel;
    for(int y = -1; y <= 1; y++){
		for(int x = -1; x <= 1; x++){
			if(x == 0 && y == 0){
				// do nothing
			}else{
				edge_pixel = texture2D(texture_map, vec2(position.x+x, position.y+y));
				if(edge_pixel != mapped_pixel){
					return 1; // edge
				}
			}
		}
	}
	return 0; // not edge
}

float luma(vec3 color) {
  return dot(color, vec3(0.299, 0.587, 0.114));
}

float luma(vec4 color) {
  return dot(color.rgb, vec3(0.299, 0.587, 0.114));
}


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
	int is_edge;

	if (dot(viewDir, normal_dir) < mix(unlit_outline_thickness, lit_outline_thickness, max(0.0, dot(normal_dir, light_dir)))) {
		color = vec4(vec3(outLineColor) * vec3(lightColor), 1.0);
	} else {
		float intensity;
		intensity = dot(light_dir, normalize(norm));
		if(intensity > 0.95){
			color = vec4(0.643, 0.713, 0.819, 1.0);
		}else if (intensity > 0.75) {
			color = vec4(0.596, 0.674, 0.792, 1.0);
		}
		else if (intensity > 0.50) {
			color = vec4(0.533, 0.615, 0.756, 1.0);
		}
		else if (intensity > 0.25) {
			color = vec4(0.384, 0.482, 0.650, 1.0);
		}
		else if (intensity > 0.10) {
			color = vec4(0.274, 0.372, 0.564, 1.0);
		}
		else if (intensity > 0.02) {
			color = vec4(0.223, 0.317, 0.513, 1.0);
		}
		else {
			color = vec4(0.0, 0.0, 0.0, 1.0);
		}

		// Dry Brush
		float height_scaling = 2.0f;
		float height_factor = ((texture(height_map, a_texture_coordinate*wobble_distortion).r - 0.5)*height_scaling) + 0.5;
		float granulation_density = abs(dry_brush_granulation) + 0.2;
		float darkening;
		float height_inverse;
		float full_darkening; 
		float application_parameter = dry_brush_granulation;
		// Darkening
		if(height_factor < application_parameter){ 
			color = abs(height_factor-application_parameter)*color + color;
		// Lightening
		}else{
			darkening = granulation_density*(1 - luma(color)) + (granulation_density * 5)*luma(color);
			height_inverse = 1 - ((height_factor*0.2)+0.8);
			full_darkening = 1 + darkening * height_inverse * dry_brush_density;
			color = color * darkening * dry_brush_density;
			color = vec4(pow(color.x,  full_darkening), pow(color.y,  full_darkening), pow(color.z,  full_darkening), 1.0);

		}

		// Texture Wobble 
		vec3 wobbled_sampled_texture_map;
		wobbled_sampled_texture_map = texture(texture_map, a_texture_coordinate*wobble_distortion).rgb;
		// Normal map diffusion, should this be wobbled as well?
		//vec3 sampled_normal_map = texture(normal_map, a_texture_coordinate).rgb;
		// float normal_map_diffusion = max(dot(light_dir, sampled_normal_map), 0.0);

		vec3 sampled_normal_map = normalize(texture( normal_map, a_texture_coordinate*wobble_distortion).rgb);
		// substrate lighting
		float Id = 1 - (1 - dot(light_dir, paper_roughness * sampled_normal_map)*diffuse_factor);

		// color = substrate lighting * darkening/lighting color (dry brush) * cel shader color * wobbled texture
		color = Id * color * vec4(vec3(wobbled_sampled_texture_map), 1.0);
	}
	frag_color = color;
}