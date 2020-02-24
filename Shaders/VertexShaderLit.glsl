#version 450

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 normal;

uniform mat4 projection_view_matrix;
uniform mat4 model_matrix;
uniform mat3 normal_matrix;

out vec3 final_normal;

void main()
{
	final_normal = normal_matrix * normal.xyz;
	gl_Position = (projection_view_matrix * model_matrix) * vec4(vertex_position, 1);
}