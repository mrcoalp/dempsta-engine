#type vertex

#version 400 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec2 texture;
layout(location = 3) in float textureIndex;

out vec4 v_color;
out vec2 v_texture;
out float v_textureIndex;

uniform mat4 u_viewProjection;

void main() {
    v_color = color;
    v_texture = texture;
    v_textureIndex = textureIndex;
    gl_Position = u_viewProjection * vec4(position, 1.0);
}

#type fragment

#version 400 core

layout(location = 0) out vec4 color;

in vec4 v_color;
in vec2 v_texture;
in float v_textureIndex;

uniform sampler2D u_textures[32];

void main() {
    vec4 texColor = texture(u_textures[int(v_textureIndex)], v_texture) * v_color;
    if (texColor.a < 0.1) discard;
    color = texColor;
}
