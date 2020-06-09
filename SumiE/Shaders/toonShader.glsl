#version 330

in vec3 fragPos;
in vec3 nEye;
in vec3 vsNormals;
in vec3 vsLightPos;
in vec3 vsPosition;

out vec4 fragColour;

uniform vec3 lightPos, viewPos;
uniform float ambientStrength, specularStrength;

uniform sampler2D texture;
uniform sampler2D normal;

const vec3 ambientColor = vec3(0.0, 1.0, 0.0);
const vec3 diffuseColor = vec3(0.0, 1.0, 0.0);
const vec3 specColor = vec3(1.0, 1.0, 1.0);

void main() {
	// Global Lighting Variables
	vec4 color;
	vec3 lightDir = normalize(lightPos - fragPos);
	vec3 norm = normalize(vsNormals);
	vec3 viewDir = normalize(viewPos - fragPos);
	// Toon
	vec3 normalDir = normalize(vsNormals);
	// Passing uniform variables
	float litOutlineThickness = ambientStrength;
	float unlitOutlineThickness = specularStrength;
	vec3 outLineColor = vec3(0.0, 0.0, 0.0);
	vec3 lightColor = vec3(1.0, 1.0, 1.0);
	if (dot(viewDir, normalDir) < mix(unlitOutlineThickness, litOutlineThickness, max(0.0, dot(normalDir, lightDir)))) {
		color = vec4(vec3(outLineColor) * vec3(lightColor), 1.0);
	} else {
		float intensity;
		intensity = dot(lightDir, normalize(norm));
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
	fragColour = color;
}