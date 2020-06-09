#version 400

//layout(location = 0) in vec3 vertexPosition;
//layout(location = 1) in vec3 vertexNormals;
in vec3 vertex_position;
in vec3 vertex_normals;
in vec2 texture_coordinate;

out vec3 n_eye;
out vec3 pos_eye; // fragPos
out vec3 vs_normals; // 
out vec3 vs_position;
out vec2 a_texture_coordinate;

uniform mat4 model, ortho, proj, view;
uniform vec3 lightPos, viewPos;
uniform float ambientStrength, specularStrength;

void main(){
	pos_eye = vec3(model * vec4(vertex_position, 1.0));
	n_eye = vec3(view * vec4 (vertex_normals, 0.0));
	vs_normals = normalize(mat3(model) * vertex_normals);
	vs_position = mat3(model) * vertex_position;
	a_texture_coordinate = texture_coordinate;
	gl_Position =  proj * model * view * ortho* vec4 (vertex_position, 1.0);
}