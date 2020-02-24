#version 450

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec2 texture_coordinates;

uniform mat4 projection_view_matrix;
uniform mat4 model_matrix;

out vec2 final_texture_coordinates;

void main()
{
	final_texture_coordinates = texture_coordinates;
	gl_Position = (projection_view_matrix * model_matrix) * vec4(vertex_position, 1);
}