#version 450 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoords;

out vec2 TexCoords;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

void main()
{
    TexCoords = texCoords;
    mat4 ViewModel = View * Model;
      // First colunm.
    ViewModel[0][0] = 1.0; 
    ViewModel[0][1] = 0.0; 
    ViewModel[0][2] = 0.0; 

    // Second colunm.
    ViewModel[1][0] = 0.0; 
    ViewModel[1][1] = 1.0; 
    ViewModel[1][2] = 0.0; 

    // Thrid colunm.
    ViewModel[2][0] = 0.0; 
    ViewModel[2][1] = 0.0; 
    ViewModel[2][2] = 1.0; 

    gl_Position = Projection * ViewModel * vec4(position, 1.0f);
} 