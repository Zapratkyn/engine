#include "../game_include/Object.hpp"

Object::Object(Loader *loader) : 
objectShader(loader->getObjectShader()),
hitboxShader(loader->getHitboxShader()),
assets(loader->getAssets()),
positionLoc(loader->getPositionLoc()),
hitboxPositionLoc(loader->getHitboxPositionLoc())
{}

Object::Object(Loader *loader, glm::vec3 movement, float speed) : 
objectShader(loader->getObjectShader()),
hitboxShader(loader->getHitboxShader()),
assets(loader->getAssets()),
positionLoc(loader->getPositionLoc()),
hitboxPositionLoc(loader->getHitboxPositionLoc()),
movement(movement),
speed(speed)
{}

void Object::update(float deltaTime) 
{
	float velocity = speed * deltaTime;
	position += movement * velocity;
}

void Object::render()
{
	glUseProgram(objectShader);
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, position);
	glUniformMatrix4fv(positionLoc, 1, GL_FALSE, &model[0][0]);
    glBindTexture(GL_TEXTURE_2D, assets);
    glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Object::render(bool showHitbox)
{
	glUseProgram(objectShader);
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, position);
	glUniformMatrix4fv(positionLoc, 1, GL_FALSE, &model[0][0]);
    glBindTexture(GL_TEXTURE_2D, assets);
    glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	if (showHitbox)
	{
		glUseProgram(hitboxShader);
	    glUniformMatrix4fv(hitboxPositionLoc, 1, GL_FALSE, &model[0][0]);
	   	glBindVertexArray(hitbox);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}
}

glm::vec3 Object::getMin() { return position; }
glm::vec3 Object::getMax() { return glm::vec3(position.x + width, position.y + height, 0.0f); }