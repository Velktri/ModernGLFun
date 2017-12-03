#version 450 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;

out vec3 ourColor;
  
uniform mat4 model;
uniform mat4 ViewProjection;

void main()
{
    gl_Position = ViewProjection * model * vec4(position, 1.0f);
    ourColor = color;
}