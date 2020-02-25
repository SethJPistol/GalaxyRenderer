#version 450

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 normal;

uniform mat4 projection_view_matrix;
uniform mat4 model_matrix;
uniform mat3 normal_matrix;

out vec3 final_normal;
out vec4 cam_space_position;

void main()
{
	final_normal = normal_matrix * normal.xyz;
	
	cam_space_position = model_matrix * vec4(vertex_position, 1);
	gl_Position = projection_view_matrix * cam_space_position;
}