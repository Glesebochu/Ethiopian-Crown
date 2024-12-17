#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

// Define the vertices of a cylinder
void generateCylinder(float radius, float height, int sectors, GLfloat* vertices) {
    int index = 0;
    float sectorStep = 2 * glm::pi<float>() / sectors;
    for (int i = 0; i < sectors; ++i) {
        float angle = i * sectorStep;
        vertices[index++] = radius * cos(angle);  // X
        vertices[index++] = -height / 2;           // Y (bottom circle)
        vertices[index++] = radius * sin(angle);  // Z

        vertices[index++] = radius * cos(angle);  // X
        vertices[index++] = height / 2;           // Y (top circle)
        vertices[index++] = radius * sin(angle);  // Z
    }
}

int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW!" << std::endl;
        return -1;
    }

    // Create a windowed mode window and its OpenGL context
    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Cylinder", NULL, NULL);
    if (!window) {
        glfwTerminate();
        std::cerr << "Failed to create GLFW window!" << std::endl;
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    // Load OpenGL functions with GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD!" << std::endl;
        return -1;
    }

    // Set up vertex data (a simple cylinder)
    int sectors = 36; // Number of slices around the cylinder
    float radius = 0.5f;
    float height = 1.0f;

    int vertexCount = 2 * sectors; // Each sector has two vertices (top and bottom)
    GLfloat* vertices = new GLfloat[vertexCount * 3]; // 3 coordinates per vertex
    generateCylinder(radius, height, sectors, vertices);

    GLuint VBO, VAO;
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertexCount * 3 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Simple rendering loop
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Render the cylinder
        glBindVertexArray(VAO);
        glDrawArrays(GL_LINES, 0, vertexCount); // Draw the cylinder
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Clean up
    delete[] vertices;
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}



