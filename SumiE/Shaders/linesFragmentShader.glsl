#version 150

in vec3 n_eye;
in vec3 pos_eye;
in vec3 vs_normals; // zwvEcNormal
in vec3 vs_position; // vs_position
in vec3 vs_position_iso; // zwvEcVertex

out vec4 frag_color;

uniform vec3 lightPos, viewPos;
uniform float ambientStrength, specularStrength;

const vec3 ambientColor = vec3(0.0, 1.0, 0.0);
const vec3 diffuseColor = vec3(0.0, 1.0, 0.0);
const vec3 specColor = vec3(0.5, 0.5, 0.5);

void main()
{

	vec4 color;
	vec3 light_dir = normalize(lightPos - pos_eye);
	vec3 norm = normalize(vs_normals);
	vec3 viewDir = normalize(viewPos - pos_eye);

    if (dot(vs_normals, viewDir) <= ambientStrength ){
        color = vec4(0.0, 0.0, 0.0, 1.0);
    }else{
        color = vec4(1.0, 1.0, 1.0, 1.0);
    }
    frag_color = color;
}