#ifndef PLAYER__H
#define PLAYER__H
#include "twowayMultisprite.h"
#include "enemy.h"
#include <list>

class Player : public TwoWayMultiSprite {

public:

   Player(const std::string&);
   Player(const Player&);
   virtual void update(Uint32 ticks);
   Player& operator=(const Player&);
   
   void collided() { collision = true; }
   void missed() { collision = false; }
   void offPath();

   void right();
   void left();
   void up();
   void down();
   void stop();

   void attach(Enemy* e) { observers.push_back(e); }
   void detach(Enemy* e);

   void incrementEggCount() { eggCount++; }
   int getEggCount() { return eggCount; }

   void reset();

private:

   std::string xmlName;
   int eggCount;
   bool collision;
   Vector2f xmlVelocity;
   std::list<Enemy *> observers;

};

#endif
