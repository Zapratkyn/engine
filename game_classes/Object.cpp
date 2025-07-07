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

glm::vec3 Object::getMin() { return position; }
glm::vec3 Object::getMax() { return glm::vec3(position.x + width, position.y + height, 0.0f); }