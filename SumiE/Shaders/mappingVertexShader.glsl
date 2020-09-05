#version 400
in vec3 vertex_position;
in vec3 vertex_normals;
in vec2 texture_coordinate;
in vec3 tangent;
in vec3 bitangent;

out vec3 n_eye;
out vec3 pos_eye;
out vec2 a_texture_coordinate;
out mat3 TBN;
out vec3 lightdir_tangent;
out vec3 eyedir_tangent;

uniform mat4 model, ortho, proj, view;
uniform vec3 lightPos, viewPos;
uniform float ambientStrength, specularStrength;

void main(){
	pos_eye = vec3(view * model * vec4(vertex_position, 1.0));
	n_eye = vec3(view * model * vec4 (vertex_normals, 0.0));
	a_texture_coordinate = texture_coordinate;
	    // TBN = mat3(normalize(vec3(model * vec4(tangent, 0.0))), B, N);
	vec3 T = normalize(vec3(view * model * vec4(tangent, 0.0)));
    vec3 B = normalize(vec3(view * model * vec4(bitangent, 0.0)));
    vec3 N = normalize(vec3(view * model * vec4(vertex_normals, 0.0)));

	TBN = transpose(mat3(T, B, N));
	// TBN = transpose(mat3(T, B, N));
	gl_Position =  proj * view * model * ortho* vec4 (vertex_position, 1.0);
	// Eyedir tangentspace
	vec3 vertex_cameraspace = ( view * model * vec4(vertex_position, 1)).xyz;
	vec3 eyedir_cameraspace = vec3(0,0,0) - vertex_cameraspace;
	// Lightdir
	vec3 lightpos_cameraspace = ( view * vec4(lightPos, 1)).xyz;
	vec3 lightdir_cameraspace = lightpos_cameraspace + eyedir_cameraspace;
	// Lightdir/eyedir output
	lightdir_tangent = TBN * lightdir_cameraspace;
	eyedir_tangent =  TBN * eyedir_cameraspace;
}