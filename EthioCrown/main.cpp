#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "shader.hpp"

// Function to load texture
GLuint loadTexture(const char* path) {
    GLuint textureID;
    glGenTextures(1, &textureID);

    int width, height, nrChannels;
    unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);
    if (!data) {
        std::cerr << "Failed to load texture: " << path << std::endl;
        return 0;  // Return 0 if loading fails
    }
    else {
        GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        stbi_image_free(data);
    }
    return textureID;
}

// Function to generate a hollow cylinder with texture coordinates
void generateHollowCylinder(float radiusOuter, float radiusInner, float height, int sectors,
    std::vector<float>& vertices, std::vector<unsigned int>& indices) {
    float sectorStep = 2 * glm::pi<float>() / sectors;

    // Generate vertices
    for (int i = 0; i <= sectors; ++i)
    {
        float angle = i * sectorStep;
        float u = (float)i / sectors; // Texture coordinate U mapping

        // Outer top
        vertices.push_back(radiusOuter * cos(angle));
        vertices.push_back(height / 2);
        vertices.push_back(radiusOuter * sin(angle));
        vertices.push_back(u);                       // Texture coordinate u
        vertices.push_back(1.0f);                    // Texture coordinate v

        // Outer bottom
        vertices.push_back(radiusOuter * cos(angle));
        vertices.push_back(-height / 2);
        vertices.push_back(radiusOuter * sin(angle));
        vertices.push_back(u);                       // Texture coordinate u
        vertices.push_back(0.0f);                    // Texture coordinate v

        // Inner top
        vertices.push_back(radiusInner * cos(angle));
        vertices.push_back(height / 2);
        vertices.push_back(radiusInner * sin(angle));
        // Inner bottom
        vertices.push_back(radiusInner * cos(angle));
        vertices.push_back(-height / 2);
        vertices.push_back(radiusInner * sin(angle));
    }

    // Generate indices for triangle strips
    for (int i = 0; i < sectors; ++i)
    {
        int k1 = i * 4;
        int k2 = k1 + 4;

        // Outer surface
        indices.push_back(k1);
        indices.push_back(k1 + 1);
        indices.push_back(k2);

        indices.push_back(k2);
        indices.push_back(k1 + 1);
        indices.push_back(k2 + 1);

        // Inner surface
        indices.push_back(k1 + 2);
        indices.push_back(k1 + 3);
        indices.push_back(k2 + 2);

        indices.push_back(k2 + 2);
        indices.push_back(k1 + 3);
        indices.push_back(k2 + 3);

        // Top edge
        indices.push_back(k1);
        indices.push_back(k1 + 2);
        indices.push_back(k2);

        indices.push_back(k2);
        indices.push_back(k1 + 2);
        indices.push_back(k2 + 2);

        // Bottom edge
        indices.push_back(k1 + 1);
        indices.push_back(k1 + 3);
        indices.push_back(k2 + 1);

        indices.push_back(k2 + 1);
        indices.push_back(k1 + 3);
        indices.push_back(k2 + 3);
    }
}
// Function to check OpenGL errors
void checkOpenGLError(const char* functionName) {
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        std::cerr << "🚨 OpenGL Error in " << functionName << ": " << err << std::endl;
    }
}

int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW!" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Textured Hollow Cylinder", NULL, NULL);
    if (!window) {
        glfwTerminate();
        std::cerr << "Failed to create GLFW window!" << std::endl;
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD!" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);
    checkOpenGLError("OpenGL Initialization");  // ✅ Check errors after initialization


    // Load texture
    GLuint texture = loadTexture("Image.jpg"); // Ensure the texture exists in the directory
    // Check if texture loaded properly
    if (texture == 0) {
        std::cerr << "Texture loading failed! Exiting program." << std::endl;
        return -1;
    }

    // Load shaders
    Shader shader("vertex_shader.glsl", "fragment_shader.glsl");
    checkOpenGLError("Shader Compilation");  // ✅ Check errors after shader loading

    // Cylinder parameters
    int sectors = 36;
    float radiusOuter = 1.0f;
    float radiusInner = 0.85f;
    float height = 1.0f;

    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    generateHollowCylinder(radiusOuter, radiusInner, height, sectors, vertices, indices);

    // VAO, VBO, EBO setup
    GLuint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Texture Coordinate attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Projection and view matrices
    glm::mat4 view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);


    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();

        // Bind texture before drawing
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        shader.setInt("texture1", 0);

        // Check for OpenGL errors before drawing
        checkOpenGLError("Before Rendering");


        // Set transformation matrices
        glm::mat4 model = glm::mat4(1.0f);
        shader.setMat4("model", model);
        shader.setMat4("view", view);
        shader.setMat4("projection", projection);

        shader.setVec3("lightPos", glm::vec3(0.0f, 1.0f, 3.0f)); // Spotlight above and in front of the crown
        shader.setVec3("lightDir", glm::vec3(0.0f, -0.3f, -1.0f)); // Spotlight directed toward the crown (slightly downward)
        shader.setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f)); // Dimmer light
        shader.setVec3("viewPos", glm::vec3(2.0f, 2.0f, 5.0f));    // Camera positioned behind and above the spotlight
        shader.setFloat("cutOff", glm::cos(glm::radians(50.0f)));  // Inner cone angle for direct illumination
        shader.setFloat("outerCutOff", glm::cos(glm::radians(60.0f))); // Wider outer cone angle for smoother edges
        shader.setVec3("insideColor", glm::vec3(0.6f, 0.5f, 0.2f));  // Darker gold for inner surface

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        // Check for OpenGL errors after drawing
        checkOpenGLError("After Rendering");

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &VAO);
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
