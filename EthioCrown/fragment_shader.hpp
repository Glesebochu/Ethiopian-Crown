#ifndef FRAGMENT_SHADER_HPP
#define FRAGMENT_SHADER_HPP

const char* fragmentShaderSource = R"(
#version 330 core
in vec3 fragNormal;
in vec3 fragPosition;

out vec4 color;

uniform vec3 lightPos;
uniform vec3 viewPos;

void main() {
    // Normalize the normal
    vec3 norm = normalize(fragNormal);

    // Calculate light direction
    vec3 lightDir = normalize(lightPos - fragPosition);

    // Diffuse shading
    float diff = max(dot(norm, lightDir), 0.0);

    // Ambient light
    float ambient = 0.1;

    // Final color
    vec3 result = (ambient + diff) * vec3(1, 1, 0); // Reddish shade
    color = vec4(result, 1.0);
}
)";

#endif // FRAGMENT_SHADER_HPP
