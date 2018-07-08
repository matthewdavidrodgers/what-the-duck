#include "gamedata.h"
#include "imageFactory.h"
#include "glob.h"

Glob::Glob(std::string& s, Image* globFrame, const Vector2f& pos, const Vector2f& vel) :
   Sprite("spitGlob_"+s, pos, vel, globFrame),
   owner(s),
   distance(0),
   maxDistance(Gamedata::getInstance().getXmlInt(owner+"/glob/expireDistance")),
   expired(false)
{}

Glob::Glob(const Glob& other) :
   Sprite(other),
   owner(other.owner),
   distance(other.distance),
   maxDistance(other.maxDistance),
   expired(other.expired)
{}

void Glob::update(Uint32 ticks) {
   Vector2f pos = getPosition();
   Sprite::update(ticks);
   if(getY() < 0 || getY()+getScaledHeight() > worldHeight)
      expired = true;
   if(getX() < 0 || getX()+getScaledWidth() > worldWidth)
      expired = true;
   distance += ( hypot(getX()-pos[0], getY()-pos[1]) );
   if(distance > maxDistance)
      expired = true;
   
}

void Glob::reset() {
   expired = false;
   distance = 0;
}
