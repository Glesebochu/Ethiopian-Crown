#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "shader.hpp"


// Define the vertices of a cylinder
void generateCylinder(float radius, float height, int sectors, GLfloat * vertices) {
    int index = 0;
    float sectorStep = 2 * glm::pi<float>() / sectors;
    for (int i = 0; i <= sectors; ++i) {
        float angle = i * sectorStep;
        vertices[index++] = radius * cos(angle);  // X
        vertices[index++] = -height / 2;          // Y (bottom circle)
        vertices[index++] = radius * sin(angle);  // Z

        vertices[index++] = radius * cos(angle);  // X
        vertices[index++] = height / 2;           // Y (top circle)
        vertices[index++] = radius * sin(angle);  // Z
    }
}

void generateCircle(float radius, int sectors, GLfloat* circleVertices, float y) {
    float sectorStep = 2 * glm::pi<float>() / sectors;
    int index = 0;
    for (int i = 0; i <= sectors; ++i) {
        float angle = i * sectorStep;
        circleVertices[index++] = radius * cos(angle);  // X
        circleVertices[index++] = y;                   // Y
        circleVertices[index++] = radius * sin(angle);  // Z
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
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD!" << std::endl;
        return -1;
    }

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);

    // Load shaders
    Shader shader("vertex_shader.glsl", "fragment_shader.glsl");


    // Setup cylinder vertices
    int sectors = 36;
    float radius = 0.7f;
    float height = 0.7f;

    int vertexCount = 2 * (sectors + 1);
    GLfloat* vertices = new GLfloat[vertexCount * 3];
    generateCylinder(radius, height, sectors, vertices);

    // Setup circle vertices
    GLfloat* topCircle = new GLfloat[sectors * 3];
    GLfloat* bottomCircle = new GLfloat[sectors * 3];
    generateCircle(radius, sectors, topCircle, height / 2);
    generateCircle(radius, sectors, bottomCircle, -height / 2);

    // Setup VAOs and VBOs
   // GLuint VBO[3], VAO[3];
   // glGenBuffers(3, VBO);
    //glGenVertexArrays(3, VAO);

    GLuint VBO, VAO;
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertexCount * 3 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glm::mat4 view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    // Cylinder lines
    //glDrawArrays(GL_LINES, 0, vertexCount);

    // Top circle
   // glDrawArrays(GL_LINE_LOOP, 0, sectors + 1);

    // Bottom circle
    //glDrawArrays(GL_LINE_LOOP, 0, sectors + 1);

    // Camera setup
    //glm::mat4 view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    //glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    // Main render loop
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();

        // Set uniform values
        glm::mat4 model = glm::mat4(1.0f);
        shader.setMat4("model", model);
        shader.setMat4("view", view);
        shader.setMat4("projection", projection);
        shader.setVec3("objectColor", glm::vec3(1.0f, 0.9f, 0.3f)); // Yellowish color

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, vertexCount);

        //glfwSwapBuffers(window);
        //glfwPollEvents();

        // Apply camera projection
      //  glMatrixMode(GL_PROJECTION);
        //glLoadMatrixf(glm::value_ptr(projection));

        //glMatrixMode(GL_MODELVIEW);
        //glLoadMatrixf(glm::value_ptr(view));

        // Draw cylinder
        //glBindVertexArray(VAO[0]);
        //glDrawArrays(GL_LINES, 0, vertexCount);

        // Draw top circle
        //glBindVertexArray(VAO[1]);
        //glDrawArrays(GL_LINE_LOOP, 0, sectors);

        // Draw bottom circle
        //glBindVertexArray(VAO[2]);
        //glDrawArrays(GL_LINE_LOOP, 0, sectors);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    delete[] vertices;
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}