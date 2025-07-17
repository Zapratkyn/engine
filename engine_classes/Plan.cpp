#include "../engine_classes/Plan.hpp"

Plan::Plan(GLuint shader) : shader(shader)
{
	float xAxis[] = {
        -1.0f,  0.0f,  // Point 1
         1.0f,  0.0f   // Point 2
    };
    float yAxis[] = {
         0.0f,  -1.0f,  // Point 1
         0.0f,   1.0f   // Point 2
    };

    // Création du VBO et VAO
    unsigned int VBO[2];
    glGenVertexArrays(2, axes);
    glGenBuffers(2, VBO);

    glBindVertexArray(axes[0]);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(xAxis), xAxis, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(axes[1]);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(yAxis), yAxis, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

void render()
{
    glBindVertexArray(axes[0]);
    glDrawArrays(GL_LINES, 0, 2);
    glBindVertexArray(axes[1]);
    glDrawArrays(GL_LINES, 0, 2);
}