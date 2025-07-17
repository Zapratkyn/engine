#include "../engine_include/VisualHelper.hpp"

VisualHelper::VisualHelper(GLuint shader) : shader(shader)
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
    glGenVertexArrays(2, lines);
    glGenBuffers(2, VBO);

    glBindVertexArray(lines[0]);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(xAxis), xAxis, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(lines[1]);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(yAxis), yAxis, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

void VisualHelper::toggleDisplay()
{
	if (!vh->isDisplayed())
		display = true;
}

bool VisualHelper::isDisplayed() { return display; }