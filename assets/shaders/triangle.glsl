#type vertex

#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;

uniform mat4 u_viewProjection;
uniform mat4 u_transform;

out vec3 o_position;
out vec4 o_color;

void main() {
    o_position = position;
    o_color = color;
    gl_Position = u_viewProjection * u_transform * vec4(position, 1.0);
}

#type fragment

#version 330 core

layout(location = 0) out vec4 color;

in vec3 o_position;
in vec4 o_color;

void main() {
    color = vec4(o_position * 0.5 + 0.5, 0.1);
    color = o_color;
}
