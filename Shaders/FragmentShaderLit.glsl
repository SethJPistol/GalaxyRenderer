#version 450


in vec3 final_normal;
in vec4 cam_space_position;
in vec2 final_texture_coordinates;


uniform sampler2D diffuse_texture;

uniform vec3 camera_position;

uniform vec3 material_ambient;
uniform vec3 material_diffuse;
uniform vec3 material_specular;
uniform float material_specular_power;

uniform vec3 light_direction;
uniform vec3 light_ambient;
uniform vec3 light_diffuse;
uniform vec3 light_specular;


out vec4 final_color;


void main()
{
	//Make sure the directions are normalised
	vec3 N = normalize(final_normal);
	vec3 L = normalize(light_direction);
	
	//Calculate the similarity between the two directions, clamp between 0-1
	float diffuse_term = max(0, min(1, dot(N, -L)));
	
	//Calculate the view and reflection vectors
	vec3 V = normalize(camera_position - cam_space_position.xyz);
	vec3 R = reflect(L, N);
	
	//Calculate
	float specular_term = pow(max(0, dot(R, V)), material_specular_power);
	
	//Calculate each colour property
	vec3 ambient = material_ambient * light_ambient;
	vec3 diffuse = material_diffuse * light_diffuse * diffuse_term;
	vec3 specular = material_specular * light_specular * specular_term;
	
	//Get the texture colour
	vec4 texture_color = texture(diffuse_texture, final_texture_coordinates);
	
	final_color = vec4(ambient + (diffuse * texture_color.xyz) + specular, 1);
}