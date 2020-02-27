#version 450


in vec3 final_normal;
in vec4 cam_space_position;
in vec2 final_texture_coordinates;

struct DirectionalLight
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec3 direction;
};

uniform sampler2D diffuse_texture;

uniform vec3 camera_position;

#define D_LIGHT_AMOUNT 2
uniform DirectionalLight directional_lights[D_LIGHT_AMOUNT];

uniform vec3 material_ambient;
uniform vec3 material_diffuse;
uniform vec3 material_specular;
uniform float material_specular_power;

//uniform vec3 light_direction;
//uniform vec3 light_ambient;
//uniform vec3 light_diffuse;
//uniform vec3 light_specular;


out vec4 final_color;


vec3 CalculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 view);

void main()
{
	//Store the total of all the lights' effect on this frag's colour
	vec3 result;

	//Make sure the normal is normalised
	vec3 normal = normalize(final_normal);
	//Calculate the view vector
	vec3 view = normalize(camera_position - cam_space_position.xyz);
	
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
	vec4 texture_color = texture(diffuse_texture, final_texture_coordinates);
	
	return (ambient + (diffuse * texture_color.xyz) + specular);
}