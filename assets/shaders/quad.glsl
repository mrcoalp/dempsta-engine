#type vertex

#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec2 texture;

out vec4 v_color;
out vec2 v_texture;

uniform mat4 u_viewProjection;

void main() {
    v_color = color;
    v_texture = texture;
    gl_Position = u_viewProjection * vec4(position, 1.0);
}

#type fragment

#version 330 core

layout(location = 0) out vec4 color;

in vec4 v_color;
in vec2 v_texture;

uniform sampler2D u_texture;
// uniform vec4 u_color;

void main() {
    // color = texture(u_texture, v_texture) * u_color;
    color = v_color;
}
