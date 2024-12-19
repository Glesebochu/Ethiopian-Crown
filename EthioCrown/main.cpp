#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "fragment_shader.hpp"
#include "vertex_shader.hpp"

// Define the vertices of a cylinder
void generateCylinder(float radius, float height, int sectors, GLfloat* vertices, GLfloat* normals) {
    int index = 0, normalIndex = 0;
    float sectorStep = 2 * glm::pi<float>() / sectors;

    for (int i = 0; i < sectors; ++i) {
        float angle = i * sectorStep;
        float x = radius * cos(angle);
        float z = radius * sin(angle);

        // Bottom vertex
        vertices[index++] = x;
        vertices[index++] = -height / 2;
        vertices[index++] = z;

        // Top vertex
        vertices[index++] = x;
        vertices[index++] = height / 2;
        vertices[index++] = z;

        // Normals (cylinder side)
        normals[normalIndex++] = x;
        normals[normalIndex++] = 0.0f;
        normals[normalIndex++] = z;

        normals[normalIndex++] = x;
        normals[normalIndex++] = 0.0f;
        normals[normalIndex++] = z;
    }
}

void generateCircle(float radius, int sectors, GLfloat* circleVertices, float y) {
    float sectorStep = 2 * glm::pi<float>() / sectors;
    int index = 0;
    for (int i = 0; i < sectors; ++i) {
        float angle = i * sectorStep;
        circleVertices[index++] = radius * cos(angle);  // X
        circleVertices[index++] = y;                   // Y
        circleVertices[index++] = radius * sin(angle);  // Z
    }
}

GLuint createShader(const char* source, GLenum shaderType) {
    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cerr << "Error: Shader compilation failed\n" << infoLog << std::endl;
    }
    return shader;
}

GLuint createProgram(const char* vertexSource, const char* fragmentSource) {
    GLuint vertexShader = createShader(vertexSource, GL_VERTEX_SHADER);
    GLuint fragmentShader = createShader(fragmentSource, GL_FRAGMENT_SHADER);

    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return program;
}

int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW!" << std::endl;
        return -1;
    }

    // Create a window
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

    // Cylinder setup
    int sectors = 36;
    float radius = 0.7f;
    float height = 0.7f;

    int vertexCount = 2 * sectors;
    GLfloat* vertices = new GLfloat[vertexCount * 3];
    GLfloat* normals = new GLfloat[vertexCount * 3];
    generateCylinder(radius, height, sectors, vertices, normals);

    GLfloat* topCircle = new GLfloat[sectors * 3];
    GLfloat* bottomCircle = new GLfloat[sectors * 3];
    generateCircle(radius, sectors, topCircle, height / 2);
    generateCircle(radius, sectors, bottomCircle, -height / 2);

    GLuint VBO[3], VAO[3];
    glGenBuffers(3, VBO);
    glGenVertexArrays(3, VAO);

    // Cylinder VAO
    glBindVertexArray(VAO[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, vertexCount * 3 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, vertexCount * 3 * sizeof(GLfloat), normals, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(1);

    // Top circle VAO
    glBindVertexArray(VAO[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sectors * 3 * sizeof(GLfloat), topCircle, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // Bottom circle VAO
    glBindVertexArray(VAO[2]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
    glBufferData(GL_ARRAY_BUFFER, sectors * 3 * sizeof(GLfloat), bottomCircle, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    GLuint shaderProgram = createProgram(vertexShaderSource, fragmentShaderSource);

    glm::mat4 view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glm::mat4 model = glm::mat4(1.0f);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

        // Draw cylinder
        glBindVertexArray(VAO[0]);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, vertexCount);

        // Draw top circle
        glBindVertexArray(VAO[1]);
        glDrawArrays(GL_LINE_LOOP, 0, sectors);

        // Draw bottom circle
        glBindVertexArray(VAO[2]);
        glDrawArrays(GL_LINE_LOOP, 0, sectors);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    delete[] vertices;
    delete[] normals;
    delete[] topCircle;
    delete[] bottomCircle;
    glDeleteBuffers(3, VBO);
    glDeleteVertexArrays(3, VAO);
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
