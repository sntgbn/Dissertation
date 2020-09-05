#version 400

in vec3 n_eye;
in vec3 pos_eye;
in vec2 a_texture_coordinate;
in mat3 TBN;
in vec3 lightdir_tangent;
in vec3 eyedir_tangent;

uniform sampler2D texture;
uniform sampler2D normal;

uniform vec3 lightPos, viewPos;
uniform float ambientStrength, specularStrength;

out vec3 frag_colour;

void main()
{
    vec3 diffuse_texture_color = texture(texture, a_texture_coordinate).rgb;
    vec3 ambient_texture_color = vec3(0.1,0.1,0.1) * diffuse_texture_color;
    // Normal in tangent space
    vec3 texture_normal = normalize(texture( normal, vec2(a_texture_coordinate.x,-a_texture_coordinate.y)).rgb*2.0 - 1.0);
    // Distance to the light
	float light_distance = length( lightPos - n_eye ); 
    vec3 l = normalize(lightdir_tangent);
    float cosTheta = clamp( dot( texture_normal,l ), 0,1 );
    // Eye vector (towards the camera)
	vec3 E = normalize(eyedir_tangent);
    // Direction in which the triangle reflects the light
    vec3 R = reflect(-l, texture_normal);
    float cosAlpha = clamp( dot( E,R ), 0,1 );
    frag_colour = ambient_texture_color + diffuse_texture_color * 40.0 * cosTheta / (light_distance*light_distance);
    // frag_colour = texture(texture, a_texture_coordinate).rgb;
}