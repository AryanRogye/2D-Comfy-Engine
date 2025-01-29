#version 410 core

layout (location = 0) in int vertexID;  // Vertex ID input

const vec2 vertices[6] = vec2[](  // Must be `const` outside `main`
    vec2(-0.5, 0.5),  // Top Left
    vec2(-0.5, -0.5), // Bottom Left
    vec2(0.5, 0.5),   // Top Right
    vec2(0.5, 0.5),   // Top Right
    vec2(-0.5, -0.5), // Bottom Left
    vec2(0.5, -0.5)   // Bottom Right
);

void main()
{
    gl_Position = vec4(vertices[gl_VertexID], 0.0, 1.0);
}
