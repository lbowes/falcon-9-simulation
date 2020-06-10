$input a_position, a_color0
$output v_color0

#include <bgfx_shader.sh>

uniform vec4 u_uniform;

void main()
{
    gl_Position = mul(u_modelViewProj, vec4(a_position, 1.0) );
	
	// (doesn't work as expected because a_color0 is a uint32_t, not vec4) 
    v_color0 = a_color0 * u_uniform;
}
