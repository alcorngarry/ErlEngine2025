#version 330 core

layout(points) in;  // Expecting point (line) primitives
layout(triangle_strip, max_vertices = 4) out;  // We output 4 vertices (for a quad)

in vec3 aPos[];

uniform float lineWidth;  // Thickness of the line

void main()
{
    // Direction vector for the line
    vec3 dir = normalize(aPos[1] - aPos[0]);

    // Normal vector perpendicular to the line's direction in 2D
    vec3 normal = normalize(cross(dir, vec3(0.0, 0.0, 1.0)));

    // Half the line width to offset vertices
    float halfWidth = lineWidth * 0.5;

    // Offset the original points in both directions along the normal to create a quad
    vec3 offset1 = aPos[0] + normal * halfWidth;
    vec3 offset2 = aPos[1] + normal * halfWidth;
    vec3 offset3 = aPos[0] - normal * halfWidth;
    vec3 offset4 = aPos[1] - normal * halfWidth;

    // Output the four vertices of the quad
    gl_Position = gl_in[0].gl_Position;
    gl_Position = projection * view * vec4(offset1, 1.0f); EmitVertex();
    gl_Position = projection * view * vec4(offset2, 1.0f); EmitVertex();
    gl_Position = projection * view * vec4(offset3, 1.0f); EmitVertex();
    gl_Position = projection * view * vec4(offset4, 1.0f); EmitVertex();

    EndPrimitive();
}
