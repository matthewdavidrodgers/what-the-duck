#include "enemy.h"
#include "gamedata.h"
#include "renderContext.h"
#include "vector2f.h"
#include "sound.h"

Enemy::Enemy(const std::string& s, const Vector2f position, int w, int h) :
  Drawable(s,
      Vector2f(Gamedata::getInstance().getXmlInt(s+"/startLoc/x"),
         Gamedata::getInstance().getXmlInt(s+"/startLoc/y")),
      Vector2f(Gamedata::getInstance().getXmlInt(s+"/speedX"),
         Gamedata::getInstance().getXmlInt(s+"/speedY"))
   ),
   name(s),
   currentMode(NORMAL),
   currentDirection(RIGHT),
   imagesLeft(RenderContext::getInstance()->getImages(name+"/left")),
   imagesRight(RenderContext::getInstance()->getImages(name+"/right")),
   imagesAngry(RenderContext::getInstance()->getImages(name+"/angry")),
   frameNumLeft(Gamedata::getInstance().getXmlInt(name+"/left/frames")),
   frameNumRight(Gamedata::getInstance().getXmlInt(name+"/right/frames")),
   frameNumAngry(Gamedata::getInstance().getXmlInt(name+"/angry/frames")),
   currentFrame(0),
   frameInterval(Gamedata::getInstance().getXmlInt(name+"/frameInterval")),
   playerPosition(position),
   playerWidth(w),
   playerHeight(h),
   territoryRadius(Gamedata::getInstance().getXmlFloat(name+"/territorySize")),
   territoryOrigin(Vector2f(getX(), getY())),
   velocityNormal(Vector2f(getVelocityX(), getVelocityY())),
   timeSinceLastFrame(0),
   worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
   worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
   globs(name),
   ticksSinceSpit(20)
{}

Enemy::Enemy(const Enemy& other) :
   Drawable(other),
   name(other.name),
   currentMode(other.currentMode),
   currentDirection(other.currentDirection),
   imagesLeft(other.imagesLeft),
   imagesRight(other.imagesRight),
   imagesAngry(other.imagesAngry),
   frameNumLeft(other.frameNumLeft),
   frameNumRight(other.frameNumRight),
   frameNumAngry(other.frameNumAngry),
   currentFrame(other.currentFrame),
   frameInterval(other.frameInterval),
   playerPosition(other.playerPosition),
   playerWidth(other.playerWidth),
   playerHeight(other.playerHeight),
   territoryRadius(other.territoryRadius),
   territoryOrigin(other.territoryOrigin),
   velocityNormal(other.velocityNormal),
   timeSinceLastFrame(other.timeSinceLastFrame),
   worldWidth(other.worldWidth),
   worldHeight(other.worldHeight),
   globs(other.globs),
   ticksSinceSpit(other.ticksSinceSpit)
{}

void Enemy::draw() const {
   switch(currentMode) {
      case NORMAL :
         switch(currentDirection) {
            case LEFT: imagesLeft[currentFrame]->draw(getX(), getY(), getScale()); break;
            case RIGHT: imagesRight[currentFrame]->draw(getX(), getY(), getScale()); break;
         }
         break;
      case ANGRY : imagesAngry[currentFrame]->draw(getX(), getY(), getScale()); break;
   }
   globs.draw();
}

void Enemy::update(Uint32 ticks) {
   advanceFrame(ticks);
   Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
   setPosition(getPosition() + incr);

   if(getX() > territoryOrigin[0] + territoryRadius || getX() > worldWidth-getScaledWidth()) {
      currentDirection = LEFT;
      currentFrame = 0;
      setVelocityX( -fabs( getVelocityX() ) );
   }

   if(getX() < territoryOrigin[0] || getX() < 0) {
      currentFrame = 0;
      currentDirection = RIGHT;
      setVelocityX( fabs( getVelocityX() ) );
   }

   globs.update(ticks);

   if(currentMode == ANGRY)  {
      if(ticksSinceSpit > 20) {
         const Vector2f pos(getX()+(getScaledWidth()/2), getY());
         int scalar = Gamedata::getInstance().getXmlInt(name+"/glob/speedMultiplier");
         Vector2f vel((playerPosition[0]-pos[0]), (playerPosition[1]-pos[1]));
         vel = vel.normalize();
         vel *= static_cast<float>(scalar);
         globs.shoot(pos, vel);
         SDLSound::getInstance()[1];
         ticksSinceSpit = 0;
      }
      else {
         ticksSinceSpit++;
      }
   }
}

