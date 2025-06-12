#version 130

in vec4 vertex;
in vec4 normal;
uniform mat4 mvpmatrix;
out float color_factor;
uniform vec4 light_direction;

void main(void)
{
    color_factor = max(dot(normal, light_direction), 0.0);
    gl_Position = mvpmatrix * vertex;
}