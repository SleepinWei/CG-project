#version 330 
out vec3 pos;
out vec3 fsun;
uniform mat4 P;
uniform mat4 V;
uniform float time = 0.0;

const vec2 data[4] = vec2[](
vec2(-1.0,  1.0), vec2(-1.0, -1.0),
vec2( 1.0,  1.0), vec2( 1.0, -1.0));

void main()
{
    float depth = 0.0;
    gl_Position = vec4(data[gl_VertexID], depth, 1.0);
    pos = transpose(mat3(V)) * (inverse(P) * gl_Position).xyz;
    // vec4 temp = transpose(V) * (inverse(P) * gl_Position);
    // pos = vec3(temp);
    // pos = (P * V * gl_Position).xyz;
    fsun = vec3(0.0, sin(time * 0.01), cos(time * 0.01));

}