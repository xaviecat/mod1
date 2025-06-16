#version 130

in float color_factor;
out vec4 color;
uniform vec4 ambiant_color;
in vec4 Color;
void main(void)
{
//    color = color_factor * ambiant_color;
    color = Color;
}