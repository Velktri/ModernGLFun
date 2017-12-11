#version 450 core
layout (location = 0) in vec3 position;

out vec3 ourColor;
  
uniform mat4 model;
uniform mat4 ViewProjection;
uniform vec3 color;
uniform int type;

void main() {
	ourColor = color;
    gl_Position = ViewProjection * model * vec4(position, 1.0f);

	if (type == 1) {
		if (position.x == 0.0f) {
    		ourColor = vec3(0.0f, 0.0f, 1.0f);
		} else if (position.z == 0.0f) {
			ourColor = vec3(1.0f, 0.0f, 0.0f);
		}
	}
}