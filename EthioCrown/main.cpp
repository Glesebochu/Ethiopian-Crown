#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>
#include <vector>
#include <cmath>
#include "shader.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Define pi manually
const float PI = 3.14159265359f;

// Window settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// Cylinder settings
const int SECTORS = 36;
const float OUTER_RADIUS = 1.2f;
const float INNER_RADIUS = 1.1f;
const float HEIGHT = 1.2f;

// Cross settings
const float CROSS_WIDTH = 0.4f;
const float CROSS_HEIGHT = 0.7f;
const float CROSS_THICKNESS = 0.1f;

// Function to generate the hollow cylinder vertices and indices
void generateHollowCylinder(float radiusOuter, float radiusInner, float height, int sectors,
    std::vector<GLfloat>& vertices, std::vector<GLuint>& indices) {
    float sectorStep = 2 * PI / sectors;

    for (int i = 0; i <= sectors; ++i) {
        float angle = i * sectorStep;
        float u = static_cast<float>(i) / sectors;

        // Outer top
        vertices.push_back(radiusOuter * cos(angle));
        vertices.push_back(height / 2);
        vertices.push_back(radiusOuter * sin(angle));
        vertices.push_back(u);
        vertices.push_back(1.0f);

        // Outer bottom
        vertices.push_back(radiusOuter * cos(angle));
        vertices.push_back(-height / 2);
        vertices.push_back(radiusOuter * sin(angle));
        vertices.push_back(u);
        vertices.push_back(0.0f);

        // Inner top
        vertices.push_back(radiusInner * cos(angle));
        vertices.push_back(height / 2);
        vertices.push_back(radiusInner * sin(angle));
        vertices.push_back(u);
        vertices.push_back(1.0f);

        // Inner bottom
        vertices.push_back(radiusInner * cos(angle));
        vertices.push_back(-height / 2);
        vertices.push_back(radiusInner * sin(angle));
        vertices.push_back(u);
        vertices.push_back(0.0f);
    }

    for (int i = 0; i < sectors; ++i) {
        int current = i * 4;
        int next = (i + 1) % sectors * 4;

        // Outer surface
        indices.push_back(current);
        indices.push_back(next);
        indices.push_back(current + 1);
        indices.push_back(next);
        indices.push_back(next + 1);
        indices.push_back(current + 1);

        // Inner surface
        indices.push_back(current + 2);
        indices.push_back(next + 2);
        indices.push_back(current + 3);
        indices.push_back(next + 2);
        indices.push_back(next + 3);
        indices.push_back(current + 3);

        // Top face (filling the gap)
        indices.push_back(current);
        indices.push_back(current + 2);
        indices.push_back(next);
        indices.push_back(next);
        indices.push_back(current + 2);
        indices.push_back(next + 2);

        // Bottom face (filling the gap)
        indices.push_back(current + 1);
        indices.push_back(next + 1);
        indices.push_back(current + 3);
        indices.push_back(next + 1);
        indices.push_back(next + 3);
        indices.push_back(current + 3);
    }
}
// Function to generate the cross vertices and indices
void generateCross(float width, float height, float thickness, std::vector<GLfloat>& vertices, std::vector<GLuint>& indices)
{
    // Define the cross as two rectangles (vertical and horizontal)
    float halfWidth = width / 2;
    float halfHeight = height / 2;
    float halfThickness = thickness / 2;

    // Vertical part
    vertices.push_back(-halfThickness); vertices.push_back(-halfHeight - 0.1); vertices.push_back(0.0f); vertices.push_back(0.0f); vertices.push_back(0.0f);
    vertices.push_back(halfThickness); vertices.push_back(-halfHeight - 0.1); vertices.push_back(0.0f); vertices.push_back(1.0f); vertices.push_back(0.0f);
    vertices.push_back(halfThickness); vertices.push_back(halfHeight - 0.1); vertices.push_back(0.0f); vertices.push_back(1.0f); vertices.push_back(1.0f);
    vertices.push_back(-halfThickness); vertices.push_back(halfHeight - 0.1); vertices.push_back(0.0f); vertices.push_back(0.0f); vertices.push_back(1.0f);

    // Horizontal part
    vertices.push_back(-halfWidth); vertices.push_back(-halfThickness); vertices.push_back(0.0f); vertices.push_back(0.0f); vertices.push_back(0.0f);
    vertices.push_back(halfWidth); vertices.push_back(-halfThickness); vertices.push_back(0.0f); vertices.push_back(1.0f); vertices.push_back(0.0f);
    vertices.push_back(halfWidth); vertices.push_back(halfThickness); vertices.push_back(0.0f); vertices.push_back(1.0f); vertices.push_back(1.0f);
    vertices.push_back(-halfWidth); vertices.push_back(halfThickness); vertices.push_back(0.0f); vertices.push_back(0.0f); vertices.push_back(1.0f);

    // Indices for the cross
    indices = {
        0, 1, 2, 2, 3, 0, // Vertical part
        4, 5, 6, 6, 7, 4  // Horizontal part
    };
}

