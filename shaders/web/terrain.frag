#version 300 es
precision mediump float;

in vec3 vBarycentric;
in vec3 vNormal;
in vec2 vTexCoords;

uniform sampler2D texture2d;
uniform vec4 ambient_color;
uniform vec4 light_direction;
uniform bool wireframe_mode;
uniform vec3 wireframe_color;
uniform float wireframe_width;

out vec4 fragColor;

float edgeFactor() {
    vec3 d = fwidth(vBarycentric);
    vec3 a3 = smoothstep(vec3(0.0), d * wireframe_width, vBarycentric);
    return min(min(a3.x, a3.y), a3.z);
}

void main() {
    vec4 texColor = texture(texture2d, vTexCoords);

    // Basic lighting
    float lightIntensity = max(dot(normalize(vNormal), normalize(light_direction.xyz)), 0.0);
    vec4 litColor = texColor * (ambient_color + vec4(lightIntensity, lightIntensity, lightIntensity, 1.0));

    if (wireframe_mode) {
        float edge = edgeFactor();
        fragColor = mix(vec4(wireframe_color, 1.0), litColor, edge);
    } else {
        fragColor = litColor;
    }
}