#version 130

out vec4 color;
in vec4 Color;
in vec3 worldNormal;
in vec3 worldPosition;

uniform vec4 light_direction;
uniform vec4 ambiant_color;

void main(void)
{
    vec3 normal = normalize(worldNormal);
    vec3 lightDir = normalize(light_direction.xyz);

    float diffuse = max(dot(normal, lightDir), 0.0);

    float height = worldPosition.y;
    vec3 baseColor = vec3(0.2 + height * 0.3, 0.4 + height * 0.2, 0.1 + height * 0.1);

    vec3 ambient = ambiant_color.rgb * baseColor;
    vec3 diffuseColor = diffuse * baseColor * 0.8;

    color = vec4(ambient + diffuseColor, 1.0);
}