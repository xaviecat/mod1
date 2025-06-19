#version 460

// Inputs from cpp attribute
in vec3 aNormal;
in vec4 aVertex;
in vec2 aTexCoords;

// Uniforms from cpp
uniform mat4 mvpmatrix;

// Outputs for fragment shader
out vec2 vTexCoord;
out vec3 vNormal;

void main(void)
{
    gl_Position = mvpmatrix * aVertex;

    vNormal = aNormal;
    vTexCoord = aTexCoords;
}