int main()
{
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // OpenGL version and profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create window
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Textured Hollow Cylinder with Cross", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    glEnable(GL_DEPTH_TEST);

    // Load shaders
    Shader shader("vertex_shader.glsl", "fragment_shader.glsl");

    // Generate cylinder
    std::vector<GLfloat> vertices;
    std::vector<GLuint> indices;
    generateHollowCylinder(OUTER_RADIUS, INNER_RADIUS, HEIGHT, SECTORS, vertices, indices);

    // Generate cross
    std::vector<GLfloat> crossVertices;
    std::vector<GLuint> crossIndices;
    generateCross(CROSS_WIDTH, CROSS_HEIGHT, CROSS_THICKNESS, crossVertices, crossIndices);

    // Set up VAO, VBO, and EBOs for cylinder
    GLuint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Set up VAO, VBO, and EBO for cross
    GLuint crossVAO, crossVBO, crossEBO;
    glGenVertexArrays(1, &crossVAO);
    glGenBuffers(1, &crossVBO);
    glGenBuffers(1, &crossEBO);

    glBindVertexArray(crossVAO);

    glBindBuffer(GL_ARRAY_BUFFER, crossVBO);
    glBufferData(GL_ARRAY_BUFFER, crossVertices.size() * sizeof(GLfloat), crossVertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, crossEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, crossIndices.size() * sizeof(GLuint), crossIndices.data(), GL_STATIC_DRAW);

    // Vertex attributes (x, y, z, u, v)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Load outer texture (goldi.jpg)
    GLuint outerTexture;
    glGenTextures(1, &outerTexture);
    glBindTexture(GL_TEXTURE_2D, outerTexture);

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load("goldi.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cerr << "Failed to load outer texture!" << std::endl;
    }
    stbi_image_free(data);

    // Load inner texture
    GLuint innerTexture;
    glGenTextures(1, &innerTexture);
    glBindTexture(GL_TEXTURE_2D, innerTexture);

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    data = stbi_load("heyy.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cerr << "Failed to load inner texture!" << std::endl;
    }
    stbi_image_free(data);

    // Load cross texture
    GLuint crossTexture;
    glGenTextures(1, &crossTexture);
    glBindTexture(GL_TEXTURE_2D, crossTexture);

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    data = stbi_load("goldi.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cerr << "Failed to load cross texture!" << std::endl;
    }
    stbi_image_free(data);

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);

    // Render loop
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Use shader
        shader.use();

        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 3.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        shader.setMat4("model", model);
        shader.setMat4("view", view);
        shader.setMat4("projection", projection);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);


        // Set light properties
        shader.setVec3("lightPos", glm::vec3(0.0f, 0.0f, 5.0f));
        shader.setVec3("lightDir", glm::vec3(0.0f, 0.0f, -1.0f));
        shader.setFloat("cutOff", glm::cos(glm::radians(12.5f)));
        shader.setFloat("outerCutOff", glm::cos(glm::radians(17.5f)));

       
        // Pass the updated model matrix to the shader
        glm::mat4 crossModel = glm::translate(model, glm::vec3(0.0f, HEIGHT / 2 + CROSS_HEIGHT / 2 + 0.55, 1.8f));
        shader.setMat4("model", crossModel);
        glBindTexture(GL_TEXTURE_2D, crossTexture);
        glBindVertexArray(crossVAO);
        glDrawElements(GL_TRIANGLES, crossIndices.size(), GL_UNSIGNED_INT, 0);

        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glfwTerminate();
    return 0;
}