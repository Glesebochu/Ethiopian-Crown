#version 330 core
layout(location = 0) in vec3 aPos;   // Vertex position
layout(location = 1) in vec2 aTexCoord; // Texture coordinates

out vec3 FragPos;  // Position for fragment shader
out vec2 TexCoord; // Texture coordinates for fragment shader

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));  // Transform vertex position
    TexCoord = aTexCoord; // Pass texture coordinates
    gl_Position = projection * view * vec4(FragPos, 1.0);
}