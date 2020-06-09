#version 330

in vec3 n_eye;
in vec3 pos_eye;
in vec3 vs_normals;
in vec3 vs_position; // vs_position
in vec3 vs_position_iso; // zwvEcVertex

out vec4 frag_color;

uniform vec3 lightPos, viewPos;
uniform float ambientStrength, specularStrength;

const vec3 ambientColor = vec3(0.0, 1.0, 0.0);
const vec3 diffuseColor = vec3(0.0, 1.0, 0.0);
const vec3 specColor = vec3(0.5, 0.5, 0.5);

void main() {
	// Global Lighting Variables
	vec4 color;
	vec3 light_dir = normalize(lightPos - pos_eye);
	vec3 norm = normalize(vs_normals);
	vec3 viewDir = normalize(viewPos - pos_eye);
	// Toon
	vec3 normal_dir = normalize(vs_normals);
	float unlitOutlineThickness = ambientStrength;
	float litOutlineThickness = specularStrength;
	vec3 outLineColor = vec3(0.0, 0.0, 0.0);
	vec3 lightColor = vec3(1.0, 1.0, 1.0);
	if (dot(viewDir, normal_dir) < mix(unlitOutlineThickness, litOutlineThickness, max(0.0, dot(normal_dir, light_dir)))) {
		color = vec4(vec3(outLineColor) * vec3(lightColor), 1.0);
	} else {
		float intensity;
		intensity = dot(light_dir, normalize(norm));
		if (intensity > 0.99) {
			color = vec4(1.0, 1.0, 1.0, 1.0);
		}
		else if (intensity > 0.95) {
			color = vec4(0.678, 1.0, 0.184, 1.0);
		}
		else if (intensity > 0.5) {
			color = vec4(0.196, 0.804, 0.196, 1.0);
		}
		else if (intensity > 0.25) {
			color = vec4(0.133, 0.545, 0.133, 1.0);
		}
		else if (intensity > 0.02) {
			color = vec4(0.000, 0.502, 0.0, 1.0);
		}
		else {
			color = vec4(0.0, 0.0, 0.0, 1.0);
		}
	}
	frag_color = color;
}