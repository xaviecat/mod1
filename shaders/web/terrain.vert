#version 300 es

in vec3 aVertex;
in vec3 aNormal;
in vec2 aTexCoords;
in vec3 aBarycentric; // New attribute for barycentric coordinates

uniform mat4 mvpmatrix;

out vec3 vBarycentric;
out vec3 vNormal;
out vec2 vTexCoords;

void main() {
    gl_Position = mvpmatrix * vec4(aVertex, 1.0);
    vBarycentric = aBarycentric;
    vNormal = aNormal;
    vTexCoords = aTexCoords;
}