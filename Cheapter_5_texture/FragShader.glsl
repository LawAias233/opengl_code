#version 430

in vec2 tc;

out vec4 color;

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;

layout (binding = 0) uniform sampler2D samp;
//uniform sampler2D samp

void main(void)
{ 
    //color = vec4(0.0, 0.0, 1.0, 1.0);
    color = texture(samp, tc) + vec4(vec3(tc, 0.0)*0.1, 1.0);
    //color = vec4(tc, 0.0, 1.0);
    //color = texture(samp, vec2(0.5, 0.5));
}