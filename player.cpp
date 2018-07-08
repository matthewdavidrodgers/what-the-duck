#include "player.h"

Player::Player(const std::string& name) :
   TwoWayMultiSprite(name),
   xmlName(name),
   eggCount(0),
   collision(false),
   xmlVelocity(getVelocity()),
   observers()
{}

Player::Player(const Player& plyr) :
   TwoWayMultiSprite(plyr),
   xmlName(plyr.xmlName),
   eggCount(plyr.eggCount),
   collision(plyr.collision),
   xmlVelocity(plyr.getVelocity()),
   observers(plyr.observers)
{}

Player& Player::operator=(const Player& plyr) {
   TwoWayMultiSprite::operator=(plyr);
   collision = plyr.collision;
   xmlVelocity = plyr.xmlVelocity;
   return *this;
}

void Player::right() {
   if(explosion) return;
   currentMode = RIGHT_MODE;
   if(getX() < worldWidth-getScaledWidth())
      setVelocityX(xmlVelocity[0]);
}

void Player::left() {
   if(explosion) return;
   currentMode = LEFT_MODE;
   if(getX() > 0)
      setVelocityX(-xmlVelocity[0]); 
}

void Player::up() {
   if(explosion) return;
   if(getY() > 150)
      setVelocityY(-xmlVelocity[1]);
}

void Player::down() {
   if(explosion) return;
   //if(getY() < worldHeight-getScaledHeight())
   if(getY() < 400)
      setVelocityY(xmlVelocity[1]);
}

void Player::stop() {
   setVelocity(Vector2f(0,0));
}

void Player::update(Uint32 ticks) {

  if(collision || explosion) {
    explode();
    explosion->update(ticks);
    if(explosion->chunkCount() == 0) {
      delete explosion;
      explosion = NULL;
      Drawable::reset();
    }
    return;
  }

   std::list<Enemy *>::iterator it = observers.begin();
   while(it != observers.end()) {
      Vector2f pos(getX()+getScaledWidth()/2, getY()+getScaledHeight()/2);
      (*it)->setPlayerPosition(pos);
      it++;
   }
   
   advanceFrame(ticks);

   Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
   setPosition(getPosition() + incr);
   stop();
}

void Player::detach(Enemy* e) {
   std::list<Enemy*>::iterator it = observers.begin();
   while(it != observers.end()) {
      if(*it == e) {
         it = observers.erase(it);
         return;
      }
      else {
         it++;
      }
   }
}

void Player::reset() {
  TwoWayMultiSprite::reset();
  eggCount = 0;
  collision = false;
}
