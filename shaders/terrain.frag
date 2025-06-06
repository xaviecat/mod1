#version 130

in float color_factor;
out vec4 color;
uniform vec4 ambiant_color;

void main(void)
{
    color = color_factor * ambiant_color;
}