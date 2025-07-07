#include "../game_include/Loader.hpp"

Loader::Loader(Scene scene, GLuint objectShader, GLuint hitboxShader, GLint positionLoc, GLint hitboxPositionLoc) :
objectShader(objectShader),
hitboxShader(hitboxShader),
positionLoc(positionLoc),
hitboxPositionLoc(hitboxPositionLoc)
{
    // Where to find the sprites in the png
    std::map<std::string, std::vector<int>> coords;

    // Vertices positions of the sprites, used to give them a size on the screen
    std::map<std::string, std::vector<float>> positions;

    // Vertices and color of the hitboxes
    std::map<std::string, std::vector<float>> hitboxes;

    
    getSceneData(scene, &coords, &positions, &hitboxes);
	loadAssets(getAssetsPath(scene).c_str());
	for (auto it = coords.begin(); it != coords.end(); it++)
		makeObject(it->first, it->second, positions);
    for (auto it = hitboxes.begin(); it != hitboxes.end(); it++)
        makeHitbox(it->first, it->second);
}

Loader::~Loader()
{
	for (std::map<std::string, GLuint>::iterator it = VAOs.begin(); it != VAOs.end(); it++)
		glDeleteVertexArrays(1, &(it->second));
	for (std::vector<GLuint>::iterator it = VBOs.begin(); it != VBOs.end(); it++)
		glDeleteBuffers(1, (&*it));
	for (std::vector<GLuint>::iterator it = EBOs.begin(); it != EBOs.end(); it++)
		glDeleteBuffers(1, &(*it));
}

void Loader::loadAssets(const char *assetsPath)
{
    int width, height, channels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(assetsPath, &width, &height, &channels, STBI_rgb_alpha);

    texWidth = width;
    texHeight = height;

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    assets = texture;

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
}

void Loader::makeObject(std::string name, std::vector<int> &coords, std::map<std::string, std::vector<float>> &positions)
{
	GLuint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO); glGenBuffers(1, &EBO);

    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    float uv[8];

    calculateUVs(coords[0], coords[1], coords[2], coords[3], uv);
    float vertices[] = {
        // positions                            // UVs
        0.0f,               0.0f,               uv[0], uv[1], // bottom left
        positions[name][0], 0.0f,               uv[2], uv[3], // bottom right
        positions[name][0], positions[name][1], uv[4], uv[5], // top right
        0.0f,               positions[name][1], uv[6], uv[7]  // top left
    };

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);    

    // Attributs
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0); // pos
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float))); // UV
    glEnableVertexAttribArray(1);

    VBOs.push_back(VBO);
    EBOs.push_back(EBO);
    VAOs[name] = VAO;
}

void Loader::makeHitbox(std::string name, std::vector<float> &data)
{
    GLuint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO); glGenBuffers(1, &EBO);

    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };
    float vertices[] = {
        // positions            // colors
        0.0f,       0.0f,       data[2], data[3], data[4], // bottom left
        data[0],    0.0f,       data[2], data[3], data[4], // bottom right
        data[0],    data[1],    data[2], data[3], data[4], // top right
        0.0f,       data[1],    data[2], data[3], data[4]  // top left
    };

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);    

    // Attributs
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0); // pos
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float))); // color
    glEnableVertexAttribArray(1);

    VBOs.push_back(VBO);
    EBOs.push_back(EBO);
    hitboxes[name] = VAO;
}

void Loader::calculateUVs(int x, int y, int w, int h, float uv[8]) {
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

GLuint Loader::getObjectShader() const { return objectShader; }
GLuint Loader::getHitboxShader() const { return hitboxShader; }
GLuint Loader::getAssets() const { return assets; }
GLint Loader::getPositionLoc() const { return positionLoc; }
GLint Loader::getHitboxPositionLoc() const { return hitboxPositionLoc; }

std::map<std::string, GLuint> Loader::getVAOs() const { return VAOs; }
std::map<std::string, GLuint> Loader::getHitboxes() const { return hitboxes; }