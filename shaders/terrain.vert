#version 130

in vec4 vertex;
in vec3 normal;
uniform mat4 mvpmatrix;

uniform vec4 light_direction;
uniform vec4 ambiant_color;

out vec4 Color;
out vec3 worldNormal;
out vec3 worldPosition;

void main(void)
{
    gl_Position = mvpmatrix * vertex;

    worldPosition = vertex.xyz;
    worldNormal = normalize(normal);

    vec3 lightDir = normalize(light_direction.xyz);
    float diffuse = max(dot(worldNormal, lightDir), 0.0);

    vec3 ambient = ambiant_color.rgb;
    vec3 finalColor = ambient + diffuse * vec3(0.6, 0.6, 0.6); // White diffuse light

    Color = vec4(finalColor, 1.0);
}