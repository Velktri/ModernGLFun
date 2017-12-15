#version 450 core
out vec4 color;

in vec2 TexCoords;
uniform vec3 LampColor;
uniform sampler2D LampTexture;

void main()
{
    vec4 texColor = texture(LampTexture, TexCoords);
    if(texColor.a < 0.7) { discard; }
    color = texColor * vec4(LampColor, 1.0); 
}