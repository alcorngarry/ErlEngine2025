#version 330 core

uniform sampler2D texture1;

in vec2 texCoord;
out vec4 FragColor;

void main()
{
	FragColor = texture(texture1, texCoord);
	if (FragColor.a < 0.1)
	{
		discard;
	}
};