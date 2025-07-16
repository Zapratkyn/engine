#include "../game_include/Score.hpp"

Score::Score(Loader *loader) : Object(loader), currentScore(0), hiScore(0)
{
	std::map<std::string, GLuint> vaos = loader->getVAOs();
	VAOs[0] = vaos["0"];
	VAOs[1] = vaos["1"];
	VAOs[2] = vaos["2"];
	VAOs[3] = vaos["3"];
	VAOs[4] = vaos["4"];
	VAOs[5] = vaos["5"];
	VAOs[6] = vaos["6"];
	VAOs[7] = vaos["7"];
	VAOs[8] = vaos["8"];
	VAOs[9] = vaos["9"];
	hi = vaos["hi"];

	frameCount = 0;

	hiPosition = glm::vec3(0.3f, 0.85f, 0.0f);
	offsets[0] = glm::vec3(0.15f, 0.0f, 0.0f); // between 'hi' and high score and between high score and current score
	offsets[1] = glm::vec3(0.04f, 0.0f, 0.0f); // between digits
}

void Score::render()
{
	std::list<int> hiDigits = getDigits(hiScore);
	std::list<int> currentDigits = getDigits(currentScore);
	glUseProgram(objectShader);
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, hiPosition);
	glUniformMatrix4fv(positionLoc, 1, GL_FALSE, &model[0][0]);
    glBindTexture(GL_TEXTURE_2D, assets);
    glBindVertexArray(hi);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	model = glm::translate(model, offsets[0]);
	glUniformMatrix4fv(positionLoc, 1, GL_FALSE, &model[0][0]);
    glBindTexture(GL_TEXTURE_2D, assets);
    glBindVertexArray(VAOs[(*hiDigits.begin())]);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	for (auto it = hiDigits.begin(); it != hiDigits.end(); it++)
	{
		if (it == hiDigits.begin())
			continue;
		model = glm::translate(model, offsets[1]);
		glUniformMatrix4fv(positionLoc, 1, GL_FALSE, &model[0][0]);
	    glBindTexture(GL_TEXTURE_2D, assets);
	    glBindVertexArray(VAOs[*it]);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}
	model = glm::translate(model, offsets[0]);
	glUniformMatrix4fv(positionLoc, 1, GL_FALSE, &model[0][0]);
    glBindTexture(GL_TEXTURE_2D, assets);
    glBindVertexArray(VAOs[(*currentDigits.begin())]);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	for (auto it = currentDigits.begin(); it != currentDigits.end(); it++)
	{
		if (it == currentDigits.begin())
			continue;
		model = glm::translate(model, offsets[1]);
		glUniformMatrix4fv(positionLoc, 1, GL_FALSE, &model[0][0]);
	    glBindTexture(GL_TEXTURE_2D, assets);
	    glBindVertexArray(VAOs[*it]);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}
}

std::list<int> Score::getDigits(unsigned int nb)
{
	std::list<int> result;

	while (nb > 0) {
        int digit = nb % 10;
        result.push_front(digit);
        nb /= 10;
    }

    while(result.size() < 5)
    	result.push_front(0);

    return result;
}

void Score::update(bool &dead) 
{
	if (!dead)
	{
		frameCount++;
		if ((frameCount % 10) == 0)
			currentScore++;
	}
	else if (currentScore > 0)
	{
		if (currentScore > hiScore)
			hiScore = currentScore;
		currentScore = 0;
		frameCount = 0;
	}
}