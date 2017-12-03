#version 450 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

uniform mat4 model;
uniform mat4 ViewProjection;
uniform vec3 cameraPos;

out vec3 Normal;
out vec3 CameraPosition;

void main()
{
    gl_Position = ViewProjection * model * vec4(position, 1.0f);
    Normal = mat3(transpose(inverse(model))) * normal;
    CameraPosition = cameraPos;
}