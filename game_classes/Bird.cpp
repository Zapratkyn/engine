#include "../game_include/Bird.hpp"

Bird::Bird(Loader *loader) : 
Object(loader)
{
	position = glm::vec3(0.0f, 0.2f, 0.0f);
	frameCount = 0;
	std::map<std::string, GLuint> vaos = loader->getVAOs();
	VAOs[0] = vaos["bird_1"];
	VAOs[1] = vaos["bird_2"];
	// hitbox[0] = dino_data.hitbox_standing;
	// hitbox[1] = dino_data.hitbox_crawling;
}

void Bird::render(bool showHitbox)
{
	(void)showHitbox;
	glUseProgram(objectShader);
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, position);
	glUniformMatrix4fv(positionLoc, 1, GL_FALSE, &model[0][0]);
    glBindTexture(GL_TEXTURE_2D, assets);
    glBindVertexArray(VAOs[(frameCount % 40 > 20) ? 0 : 1]);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	// glUseProgram(hitboxShader);
    // glUniformMatrix4fv(positionLoc, 1, GL_FALSE, &model[0][0]);
   	// glBindVertexArray(hitbox[crawl ? 1 : 0]);
	// if (showHitbox)
	// {
    // 	glUniform4f(hitboxColorLoc, 0.0f, 0.0f, 1.0f, 0.3f);
	// 	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	// }
	frameCount++;
	if (frameCount >= 10000)
		frameCount = 0;
}