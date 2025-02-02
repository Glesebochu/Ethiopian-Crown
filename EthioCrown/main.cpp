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
#include "main.h"

// Define pi manually
const float PI = 3.14159265359f;

// Window settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// Cylinder settings
const int SECTORS = 36;
const float OUTER_RADIUS = 1.5f;
const float INNER_RADIUS = 1.3f;
const float HEIGHT = 1.5f;

// Cross settings
const float CROSS_WIDTH = 0.4f;
const float CROSS_HEIGHT = 0.65f;
const float CROSS_THICKNESS = 0.2f;

// Spike settings
const float SPIKE_WIDTH = 0.5f;
const float SPIKE_HEIGHT = 0.7f;
const float SPIKE_THICKNESS = 0.1f;

// Spike1 settings
const float SPIKE_WIDTH1 = 0.4f;
const float SPIKE_HEIGHT1 = 0.7f;
const float SPIKE_THICKNESS1 = 0.08f;

// Spike2 settings
const float SPIKE_WIDTH2 = 0.4f;
const float SPIKE_HEIGHT2 = 0.7f;
const float SPIKE_THICKNESS2 = 0.08f;

// Spike3 settings
const float SPIKE_WIDTH3 = 0.5f;
const float SPIKE_HEIGHT3 = 0.4f;
const float SPIKE_THICKNESS3 = 0.1f;

// Spike4 settings
const float SPIKE_WIDTH4 = 0.4f;
const float SPIKE_HEIGHT4 = 0.4f;
const float SPIKE_THICKNESS4 = 0.1f;

// Spike5 settings
const float SPIKE_WIDTH5 = 0.4f;
const float SPIKE_HEIGHT5 = 0.4f;
const float SPIKE_THICKNESS5 = 0.08f;

// Spike6 settings
const float SPIKE_WIDTH6 = 0.4f;
const float SPIKE_HEIGHT6 = 0.4f;
const float SPIKE_THICKNESS6 = 0.08f;

// Function to generate the hollow cylinder vertices and indices
void generateHollowCylinder(float radiusOuter, float radiusInner, float height, int sectors,
    std::vector<GLfloat>& vertices, std::vector<GLuint>& outerIndices,
    std::vector<GLuint>& innerIndices, std::vector<GLuint>& topCapIndices,
    std::vector<GLuint>& bottomCapIndices)
{
    float sectorStep = 2 * PI / sectors;

    for (int i = 0; i <= sectors; ++i)
    {
        float angle = i * sectorStep;
        float u = static_cast<float>(i) / sectors;

        // Outer top
        vertices.push_back(radiusOuter * cos(angle));
        vertices.push_back(height / 2);
        vertices.push_back(radiusOuter * sin(angle));
        vertices.push_back(u); // Texture X
        vertices.push_back(1.0f); // Texture Y

        // Outer bottom
        vertices.push_back(radiusOuter * cos(angle));
        vertices.push_back(-height / 2);
        vertices.push_back(radiusOuter * sin(angle));
        vertices.push_back(u); // Texture X
        vertices.push_back(0.0f); // Texture Y

        // Inner top
        vertices.push_back(radiusInner * cos(angle));
        vertices.push_back(height / 2);
        vertices.push_back(radiusInner * sin(angle));
        vertices.push_back(u); // Texture X
        vertices.push_back(1.0f); // Texture Y

        // Inner bottom
        vertices.push_back(radiusInner * cos(angle));
        vertices.push_back(-height / 2);
        vertices.push_back(radiusInner * sin(angle));
        vertices.push_back(u); // Texture X
        vertices.push_back(0.0f); // Texture Y
    }

    // Generate indices for outer and inner surfaces
    for (int i = 0; i < sectors; ++i)
    {
        int current = i * 4;
        int next = ((i + 1) % sectors) * 4;

        // Outer surface
        outerIndices.push_back(current);
        outerIndices.push_back(next);
        outerIndices.push_back(current + 1);
        outerIndices.push_back(next);
        outerIndices.push_back(next + 1);
        outerIndices.push_back(current + 1);

        // Inner surface
        innerIndices.push_back(current + 2);
        innerIndices.push_back(next + 2);
        innerIndices.push_back(current + 3);
        innerIndices.push_back(next + 2);
        innerIndices.push_back(next + 3);
        innerIndices.push_back(current + 3);

        // Top Cap (connect inner and outer top edges)
        topCapIndices.push_back(current);
        topCapIndices.push_back(next);
        topCapIndices.push_back(current + 2);
        topCapIndices.push_back(next);
        topCapIndices.push_back(next + 2);
        topCapIndices.push_back(current + 2);

        // Bottom Cap (connect inner and outer bottom edges)
        bottomCapIndices.push_back(current + 1);
        bottomCapIndices.push_back(next + 1);
        bottomCapIndices.push_back(current + 3);
        bottomCapIndices.push_back(next + 1);
        bottomCapIndices.push_back(next + 3);
        bottomCapIndices.push_back(current + 3);
    }
}

