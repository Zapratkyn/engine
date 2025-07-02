#include "../game_include/Object.hpp"

Object::Object(Loader *loader) : 
objectShader(loader->getObjectShader()),
hitboxShader(loader->getHitboxShader()),
assets(loader->getAssets()),
positionLoc(loader->getPositionLoc()),
hitboxPositionLoc(loader->getHitboxPositionLoc()),
hitboxColorLoc(loader->getHitboxColorLoc())
{}