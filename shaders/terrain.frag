#version 460

// Inputs from vertex shader
in float color_factor;
in vec2 vTexCoord;

// Uniforms from cpp
uniform vec4 ambiant_color;
uniform sampler2D texture2d;

// Outputs for GPU
out vec4 color;

void main(void)
{
    vec4 texColor = texture(texture2d, vTexCoord.st);
    color = texColor;
}
