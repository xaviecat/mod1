#version 460

// Inputs from cpp
in vec4 aNormal;
in vec4 aVertex;
in vec2 aTexCoords;

// Uniforms from cpp
uniform mat4 mvpmatrix;
uniform vec4 light_direction;

// Outputs for fragment shader
out vec2 vTexCoord;

void main(void)
{

    gl_Position = mvpmatrix * aVertex;

    vTexCoord = aTexCoords;
}
