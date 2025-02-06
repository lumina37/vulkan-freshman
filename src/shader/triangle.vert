#version 460

layout (location = 0) out vec4 vertexColor;

const vec2 positions[] = vec2[](
vec2(-0.5, 0.5),
vec2(0.5, 0.5),
vec2(0.0, -0.5)
);

const vec3 colors[] = vec3[](
vec3(1.0, 0.0, 0.0),
vec3(0.0, 1.0, 0.0),
vec3(0.0, 0.0, 1.0)
);

void main() {
    gl_Position = vec4(positions[gl_VertexIndex], 0.0, 1.0);
    vertexColor = vec4(colors[gl_VertexIndex], 1.0);
}
