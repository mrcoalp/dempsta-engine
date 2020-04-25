#type vertex

#version 330 core

layout(location = 0) in vec3 position;

uniform mat4 u_viewProjection;
uniform mat4 u_transform;

void main() { gl_Position = u_viewProjection * u_transform * vec4(position, 1.0); }

#type fragment

#version 330 core

layout(location = 0) out vec4 color;

uniform vec3 u_color;

void main() { color = vec4(u_color, 1.0); }
