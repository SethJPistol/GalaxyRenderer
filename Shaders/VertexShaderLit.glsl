#version 450

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texture_coordinates;

uniform mat4 projection_view_matrix;
uniform mat4 model_matrix;
uniform mat3 normal_matrix;

out vec3 final_normal;
out vec4 cam_space_position;
out vec2 final_texture_coordinates;

void main()
{
	//Flip the UVs vertically so that the top left is 0,0
	final_texture_coordinates = texture_coordinates; //* vec2(1, -1);

	final_normal = normal_matrix * normal.xyz;
	
	cam_space_position = model_matrix * vec4(vertex_position, 1);
	gl_Position = projection_view_matrix * cam_space_position;
}