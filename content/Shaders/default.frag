#version 430

uniform vec4 tint;

in vec4 _color;
in vec3 _normal;

out vec4 finalColor;

void main()
{
	float diffFactor = dot(_normal, normalize(vec3(0.0, 1.0, -1.0)));	
	finalColor = _color;
}