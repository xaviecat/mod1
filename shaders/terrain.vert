#version 460

// Inputs from cpp
in vec3 aNormal;
in vec4 aVertex;
in vec2 aTexCoords;

// Uniforms from cpp
uniform mat4 mvpmatrix;
uniform vec4 light_direction;
uniform vec4 ambiant_color;

// Outputs for fragment shader
out vec2 vTexCoord;
out vec4 Color;
out vec3 worldNormal;
out vec3 worldPosition;

void main(void)
{
    gl_Position = mvpmatrix * aVertex;

    worldPosition = aVertex.xyz;
    worldNormal = normalize(aNormal);

    vec3 lightDir = normalize(light_direction.xyz);
    float diffuse = max(dot(worldNormal, lightDir), 0.0);

    vec3 ambient = ambiant_color.rgb;
    vec3 finalColor = ambient + diffuse * vec3(0.6, 0.6, 0.6); // White diffuse light

    vTexCoord = aTexCoords;
    Color = vec4(finalColor, 1.0);
}


