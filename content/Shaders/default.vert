#version 430

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texcoord;
layout(location = 2) in vec4 color;
layout(location = 3) in vec3 normal;
layout(location = 4) in vec3 tangent;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 modelInverseTranspose;

uniform vec3 viewPos;

out vec4 _color;
out vec3 _normal;

void main()
{
	_color = color;
	_normal = normal * mat3(modelInverseTranspose);
	gl_Position = projection * view * model * vec4(position, 1.0);
}