void Enemy::advanceFrame(Uint32 ticks) {
   timeSinceLastFrame += ticks;
   if(timeSinceLastFrame > frameInterval) {
      unsigned * frameNum;
      switch(currentMode) {
         case NORMAL :
            switch(currentDirection) {
               case LEFT : frameNum = &frameNumLeft; break;
               case RIGHT: frameNum = &frameNumRight; break;
            }
            break;
         case ANGRY : frameNum = &frameNumAngry; break;
      }
      currentFrame = (currentFrame+1) % *frameNum;
      timeSinceLastFrame = 0;
   } 
}

const Image * Enemy::getImage() const {
   if(currentMode == NORMAL) {
      switch(currentDirection) {
         case LEFT : return imagesLeft[currentFrame];
         case RIGHT : return imagesRight[currentFrame];
         default : return imagesLeft[currentFrame];
      }
   }
   else
      return imagesAngry[currentFrame];
}

int Enemy::getScaledWidth() const {
   if(currentMode == NORMAL) {
      switch(currentDirection) {
         case LEFT : return getScale()*imagesLeft[currentFrame]->getWidth(); 
         case RIGHT : return getScale()*imagesRight[currentFrame]->getWidth(); 
         default : return getScale()*imagesLeft[currentFrame]->getWidth(); 
      }
   }
   else
      return getScale()*imagesAngry[currentFrame]->getWidth(); 
}

int Enemy::getScaledHeight() const {
   if(currentMode == NORMAL) {
      switch(currentDirection) {
         case LEFT : return getScale()*imagesLeft[currentFrame]->getHeight(); 
         case RIGHT : return getScale()*imagesRight[currentFrame]->getHeight(); 
         default : return getScale()*imagesLeft[currentFrame]->getHeight(); 
      }
   }
   else
      return getScale()*imagesAngry[currentFrame]->getHeight(); 
}

const SDL_Surface* Enemy::getSurface() const {
   if(currentMode == NORMAL) {
      switch(currentDirection) {
         case LEFT : return imagesLeft[currentFrame]->getSurface(); 
         case RIGHT : return imagesRight[currentFrame]->getSurface(); 
         default : return imagesLeft[currentFrame]->getSurface(); 
      }
   }
   else
      return imagesAngry[currentFrame]->getSurface(); 
}

bool Enemy::inTerritory(const Drawable * intruder) const {
   return intruder->getX() > territoryOrigin[0] &&
      intruder->getX() < territoryOrigin[0] + territoryRadius &&
      intruder->getY() < territoryOrigin[1] + territoryRadius &&
      intruder->getY() > territoryOrigin[1] - territoryRadius;
}

void Enemy::getAngry() {
   if(currentMode == ANGRY) return;
   currentMode = ANGRY;
   setVelocity(Vector2f(0,0));
   ticksSinceSpit = 20;
}

void Enemy::calm() {
   if(currentMode == NORMAL) return;
   currentMode = NORMAL;
   currentFrame = 0;
   setVelocity(velocityNormal);
   currentDirection = RIGHT;
}

void Enemy::reset() {
      Drawable::reset();
      currentMode = NORMAL;
      currentDirection = RIGHT;
      currentFrame = 0;
      timeSinceLastFrame = 0;
      globs.reset();
      ticksSinceSpit = 0;
}
