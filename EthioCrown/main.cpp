#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "shader.hpp"

void checkGLError(const std::string& context)
{
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR)
    {
        std::cerr << "OpenGL Error at " << context << ": " << err << std::endl;
    }
}

void generateCylinder(float radiusOuter, float radiusInner, float height, int sectors, GLfloat* vertices)
{
    int index = 0;
    float sectorStep = 2 * glm::pi<float>() / sectors;

    // Outer Surface Vertices
    for (int i = 0; i <= sectors; ++i)
    {
        float angle = i * sectorStep;
        vertices[index++] = radiusOuter * cos(angle); // X (outer bottom)
        vertices[index++] = -height / 2;              // Y
        vertices[index++] = radiusOuter * sin(angle); // Z

        vertices[index++] = radiusOuter * cos(angle); // X (outer top)
        vertices[index++] = height / 2;               // Y
        vertices[index++] = radiusOuter * sin(angle); // Z
    }

    // Inner Surface Vertices
    for (int i = 0; i <= sectors; ++i)
    {
        float angle = i * sectorStep;
        vertices[index++] = radiusInner * cos(angle); // X (inner bottom)
        vertices[index++] = -height / 2;              // Y
        vertices[index++] = radiusInner * sin(angle); // Z

        vertices[index++] = radiusInner * cos(angle); // X (inner top)
        vertices[index++] = height / 2;               // Y
        vertices[index++] = radiusInner * sin(angle); // Z
    }
}

void generateCylinderNormals(float radiusOuter, float height, int sectors, GLfloat* normals)
{
    int index = 0;
    float sectorStep = 2 * glm::pi<float>() / sectors;
    for (int i = 0; i <= sectors; ++i)
    {
        float angle = i * sectorStep;
        normals[index++] = cos(angle); // X
        normals[index++] = 0.0f;       // Y
        normals[index++] = sin(angle); // Z

        normals[index++] = cos(angle); // X
        normals[index++] = 0.0f;       // Y
        normals[index++] = sin(angle); // Z
    }
}

void generateCircle(float radius, int sectors, GLfloat* circleVertices, float y)
{
    float sectorStep = 2 * glm::pi<float>() / sectors;
    int index = 0;
    for (int i = 0; i <= sectors; ++i)
    {
        float angle = i * sectorStep;
        circleVertices[index++] = radius * cos(angle); // X
        circleVertices[index++] = y;                   // Y
        circleVertices[index++] = radius * sin(angle); // Z
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

    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Cylinder with Thickness", NULL, NULL);
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
    std::cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

    // Load shaders
    Shader shader("vertex_shader.glsl", "fragment_shader.glsl");

    // Cylinder parameters
    int sectors = 36;
    float radiusOuter = 0.7f;
    float radiusInner = 0.5f; // Inner radius to add thickness
    float height = 0.7f;

    // Generate vertices
    int vertexCount = 4 * (sectors + 1);
    GLfloat* vertices = new GLfloat[vertexCount * 3];
    generateCylinder(radiusOuter, radiusInner, height, sectors, vertices);

    // Generate normals
    GLfloat* normals = new GLfloat[vertexCount * 3];
    generateCylinderNormals(radiusOuter, height, sectors, normals);

    // VAO and VBO Setup
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

    while (!glfwWindowShouldClose(window))
    {
        std::cout << "Rendering Frame..." << std::endl;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shader.use();
        checkGLError("After shader activation");

        glm::mat4 model = glm::mat4(1.0f);
        shader.setMat4("model", model);
        shader.setMat4("view", view);
        shader.setMat4("projection", projection);
        shader.setVec3("objectColor", glm::vec3(1.0f, 0.9f, 0.3f));
        shader.setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
        shader.setVec3("lightPos", glm::vec3(1.2f, 1.0f, 2.0f));
        shader.setVec3("viewPos", glm::vec3(2.0f, 2.0f, 2.0f));

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, vertexCount);
        checkGLError("Before draw call");

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
