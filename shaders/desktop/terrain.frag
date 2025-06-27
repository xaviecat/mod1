#version 460

//Inputs from vertex shader
in vec3 vNormal;

in float color_factor;
in vec2 vTexCoord;
in vec3 worldNormal;

// Uniforms from cpp
uniform vec4 light_direction;
uniform vec4 ambient_color;
uniform sampler2D texture2d;

//Outputs for GPU
out vec4 color;

void main(void)
{
    float diffuse = max(dot(normalize(vNormal), normalize(light_direction.xyz)), 0.0);

    vec3 baseColor = vec3(1.0, 1.0, 1.0);

    vec3 ambient = ambient_color.rgb;
    vec3 diffuseColor = diffuse * baseColor * 0.8;

    vec4 texColor = texture(texture2d, vTexCoord.st);
    color = vec4(ambient + diffuseColor, 1.0) * texColor;
}
