#version 450

layout(location = 0) in vec2 vertex_position;

out vec2 final_texture_coordinates;

void main()
{
	//Calculate UVs by converting the clip-space positions to texture-space coords [0,1]
	final_texture_coordinates = vertex_position * 0.5f + 0.5f;
	
	gl_Position = vec4(vertex_position, 0, 1);
}