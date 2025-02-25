#version 330 core

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

uniform mat4 VP;
uniform vec3 cameraPos;

out vec2 texCoord;

void main()
{
    vec3 Pos = gl_in[0].gl_Position.xyz;
    vec3 CameraToPoint = normalize(Pos - cameraPos);
    vec3 up = vec3(0.0, 1.0, 0.0);
    vec3 right = cross(up, CameraToPoint);

    right *= 10;

    gl_Position = VP * vec4(Pos, 1.0);
    texCoord = vec2(0.0, 0.0);
    EmitVertex();

    Pos.y += 10.0;
    gl_Position = VP * vec4(Pos, 1.0);
    texCoord = vec2(0.0, 1.0);
    EmitVertex();

    Pos.y -= 10.0;
    Pos += right;
    gl_Position = VP * vec4(Pos, 1.0);
    texCoord = vec2(1.0, 0.0);
    EmitVertex();

    Pos.y += 10.0;
    gl_Position = VP * vec4(Pos, 1.0);
    texCoord = vec2(1.0, 1.0);
    EmitVertex();

    EndPrimitive();
};