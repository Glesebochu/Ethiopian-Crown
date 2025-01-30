#version 330 core
layout(location = 0) in vec3 aPos;   // Vertex position
layout(location = 1) in vec3 aNormal; // Vertex normal
out vec2 TexCoord;
out vec3 FragPos;  // Position for fragment shader
out vec3 Normal;   // Normal for fragment shader

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));  // Transform vertex position
    Normal = mat3(transpose(inverse(model))) * aNormal; // Correct normals
    TexCoord = aTexCoord; 
    gl_Position = projection * view * model * vec4(aPos, 1.0);


}
