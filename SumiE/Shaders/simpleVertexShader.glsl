#version 330

//layout(location = 0) in vec3 vertexPosition;
//layout(location = 1) in vec3 vertexNormals;
in vec3 vertexPosition;
in vec3 vertexNormals;

out vec3 nEye;
out vec3 fragPos;
out vec3 vsNormals;
out vec3 vsPosition;

uniform mat4 model, ortho, proj, view;
uniform vec3 lightPos, viewPos;
uniform float ambientStrength, specularStrength;

void main(){
	fragPos = vec3(model * vec4(vertexPosition, 1.0));
	nEye = (view * vec4 (vertexNormals, 0.0)).xyz;
	//vsNormals = vertexNormals;
	vsNormals = normalize(mat3(model) * vertexNormals);
	//vsPosition = vertexPosition;
	vsPosition = mat3(model) * vertexPosition;
	gl_Position =  proj * view * model * ortho* vec4 (vertexPosition, 1.0);
}