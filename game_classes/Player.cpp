#include "../game_include/Player.hpp"

Player::Player(GLuint objectShader, GLuint hitboxShader) : crawl(false), position(glm::vec3(-0.65f, 0.0f, 0.0f)), shader(objectShader), hitboxShader(hitboxShader), showHitbox(false), frameCount(0)
{
	makeObject();
	makeHitbox();
    positionLoc = glGetUniformLocation(shader, "position");
    hitboxPositionLoc = glGetUniformLocation(hitboxShader, "position");
    hitboxColorLoc = glGetUniformLocation(hitboxShader, "Color");
}

void Player::makeObject()
{
	int texWidth, texHeight, texChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load("assets/dino_sprite.png", &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);

    GLuint tex;
    glGenTextures(1, &tex);
    texture = tex;
    glBindTexture(GL_TEXTURE_2D, texture);

    // Paramètres texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texWidth, texHeight, 0,
                     GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } 
    else
        std::cerr << "ERROR::PLAYER::TEXTURE::LOADING_FAILED\n";
    stbi_image_free(data);

    // Buffers
    GLuint vao[4], vbo[4], ebo[4];
    glGenVertexArrays(4, vao);
    glGenBuffers(4, vbo); glGenBuffers(4, ebo);

    VAO[0] = vao[0];
    VAO[1] = vao[1];
    VAO[2] = vao[2];
    VAO[3] = vao[3];

    unsigned int indices[] = {
    	0, 1, 2,
    	2, 3, 0
    };

    float uv[8];

    int spriteX = 150, spriteY = 0;
    int spriteW = 50, spriteH = 50;

    calculateUVs(spriteX, spriteY, spriteW, spriteH, texWidth, texHeight, uv);

    float vertices_standing_1[] = {
        // positions     // UVs
        -0.10f, -0.10f,     uv[0], uv[1], // bottom left
         0.10f, -0.10f,     uv[2], uv[3], // bottom right
         0.10f,  0.10f,     uv[4], uv[5], // top right
        -0.10f,  0.10f,     uv[6], uv[7]  // top left
    };

    glBindVertexArray(VAO[0]);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_standing_1), vertices_standing_1, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[0]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Attributs
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0); // pos
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float))); // UV
    glEnableVertexAttribArray(1);

    spriteX = 200;
    spriteY = 0;
    spriteW = 50;
    spriteH = 50;

    calculateUVs(spriteX, spriteY, spriteW, spriteH, texWidth, texHeight, uv);

    float vertices_standing_2[] = {
        // positions     // UVs
        -0.10f, -0.10f,     uv[0], uv[1], // bottom left
         0.10f, -0.10f,     uv[2], uv[3], // bottom right
         0.10f,  0.10f,     uv[4], uv[5], // top right
        -0.10f,  0.10f,     uv[6], uv[7]  // top left
    };

    glBindVertexArray(VAO[1]);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_standing_2), vertices_standing_2, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Attributs
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0); // pos
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float))); // UV
    glEnableVertexAttribArray(1);

    spriteX = 350;
    spriteY = 0;
    spriteW = 55;
    spriteH = 50;

    calculateUVs(spriteX, spriteY, spriteW, spriteH, texWidth, texHeight, uv);

    float vertices_crawling_1[] = {
        // positions     // UVs
        -0.10f, -0.10f,     uv[0], uv[1], // bottom left
         0.15f, -0.10f,     uv[2], uv[3], // bottom right
         0.15f,  0.10f,     uv[4], uv[5], // top right
        -0.10f,  0.10f,     uv[6], uv[7]  // top left
    };

    glBindVertexArray(VAO[2]);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_crawling_1), vertices_crawling_1, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[2]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Attributs
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0); // pos
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float))); // UV
    glEnableVertexAttribArray(1);

    spriteX = 414;
    spriteY = 0;
    spriteW = 55;
    spriteH = 50;

    calculateUVs(spriteX, spriteY, spriteW, spriteH, texWidth, texHeight, uv);

    float vertices_crawling_2[] = {
        // positions     // UVs
        -0.10f, -0.10f,     uv[0], uv[1], // bottom left
         0.15f, -0.10f,     uv[2], uv[3], // bottom right
         0.15f,  0.10f,     uv[4], uv[5], // top right
        -0.10f,  0.10f,     uv[6], uv[7]  // top left
    };

    glBindVertexArray(VAO[3]);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_crawling_2), vertices_crawling_2, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[3]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Attributs
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0); // pos
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float))); // UV
    glEnableVertexAttribArray(1);
}


void Player::makeHitbox()
{
	float vertices_standing[] = {
        // positions
        -0.10f,  -0.09f,
         0.077f, -0.09f,
         0.077f,  0.085f,
        -0.10f,   0.085f
    };

    float vertices_crawling[] = {
        // positions
        -0.10f, -0.10f,
         0.135f, -0.10f,
         0.135f,  0.002f,
        -0.10f,  0.002f
    };

    unsigned int indices[] = {
    	0, 1, 2,
    	2, 3, 0
    };

    GLuint vao[2], vbo[2], ebo[2];
    glGenVertexArrays(2, vao);
    glGenBuffers(2, vbo); glGenBuffers(2, ebo);

    hitbox[0] = vao[0];
    hitbox[1] = vao[1];

    glBindVertexArray(hitbox[0]);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_standing), vertices_standing, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[0]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Attributs
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0); // pos
    glEnableVertexAttribArray(0);

    glBindVertexArray(hitbox[1]);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_crawling), vertices_crawling, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Attributs
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0); // pos
    glEnableVertexAttribArray(0);
}

void Player::calculateUVs(int x, int y, int w, int h, int texWidth, int texHeight, float uv[8]) {
    float u_min = (float)x / texWidth;
    float u_max = (float)(x + w) / texWidth;
    float v_max = 1.0f - (float)y / texHeight;
    float v_min = 1.0f - (float)(y + h) / texHeight;

    // Chaque coin du quad : bas-gauche, bas-droit, haut-droit, haut-gauche
    uv[0] = u_min; uv[1] = v_min;  // Bottom-left
    uv[2] = u_max; uv[3] = v_min;  // Bottom-right
    uv[4] = u_max; uv[5] = v_max;  // Top-right
    uv[6] = u_min; uv[7] = v_max;  // Top-left
}

void Player::render()
{
	glUseProgram(shader);
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, position);
	glUniformMatrix4fv(positionLoc, 1, GL_FALSE, &model[0][0]);
    glBindTexture(GL_TEXTURE_2D, texture);
    if (crawl)
		glBindVertexArray(VAO[(frameCount % 40 > 20) ? 2 : 3]);
	else
		glBindVertexArray(VAO[(frameCount % 40 > 20) ? 0 : 1]);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	if (showHitbox)
	{
		glUseProgram(hitboxShader);
    	glUniformMatrix4fv(positionLoc, 1, GL_FALSE, &model[0][0]);
    	glUniform4f(hitboxColorLoc, 0.0f, 0.0f, 1.0f, 0.3f); 
    	glBindVertexArray(hitbox[crawl ? 1 : 0]);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}
	frameCount++;
	if (frameCount >= 10000)
		frameCount = 0;
}

void Player::move(movement direction, float deltaTime)
{
	(void)deltaTime;
	// float velocity = 2.5f * deltaTime;
	if (direction == DOWN && !crawl)
		crawl = true;
	else if (direction == UP && crawl)
		crawl = false;
}

void Player::toggleHitbox(press button)
{
	if (button == ON && !showHitbox)
		showHitbox = true;
	else if (button == OFF && showHitbox)
		showHitbox = false;
}