#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>
#include "shader.hpp"


void generateHollowCylinder(float radiusOuter, float radiusInner, float height, int sectors, std::vector<GLfloat>& vertices, std::vector<GLuint>& indices)
{
    float sectorStep = 2 * glm::pi<float>() / sectors;

    // Generate vertices
    for (int i = 0; i <= sectors; ++i)
    {
        float angle = i * sectorStep;
        // Outer top
        vertices.push_back(radiusOuter * cos(angle));
        vertices.push_back(height / 2);
        vertices.push_back(radiusOuter * sin(angle));
        // Outer bottom
        vertices.push_back(radiusOuter * cos(angle));
        vertices.push_back(-height / 2);
        vertices.push_back(radiusOuter * sin(angle));

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

int main()
{
    // Initialize GLFW
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW!" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Hollow Cylinder", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        std::cerr << "Failed to create GLFW window!" << std::endl;
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD!" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);


    // Log OpenGL version
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

    // Load shaders
    Shader shader("vertex_shader.glsl", "fragment_shader.glsl");

    // Cylinder parameters
    int sectors = 36;
    float radiusOuter = 1.0f;
    float radiusInner = 0.9f; // Inner radius for hollow effect
    float height = 1.0f;

    std::vector<GLfloat> vertices;
    std::vector<GLuint> indices;
    generateHollowCylinder(radiusOuter, radiusInner, height, sectors, vertices, indices);

    // VAO, VBO, and EBO setup
    GLuint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);


    // Projection and view matrices
    glm::mat4 view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();

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
        shader.setVec3("outsideColor", glm::vec3(1.0f, 0.85f, 0.3f)); // Bright gold for outer surface
        shader.setVec3("insideColor", glm::vec3(0.6f, 0.5f, 0.2f));  // Darker gold for inner surface

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

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
