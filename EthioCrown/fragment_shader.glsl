#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform vec3 objectColor;   // Base object color
uniform vec3 lightColor;    // Spotlight color
uniform vec3 lightPos;      // Spotlight position
uniform vec3 lightDir;      // Spotlight direction
uniform vec3 viewPos;       // Camera position
uniform float cutOff;       // Spotlight cutoff angle (cosine value)
uniform float outerCutOff;  // Spotlight outer cutoff for smooth edges
uniform vec3 outsideColor;  // Outer wall color
uniform vec3 insideColor;   // Inner wall color

void main() {
    // Normalize vectors
    vec3 norm = normalize(Normal);
    vec3 lightDirNorm = normalize(lightPos - FragPos);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 spotlightDir = normalize(lightDir);

    // Spotlight effect
    float theta = dot(lightDirNorm, -spotlightDir); // Angle between spotlight direction and fragment
    float epsilon = cutOff - outerCutOff;
    float intensity = clamp((theta - outerCutOff) / epsilon, 0.0, 1.0); // Spotlight intensity

    // Ambient
    float ambientStrength = 0.5; // Subtle ambient lighting
    vec3 ambient = ambientStrength * lightColor;

    // Diffuse
    float diff = max(dot(norm, lightDirNorm), 0.0);
    vec3 diffuse = diff * lightColor * 0.9;

    // Specular
    float specularStrength = 0.5; // Reduced specular intensity
    vec3 reflectDir = reflect(-lightDirNorm, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16.0); // Softer highlights
    vec3 specular = specularStrength * spec * lightColor;

    // Apply texture only to the outer surface
    vec3 textureColor = texture(texture1, TexCoord).rgb;

    // Determine if the fragment belongs to the inner or outer cylinder
    vec3 resultColor;
    if (dot(norm, lightDirNorm) < 0.0) {
        resultColor = insideColor;  // Apply insideColor for the inner cylinder
    } else {
        resultColor = textureColor; // Apply texture for the outer cylinder
    }

    // Combine lighting effects with spotlight intensity
    vec3 finalColor = (ambient + intensity * (diffuse + specular)) * resultColor;
    FragColor = vec4(finalColor, 1.0);
}
