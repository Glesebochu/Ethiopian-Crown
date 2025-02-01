#version 330 core
layout(location = 0) in vec3 aPos;    // Vertex position
layout(location = 1) in vec3 aNormal; // Vertex normal
layout(location = 2) in vec2 aTexCoord; // Texture coordinates

out vec3 FragPos;  // Position for fragment shader
out vec3 Normal;   // Normal for fragment shader
out vec2 TexCoord; // Texture coordinate passed to fragment shader

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    // Transform vertex position to world coordinates
    FragPos = vec3(model * vec4(aPos, 1.0));

    // Apply normal transformation (only needed for non-uniform scaling)
    Normal = mat3(transpose(inverse(model))) * aNormal;

    // Compute final vertex position
    gl_Position = projection * view * model * vec4(aPos, 1.0);

    // Pass texture coordinates to fragment shader
    TexCoord = aTexCoord; 
}
