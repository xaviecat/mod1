#version 460

//Inputs from vertex shader
in float color_factor;
in vec2 vTexCoord;
in vec3 worldNormal;
in vec3 worldPosition;
// Uniforms from cpp
uniform vec4 light_direction;
uniform vec4 ambiant_color;
uniform sampler2D texture2d;

//Outputs for GPU
out vec4 color;

void main(void)
{
    vec3 normal = normalize(worldNormal);
    vec3 lightDir = normalize(light_direction.xyz);

    float diffuse = max(dot(normal, lightDir), 0.0);

    float height = worldPosition.y;
    vec3 baseColor = vec3(1.0, 1.0, 1.0);

    vec3 ambient = ambiant_color.rgb * baseColor;
    vec3 diffuseColor = diffuse * baseColor * 0.8;

    vec4 texColor = texture(texture2d, vTexCoord.st);
    color = vec4(ambient + diffuseColor, 1.0) * texColor;
}
