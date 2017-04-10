#version 450 core
in vec3 ourColor;

out vec4 color;

uniform int type;

void main()
{
	if (type == 1) {
		color = vec4(ourColor, 1.0f);	
	} else if (type == 2 || type == 3) {
		color = vec4(0.0f, 1.0f, 0.0f, 1.0f);
	} else {
		color = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	}
}