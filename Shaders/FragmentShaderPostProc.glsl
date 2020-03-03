#version 450

in vec2 final_texture_coordinates;

uniform sampler2D diffuse_texture;

uniform int effect_to_use = 0;
uniform float chromatic_aberration_amount = 1.0f;

out vec4 final_color;

//Different effects to call
vec4 Default(vec2 tex_coord);
vec4 Tint(vec2 tex_coord, vec3 color_tint);
vec4 Sepia(vec2 tex_coord);
vec4 ChromaticAberration(vec2 tex_coord, vec2 texel_size);
vec4 ChromaticAberrationCentered(vec2 tex_coord, vec2 texel_size);

void main()
{
	//Calculate the resolution of the image
	vec2 texture_size = textureSize(diffuse_texture, 0);
	//Calculate texel size
	vec2 texel_size = 1.0f / texture_size;
	
	//Scale the UVs down to fit into a -texel to texel range
	vec2 scale = (texture_size - texel_size) / texture_size;
	vec2 tex_coord = final_texture_coordinates / scale + texel_size * 0.5f;
	
	//Check which effect to use and apply
	vec4 result;
	if (effect_to_use == 0)
		result = Default(tex_coord);
	if (effect_to_use == 1)
		result = Tint(tex_coord, vec3(0.9f, 0.7f, 0.7f));
	if (effect_to_use == 2)
		result = Sepia(tex_coord);
	if (effect_to_use == 3)
		result = ChromaticAberration(tex_coord, texel_size);
	if (effect_to_use == 4)
		result = ChromaticAberrationCentered(tex_coord, texel_size);
	
	final_color = result;
}

vec4 Default(vec2 tex_coord)
{
	return texture(diffuse_texture, tex_coord);
}
vec4 Tint(vec2 tex_coord, vec3 color_tint)
{
	return (texture(diffuse_texture, tex_coord) * vec4(color_tint, 1));
}
vec4 Sepia(vec2 tex_coord)
{
	vec4 output_color;
	vec4 texture_color = texture(diffuse_texture, tex_coord);
	output_color.r = (texture_color.r * 0.393f) + (texture_color.g * 0.769f) + (texture_color.b * 0.189f);
	output_color.g = (texture_color.r * 0.349f) + (texture_color.g * 0.686f) + (texture_color.b * 0.168f);
	output_color.b = (texture_color.r * 0.272f) + (texture_color.g * 0.534f) + (texture_color.b * 0.131f);
	return output_color;
}
vec4 ChromaticAberration(vec2 tex_coord, vec2 texel_size)
{
	vec4 output_color;
	
	//Offset where each colour will be sampled, clamp to stop wrapping
	vec2 r_offset = tex_coord;
	vec2 g_offset = vec2(clamp(tex_coord.x - texel_size.x * 2.5f * chromatic_aberration_amount, texel_size.x * 0.5f, 1.0f), tex_coord.y);
	vec2 b_offset = vec2(clamp(tex_coord.x - texel_size.x * 5.0f * chromatic_aberration_amount, texel_size.x * 0.5f, 1.0f), tex_coord.y);
	
	//Sample the colours at the different offsets
	output_color.r = texture2D(diffuse_texture, r_offset).r;
	output_color.g = texture2D(diffuse_texture, g_offset).g;
	output_color.b = texture2D(diffuse_texture, b_offset).b;
	
	return output_color;
}
vec4 ChromaticAberrationCentered(vec2 tex_coord, vec2 texel_size)
{
	vec4 output_color;
	
	//Offset where each colour will be sampled
	vec2 r_offset = tex_coord;
	vec2 g_offset = tex_coord + ((tex_coord - vec2(0.5f, 0.5f)) * texel_size * 10.0f * chromatic_aberration_amount);
	vec2 b_offset = tex_coord + ((tex_coord - vec2(0.5f, 0.5f)) * texel_size * 20.0f * chromatic_aberration_amount);
	
	//Clamp to stop wrapping
	r_offset.x = clamp(r_offset.x, texel_size.x * 0.5f, 1.0f - texel_size.x * 0.5f);
	r_offset.y = clamp(r_offset.y, texel_size.y * 0.5f, 1.0f - texel_size.y * 0.5f);
	g_offset.x = clamp(g_offset.x, texel_size.x * 0.5f, 1.0f - texel_size.x * 0.5f);
	g_offset.y = clamp(g_offset.y, texel_size.y * 0.5f, 1.0f - texel_size.y * 0.5f);
	b_offset.x = clamp(b_offset.x, texel_size.x * 0.5f, 1.0f - texel_size.x * 0.5f);
	b_offset.y = clamp(b_offset.y, texel_size.y * 0.5f, 1.0f - texel_size.y * 0.5f);
	
	//Sample the colours at the different offsets
	output_color.r = texture2D(diffuse_texture, r_offset).r;
	output_color.g = texture2D(diffuse_texture, g_offset).g;
	output_color.b = texture2D(diffuse_texture, b_offset).b;
	
	return output_color;
}