// Function to generate the cross vertices and indices
void generateCross(float width, float height, float thickness, std::vector<GLfloat>& vertices, std::vector<GLuint>& indices)
{
    // Define the cross as two rectangles (vertical and horizontal)
    float halfWidth = width / 2.0;
    float halfHeight = height / 2.0;
    float halfThickness = thickness / 3;

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
// Function to generate the spike vertices and indices
void generateSpike(float width, float height, float thickness,
    std::vector<GLfloat>& vertices, std::vector<GLuint>& indices)
{
    float halfWidth = width / 9;
    float halfHeight = height / 3.0;
    float halfThickness = thickness; // Keep full thickness

    // Define 8 vertices for a 3D spike
    vertices = {
        // Front face
        -halfThickness, -halfHeight,  halfThickness,  0.0f, 0.0f,  // 0 - Bottom Left Front
         halfThickness, -halfHeight,  halfThickness,  1.0f, 0.0f,  // 1 - Bottom Right Front
         0.0f,          halfHeight,   halfThickness,  0.5f, 1.0f,  // 2 - Top Front

         // Back face
         -halfThickness, -halfHeight, -halfThickness,  0.0f, 0.0f,  // 3 - Bottom Left Back
          halfThickness, -halfHeight, -halfThickness,  1.0f, 0.0f,  // 4 - Bottom Right Back
          0.0f,          halfHeight,  -halfThickness,  0.5f, 1.0f,  // 5 - Top Back
    };

    // Define indices for 3D spike
    indices = {
        // Front face
        0, 1, 2,

        // Back face
        3, 5, 4,

        // Left side
        0, 2, 3,
        3, 2, 5,

        // Right side
        1, 4, 2,
        2, 4, 5,

        // Bottom face
        0, 3, 1,
        1, 3, 4
    };
}

void generateSpike1(float width, float height, float thickness,
    std::vector<GLfloat>& vertices, std::vector<GLuint>& indices)
{
    float halfWidth = width / 9;
    float halfHeight = height / 3.0;
    float halfThickness = thickness; // Keep full thickness

    // Define 8 vertices for a 3D spike
    vertices = {
        // Front face
        -halfThickness, -halfHeight,  halfThickness,  0.0f, 0.0f,  // 0 - Bottom Left Front
         halfThickness, -halfHeight,  halfThickness,  1.0f, 0.0f,  // 1 - Bottom Right Front
         0.0f,          halfHeight,   halfThickness,  0.5f, 1.0f,  // 2 - Top Front

         // Back face
         -halfThickness, -halfHeight, -halfThickness,  0.0f, 0.0f,  // 3 - Bottom Left Back
          halfThickness, -halfHeight, -halfThickness,  1.0f, 0.0f,  // 4 - Bottom Right Back
          0.0f,          halfHeight,  -halfThickness,  0.5f, 1.0f,  // 5 - Top Back
    };

    // Define indices for 3D spike
    indices = {
        // Front face
        0, 1, 2,

        // Back face
        3, 5, 4,

        // Left side
        0, 2, 3,
        3, 2, 5,

        // Right side
        1, 4, 2,
        2, 4, 5,

        // Bottom face
        0, 3, 1,
        1, 3, 4
    };
}

void generateSpike2(float width, float height, float thickness,
    std::vector<GLfloat>& vertices, std::vector<GLuint>& indices)
{
    float halfWidth = width / 9;
    float halfHeight = height / 3.0;
    float halfThickness = thickness; // Keep full thickness

    // Define 8 vertices for a 3D spike
    vertices = {
        // Front face
        -halfThickness, -halfHeight,  halfThickness,  0.0f, 0.0f,  // 0 - Bottom Left Front
         halfThickness, -halfHeight,  halfThickness,  1.0f, 0.0f,  // 1 - Bottom Right Front
         0.0f,          halfHeight,   halfThickness,  0.5f, 1.0f,  // 2 - Top Front

         // Back face
         -halfThickness, -halfHeight, -halfThickness,  0.0f, 0.0f,  // 3 - Bottom Left Back
          halfThickness, -halfHeight, -halfThickness,  1.0f, 0.0f,  // 4 - Bottom Right Back
          0.0f,          halfHeight,  -halfThickness,  0.5f, 1.0f,  // 5 - Top Back
    };

    // Define indices for 3D spike
    indices = {
        // Front face
        0, 1, 2,

        // Back face
        3, 5, 4,

        // Left side
        0, 2, 3,
        3, 2, 5,

        // Right side
        1, 4, 2,
        2, 4, 5,

        // Bottom face
        0, 3, 1,
        1, 3, 4
    };
}
void generateSpike3(float width, float height, float thickness,
    std::vector<GLfloat>& vertices, std::vector<GLuint>& indices)
{
    float halfWidth = width / 9;
    float halfHeight = height / 3.0;
    float halfThickness = thickness; // Keep full thickness

    // Define 8 vertices for a 3D spike
    vertices = {
        // Front face
        -halfThickness, -halfHeight,  halfThickness,  0.0f, 0.0f,  // 0 - Bottom Left Front
         halfThickness, -halfHeight,  halfThickness,  1.0f, 0.0f,  // 1 - Bottom Right Front
         0.0f,          halfHeight,   halfThickness,  0.5f, 1.0f,  // 2 - Top Front

         // Back face
         -halfThickness, -halfHeight, -halfThickness,  0.0f, 0.0f,  // 3 - Bottom Left Back
          halfThickness, -halfHeight, -halfThickness,  1.0f, 0.0f,  // 4 - Bottom Right Back
          0.0f,          halfHeight,  -halfThickness,  0.5f, 1.0f,  // 5 - Top Back
    };

    // Define indices for 3D spike
    indices = {
        // Front face
        0, 1, 2,

        // Back face
        3, 5, 4,

        // Left side
        0, 2, 3,
        3, 2, 5,

        // Right side
        1, 4, 2,
        2, 4, 5,

        // Bottom face
        0, 3, 1,
        1, 3, 4
    };
}
void generateSpike4(float width, float height, float thickness,
    std::vector<GLfloat>& vertices, std::vector<GLuint>& indices)
{
    float halfWidth = width / 9;
    float halfHeight = height / 3.0;
    float halfThickness = thickness; // Keep full thickness

    // Define 8 vertices for a 3D spike
    vertices = {
        // Front face
        -halfThickness, -halfHeight,  halfThickness,  0.0f, 0.0f,  // 0 - Bottom Left Front
         halfThickness, -halfHeight,  halfThickness,  1.0f, 0.0f,  // 1 - Bottom Right Front
         0.0f,          halfHeight,   halfThickness,  0.5f, 1.0f,  // 2 - Top Front

         // Back face
         -halfThickness, -halfHeight, -halfThickness,  0.0f, 0.0f,  // 3 - Bottom Left Back
          halfThickness, -halfHeight, -halfThickness,  1.0f, 0.0f,  // 4 - Bottom Right Back
          0.0f,          halfHeight,  -halfThickness,  0.5f, 1.0f,  // 5 - Top Back
    };

    // Define indices for 3D spike
    indices = {
        // Front face
        0, 1, 2,

        // Back face
        3, 5, 4,

        // Left side
        0, 2, 3,
        3, 2, 5,

        // Right side
        1, 4, 2,
        2, 4, 5,

        // Bottom face
        0, 3, 1,
        1, 3, 4
    };
}
void generateSpike5(float width, float height, float thickness,
    std::vector<GLfloat>& vertices, std::vector<GLuint>& indices)
{
    float halfWidth = width / 9;
    float halfHeight = height / 3.0;
    float halfThickness = thickness; // Keep full thickness

    // Define 8 vertices for a 3D spike
    vertices = {
        // Front face
        -halfThickness, -halfHeight,  halfThickness,  0.0f, 0.0f,  // 0 - Bottom Left Front
         halfThickness, -halfHeight,  halfThickness,  1.0f, 0.0f,  // 1 - Bottom Right Front
         0.0f,          halfHeight,   halfThickness,  0.5f, 1.0f,  // 2 - Top Front

         // Back face
         -halfThickness, -halfHeight, -halfThickness,  0.0f, 0.0f,  // 3 - Bottom Left Back
          halfThickness, -halfHeight, -halfThickness,  1.0f, 0.0f,  // 4 - Bottom Right Back
          0.0f,          halfHeight,  -halfThickness,  0.5f, 1.0f,  // 5 - Top Back
    };

    // Define indices for 3D spike
    indices = {
        // Front face
        0, 1, 2,

        // Back face
        3, 5, 4,

        // Left side
        0, 2, 3,
        3, 2, 5,

        // Right side
        1, 4, 2,
        2, 4, 5,

        // Bottom face
        0, 3, 1,
        1, 3, 4
    };
}
void generateSpike6(float width, float height, float thickness,
    std::vector<GLfloat>& vertices, std::vector<GLuint>& indices)
{
    float halfWidth = width / 9;
    float halfHeight = height / 3.0;
    float halfThickness = thickness; // Keep full thickness

    // Define 8 vertices for a 3D spike
    vertices = {
        // Front face
        -halfThickness, -halfHeight,  halfThickness,  0.0f, 0.0f,  // 0 - Bottom Left Front
         halfThickness, -halfHeight,  halfThickness,  1.0f, 0.0f,  // 1 - Bottom Right Front
         0.0f,          halfHeight,   halfThickness,  0.5f, 1.0f,  // 2 - Top Front

         // Back face
         -halfThickness, -halfHeight, -halfThickness,  0.0f, 0.0f,  // 3 - Bottom Left Back
          halfThickness, -halfHeight, -halfThickness,  1.0f, 0.0f,  // 4 - Bottom Right Back
          0.0f,          halfHeight,  -halfThickness,  0.5f, 1.0f,  // 5 - Top Back
    };

    // Define indices for 3D spike
    indices = {
        // Front face
        0, 1, 2,

        // Back face
        3, 5, 4,

        // Left side
        0, 2, 3,
        3, 2, 5,

        // Right side
        1, 4, 2,
        2, 4, 5,

        // Bottom face
        0, 3, 1,
        1, 3, 4
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

    // Load shaders
    Shader shader("vertex_shader.glsl", "fragment_shader.glsl");

    // Generate cylinder
    std::vector<GLfloat> vertices;
    std::vector<GLuint> outerIndices, innerIndices, topCapIndices, bottomCapIndices;
    generateHollowCylinder(OUTER_RADIUS, INNER_RADIUS, HEIGHT, SECTORS, vertices, outerIndices, innerIndices, topCapIndices, bottomCapIndices);

    // Generate cross
    std::vector<GLfloat> crossVertices;
    std::vector<GLuint> crossIndices;
    generateCross(CROSS_WIDTH, CROSS_HEIGHT, CROSS_THICKNESS, crossVertices, crossIndices);

    // Generate spike
    std::vector<GLfloat> spikeVertices;
    std::vector<GLuint> spikeIndices;
    generateSpike(SPIKE_WIDTH, SPIKE_HEIGHT, SPIKE_THICKNESS, spikeVertices, spikeIndices);
    
    // Generate spike1
    std::vector<GLfloat> spike1Vertices;
    std::vector<GLuint> spike1Indices;
    generateSpike1(SPIKE_WIDTH1, SPIKE_HEIGHT1, SPIKE_THICKNESS1, spike1Vertices, spike1Indices);

    // Generate spike2
    std::vector<GLfloat> spike2Vertices;
    std::vector<GLuint> spike2Indices;
    generateSpike2(SPIKE_WIDTH2, SPIKE_HEIGHT2, SPIKE_THICKNESS2, spike2Vertices, spike2Indices);

    // Generate spike3
    std::vector<GLfloat> spike3Vertices;
    std::vector<GLuint> spike3Indices;
    generateSpike3(SPIKE_WIDTH3, SPIKE_HEIGHT3, SPIKE_THICKNESS3, spike3Vertices, spike3Indices);

    // Generate spike4
    std::vector<GLfloat> spike4Vertices;
    std::vector<GLuint> spike4Indices;
    generateSpike4(SPIKE_WIDTH4, SPIKE_HEIGHT4, SPIKE_THICKNESS4, spike4Vertices, spike4Indices);

    // Generate spike5
    std::vector<GLfloat> spike5Vertices;
    std::vector<GLuint> spike5Indices;
    generateSpike5(SPIKE_WIDTH5, SPIKE_HEIGHT5, SPIKE_THICKNESS5, spike5Vertices, spike5Indices);

    // Generate spike6
    std::vector<GLfloat> spike6Vertices;
    std::vector<GLuint> spike6Indices;
    generateSpike6(SPIKE_WIDTH6, SPIKE_HEIGHT6, SPIKE_THICKNESS6, spike6Vertices, spike6Indices);

    // Set up VAO, VBO, and EBOs for cylinder
    GLuint VAO, VBO, outerEBO, innerEBO, topEBO, bottomEBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &outerEBO);
    glGenBuffers(1, &innerEBO);
    glGenBuffers(1, &topEBO);
    glGenBuffers(1, &bottomEBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, outerEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, outerIndices.size() * sizeof(GLuint), outerIndices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, innerEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, innerIndices.size() * sizeof(GLuint), innerIndices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, topEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, topCapIndices.size() * sizeof(GLuint), topCapIndices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bottomEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, bottomCapIndices.size() * sizeof(GLuint), bottomCapIndices.data(), GL_STATIC_DRAW);

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

    // Set up VAO, VBO, and EBO for spike
    GLuint spikeVAO, spikeVBO, spikeEBO;
    glGenVertexArrays(1, &spikeVAO);
    glGenBuffers(1, &spikeVBO);
    glGenBuffers(1, &spikeEBO);

    glBindVertexArray(spikeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, spikeVBO);
    glBufferData(GL_ARRAY_BUFFER, spikeVertices.size() * sizeof(GLfloat), spikeVertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, spikeEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, spikeIndices.size() * sizeof(GLuint), spikeIndices.data(), GL_STATIC_DRAW);

    // Vertex attributes (x, y, z, u, v)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Set up VAO, VBO, and EBO for spike1
    GLuint spike1VAO, spike1VBO, spike1EBO;
    glGenVertexArrays(1, &spike1VAO);
    glGenBuffers(1, &spike1VBO);
    glGenBuffers(1, &spike1EBO);

    glBindVertexArray(spike1VAO);

    glBindBuffer(GL_ARRAY_BUFFER, spike1VBO);
    glBufferData(GL_ARRAY_BUFFER, spike1Vertices.size() * sizeof(GLfloat), spike1Vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, spike1EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, spike1Indices.size() * sizeof(GLuint), spike1Indices.data(), GL_STATIC_DRAW);

    // Vertex attributes (x, y, z, u, v)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Set up VAO, VBO, and EBO for spike2
    GLuint spike2VAO, spike2VBO, spike2EBO;
    glGenVertexArrays(1, &spike2VAO);
    glGenBuffers(1, &spike2VBO);
    glGenBuffers(1, &spike2EBO);

    glBindVertexArray(spike2VAO);

    glBindBuffer(GL_ARRAY_BUFFER, spike2VBO);
    glBufferData(GL_ARRAY_BUFFER, spike2Vertices.size() * sizeof(GLfloat), spike2Vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, spike2EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, spike2Indices.size() * sizeof(GLuint), spike2Indices.data(), GL_STATIC_DRAW);

    // Vertex attributes (x, y, z, u, v)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Set up VAO, VBO, and EBO for spike3
    GLuint spike3VAO, spike3VBO, spike3EBO;
    glGenVertexArrays(1, &spike3VAO);
    glGenBuffers(1, &spike3VBO);
    glGenBuffers(1, &spike3EBO);

    glBindVertexArray(spike3VAO);

    glBindBuffer(GL_ARRAY_BUFFER, spike3VBO);
    glBufferData(GL_ARRAY_BUFFER, spike3Vertices.size() * sizeof(GLfloat), spike3Vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, spike3EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, spike3Indices.size() * sizeof(GLuint), spike3Indices.data(), GL_STATIC_DRAW);

    // Vertex attributes (x, y, z, u, v)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Set up VAO, VBO, and EBO for 4
    GLuint spike4VAO, spike4VBO, spike4EBO;
    glGenVertexArrays(1, &spike4VAO);
    glGenBuffers(1, &spike4VBO);
    glGenBuffers(1, &spike4EBO);

    glBindVertexArray(spike4VAO);

    glBindBuffer(GL_ARRAY_BUFFER, spike4VBO);
    glBufferData(GL_ARRAY_BUFFER, spike4Vertices.size() * sizeof(GLfloat), spike4Vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, spike4EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, spike4Indices.size() * sizeof(GLuint), spike4Indices.data(), GL_STATIC_DRAW);

    // Vertex attributes (x, y, z, u, v)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Set up VAO, VBO, and EBO for spike5
    GLuint spike5VAO, spike5VBO, spike5EBO;
    glGenVertexArrays(1, &spike5VAO);
    glGenBuffers(1, &spike5VBO);
    glGenBuffers(1, &spike5EBO);

    glBindVertexArray(spike5VAO);

    glBindBuffer(GL_ARRAY_BUFFER, spike5VBO);
    glBufferData(GL_ARRAY_BUFFER, spike5Vertices.size() * sizeof(GLfloat), spike5Vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, spike5EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, spike5Indices.size() * sizeof(GLuint), spike5Indices.data(), GL_STATIC_DRAW);

    // Vertex attributes (x, y, z, u, v)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Set up VAO, VBO, and EBO for spike6
    GLuint spike6VAO, spike6VBO, spike6EBO;
    glGenVertexArrays(1, &spike6VAO);
    glGenBuffers(1, &spike6VBO);
    glGenBuffers(1, &spike6EBO);

    glBindVertexArray(spike6VAO);

    glBindBuffer(GL_ARRAY_BUFFER, spike6VBO);
    glBufferData(GL_ARRAY_BUFFER, spike6Vertices.size() * sizeof(GLfloat), spike6Vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, spike6EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, spike6Indices.size() * sizeof(GLuint), spike6Indices.data(), GL_STATIC_DRAW);

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
    unsigned char* data = stbi_load("cylinder.jpg", &width, &height, &nrChannels, 0);
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

    data = stbi_load("cylinder.jpg", &width, &height, &nrChannels, 0);
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

    data = stbi_load("spikes.jfif", &width, &height, &nrChannels, 0);
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

    // Load spike texture
    GLuint spikeTexture;
    glGenTextures(1, &spikeTexture);
    glBindTexture(GL_TEXTURE_2D, spikeTexture);

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    data = stbi_load("spikes.jfif", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cerr << "Failed to load spike texture!" << std::endl;
    }
    stbi_image_free(data);

    // Load spike1 texture
    GLuint spike1Texture;
    glGenTextures(1, &spike1Texture);
    glBindTexture(GL_TEXTURE_2D, spike1Texture);

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    data = stbi_load("spikes.jfif", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cerr << "Failed to load spike texture!" << std::endl;
    }
    stbi_image_free(data);

    // Load spike2 texture
    GLuint spike2Texture;
    glGenTextures(1, &spike2Texture);
    glBindTexture(GL_TEXTURE_2D, spike2Texture);

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    data = stbi_load("spikes.jfif", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cerr << "Failed to load spike2 texture!" << std::endl;
    }
    stbi_image_free(data);

    // Load spike3 texture
    GLuint spike3Texture;
    glGenTextures(1, &spike3Texture);
    glBindTexture(GL_TEXTURE_2D, spike3Texture);

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    data = stbi_load("spikes.jfif", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cerr << "Failed to load spike2 texture!" << std::endl;
    }
    stbi_image_free(data);

    // Load spike4 texture
    GLuint spike4Texture;
    glGenTextures(1, &spike4Texture);
    glBindTexture(GL_TEXTURE_2D, spike4Texture);

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    data = stbi_load("spikes.jfif", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cerr << "Failed to load spike2 texture!" << std::endl;
    }
    stbi_image_free(data);

    // Load spike5 texture
    GLuint spike5Texture;
    glGenTextures(1, &spike5Texture);
    glBindTexture(GL_TEXTURE_2D, spike5Texture);

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    data = stbi_load("spikes.jfif", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cerr << "Failed to load spike2 texture!" << std::endl;
    }
    stbi_image_free(data);

    // Load spike6 texture
    GLuint spike6Texture;
    glGenTextures(1, &spike6Texture);
    glBindTexture(GL_TEXTURE_2D, spike6Texture);

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    data = stbi_load("spikes.jfif", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cerr << "Failed to load spike2 texture!" << std::endl;
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

        // Set transformation matrices
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 4.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        shader.setMat4("model", model);
        shader.setMat4("view", view);
        shader.setMat4("projection", projection);

        // Set light properties
        shader.setVec3("lightPos", glm::vec3(0.0f, 5.0f, 8.0f));  // A bit lower and forward
        shader.setVec3("lightDir", glm::normalize(glm::vec3(0.0f, -0.8f, -1.0f)));

        // Make the beam sharper
        shader.setFloat("cutOff", glm::cos(glm::radians(8.0f)));  // Wider light
        shader.setFloat("outerCutOff", glm::cos(glm::radians(11.0f))); // Soft edge

        // Reduce brightness to match reference
        shader.setVec3("lightColor", glm::vec3(1.5f, 1.5f, 1.2f));  // Softer warm light





        // Draw outer surface
        glBindTexture(GL_TEXTURE_2D, outerTexture);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, outerEBO);
        glDrawElements(GL_TRIANGLES, outerIndices.size(), GL_UNSIGNED_INT, 0);

        // Draw inner surface
        glBindTexture(GL_TEXTURE_2D, innerTexture);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, innerEBO);
        glDrawElements(GL_TRIANGLES, innerIndices.size(), GL_UNSIGNED_INT, 0);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, outerEBO);
        glDrawElements(GL_TRIANGLES, outerIndices.size(), GL_UNSIGNED_INT, 0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, innerEBO);
        glDrawElements(GL_TRIANGLES, innerIndices.size(), GL_UNSIGNED_INT, 0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, topEBO);
        glDrawElements(GL_TRIANGLES, topCapIndices.size(), GL_UNSIGNED_INT, 0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bottomEBO);
        glDrawElements(GL_TRIANGLES, bottomCapIndices.size(), GL_UNSIGNED_INT, 0);

        // Pass the updated model matrix to the shader
        glm::mat4 crossModel = glm::translate(model, glm::vec3(0.0f, HEIGHT / 2 + CROSS_HEIGHT / 1.5 + 0.55, 2.12f));
        shader.setMat4("model", crossModel);
        glBindTexture(GL_TEXTURE_2D, crossTexture);
        glBindVertexArray(crossVAO);
        glDrawElements(GL_TRIANGLES, crossIndices.size(), GL_UNSIGNED_INT, 0);

        // Pass the updated model matrix to the shader for spike(back spike)
        glm::mat4 spikeModel = glm::translate(model, glm::vec3(0.0f, HEIGHT / 2.5 + SPIKE_HEIGHT / 1.5 + 0.55, -0.2f));
        shader.setMat4("model", spikeModel);
        glBindTexture(GL_TEXTURE_2D, spikeTexture);
        glBindVertexArray(spikeVAO);
        glDrawElements(GL_TRIANGLES, spikeIndices.size(), GL_UNSIGNED_INT, 0);

        // Pass the updated model matrix to the shader for spike 1
        glm::mat4 spike1Model = glm::translate(model, glm::vec3(1.099f, HEIGHT / 2.5 + SPIKE_HEIGHT1 / 1.5 + 0.55, 1.15f));
        shader.setMat4("model", spike1Model);
        glBindTexture(GL_TEXTURE_2D, spike1Texture);
        glBindVertexArray(spike1VAO);
        glDrawElements(GL_TRIANGLES, spike1Indices.size(), GL_UNSIGNED_INT, 0);

        // Pass the updated model matrix to the shader for spike 2
        glm::mat4 spike2Model = glm::translate(model, glm::vec3(-1.099f, HEIGHT / 2.5 + SPIKE_HEIGHT1 / 1.5 + 0.55, 1.15f));
        shader.setMat4("model", spike2Model);
        glBindTexture(GL_TEXTURE_2D, spike2Texture);
        glBindVertexArray(spike2VAO);
        glDrawElements(GL_TRIANGLES, spike2Indices.size(), GL_UNSIGNED_INT, 0);

        // Pass the updated model matrix to the shader for spike 3(Left center)
        glm::mat4 spike3Model = glm::translate(model, glm::vec3(-0.7f, HEIGHT / 2.5 + SPIKE_HEIGHT1 / 1.5 + 0.55, 0.3f));
        spike3Model = glm::rotate(spike3Model, glm::radians(30.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        shader.setMat4("model", spike3Model);
        glBindTexture(GL_TEXTURE_2D, spike3Texture);
        glBindVertexArray(spike3VAO);
        glDrawElements(GL_TRIANGLES, spike3Indices.size(), GL_UNSIGNED_INT, 0);

        // Pass the updated model matrix to the shader for spike(Right center)
        glm::mat4 spike4Model = glm::translate(model, glm::vec3(0.7f, HEIGHT / 2.5 + SPIKE_HEIGHT1 / 1.5 + 0.55, 0.3f));
        spike4Model = glm::rotate(spike4Model, glm::radians(30.0f), glm::vec3(0.0f, -1.0f, 0.0f));
        shader.setMat4("model", spike4Model);
        glBindTexture(GL_TEXTURE_2D, spike4Texture);
        glBindVertexArray(spike4VAO);
        glDrawElements(GL_TRIANGLES, spike4Indices.size(), GL_UNSIGNED_INT, 0);

        // Pass the updated model matrix to the shader for spike 5
        glm::mat4 spike5Model = glm::translate(model, glm::vec3(-0.65f, HEIGHT / 2.5 + SPIKE_HEIGHT1 / 1.5 + 0.55, 2.0f));
        spike5Model = glm::rotate(spike5Model, glm::radians(30.0f), glm::vec3(0.0f, -1.0f, 0.0f));
        shader.setMat4("model", spike5Model);
        glBindTexture(GL_TEXTURE_2D, spike5Texture);
        glBindVertexArray(spike5VAO);
        glDrawElements(GL_TRIANGLES, spike5Indices.size(), GL_UNSIGNED_INT, 0);

        // Pass the updated model matrix to the shader for spike 6

        glm::mat4 spike6Model = glm::translate(model, glm::vec3(0.6f, HEIGHT / 2.5 + SPIKE_HEIGHT1 / 1.5 + 0.55, 2.05));
        spike6Model = glm::rotate(spike6Model, glm::radians(30.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        shader.setMat4("model", spike6Model);
        glBindTexture(GL_TEXTURE_2D, spike6Texture);
        glBindVertexArray(spike6VAO);
        glDrawElements(GL_TRIANGLES, spike6Indices.size(), GL_UNSIGNED_INT, 0);

        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &outerEBO);
    glDeleteBuffers(1, &innerEBO);
    glDeleteVertexArrays(1, &crossVAO);
    glDeleteBuffers(1, &crossVBO);
    glDeleteBuffers(1, &crossEBO);
    glDeleteVertexArrays(1, &spikeVAO);
    glDeleteBuffers(1, &spikeVBO);
    glDeleteBuffers(1, &spikeEBO);
    glDeleteVertexArrays(1, &spike1VAO);
    glDeleteBuffers(1, &spike1VBO);
    glDeleteBuffers(1, &spike1EBO);
    glfwTerminate();

    return 0;
}