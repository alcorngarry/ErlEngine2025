#version 330 core

out vec4 color;

uniform vec4 PickingColor;

void main() 
{
	color = PickingColor;
}