#version 450

in vec3 final_normal;

uniform vec3 directional_light;

out vec4 final_color;

void main()
{
	//Make sure the directions are normalised
	vec3 N = normalize(final_normal);
	vec3 L = normalize(directional_light);
	
	//Calculate the similarity between the two directions, clamp between 0-1
	float diffuseTerm = max(0, min(1, dot(N, -L)));
	
	final_color = vec4(diffuseTerm, diffuseTerm, diffuseTerm, 1);
}