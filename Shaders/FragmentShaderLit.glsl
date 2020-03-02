#version 450


in vec3 final_normal;
in vec4 cam_space_position;
in vec2 final_texture_coordinates;
in vec3 final_tangent;
in vec3 final_bitangent;

struct DirectionalLight
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec3 direction;
};

uniform sampler2D diffuse_texture;
uniform sampler2D normal_texture;

uniform vec3 camera_position;

#define D_LIGHT_AMOUNT 2
uniform DirectionalLight directional_lights[D_LIGHT_AMOUNT];

uniform vec3 material_ambient;
uniform vec3 material_diffuse;
uniform vec3 material_specular;
uniform float material_specular_power;


out vec4 final_color;


vec3 CalculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 view);

void main()
{
	//Store the total of all the lights' effect on this frag's colour
	vec3 result;

	//Make sure the vectors are normalised
	vec3 normal = normalize(final_normal);
	vec3 tangent = normalize(final_tangent);
	vec3 bitangent = normalize(final_bitangent);
	
	//Calculate the view vector
	vec3 view = normalize(camera_position - cam_space_position.xyz);
	
	//Create the Tangent Basis Matrix
	mat3 TBN = mat3(tangent, bitangent, normal);
	//Get the normal map value at the current texture coordinate
	vec3 texture_normal = texture(normal_texture, final_texture_coordinates).xyz;
	//Recalculate the normal using the normal map
	normal = TBN * (texture_normal * 2 - 1);
	
	//For all the directional lights in the scene, calculate their effect
	for (int i = 0; i < D_LIGHT_AMOUNT; ++i)
		result += CalculateDirectionalLight(directional_lights[i], normal, view);
	
	final_color = vec4(result, 1);
}

vec3 CalculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 view)
{
	//Make sure the light direction is normalised
	vec3 light_direction = normalize(light.direction);
	
	//Calculate the similarity between the two directions, clamp between 0-1
	float diffuse_term = max(0, min(1, dot(normal, -light_direction)));
	
	//Calculate the reflection vector
	vec3 reflection = reflect(light_direction, normal);
	
	//Calculate the specular term
	float specular_term = pow(max(0, dot(reflection, view)), material_specular_power);
	
	//Calculate each colour property
	vec3 ambient = material_ambient * light.ambient;
	vec3 diffuse = material_diffuse * light.diffuse * diffuse_term;
	vec3 specular = material_specular * light.specular * specular_term;
	
	//Get the texture colour at the current texture coordinate
	vec4 texture_diffuse = texture(diffuse_texture, final_texture_coordinates);
	
	return (ambient + (diffuse * texture_diffuse.xyz) + specular);
}