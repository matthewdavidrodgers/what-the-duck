#ifndef GLOBMANAGER__H
#define GLOBMANAGER__H
#include <list>
#include "collisionStrategy.h"
#include "glob.h"

class GlobManager {

public :

   GlobManager(const std::string&);
   GlobManager(const GlobManager&);
   ~GlobManager();

   void draw() const;
   void update(Uint32);
   void shoot(const Vector2f&, const Vector2f&);

   unsigned int globCount() const { return globList.size(); }
   unsigned int freeCount() const { return freeList.size(); }

   //bool collided(const Drawable *) const;
   std::list<Glob *>& getGlobs() { return globList; }

   void reset();

   GlobManager& operator=(const GlobManager& lhs) = delete;

private :

   std::string owner;

   Image* globImage;
   std::list<Glob *> freeList;
   std::list<Glob *> globList;

   PerPixelCollisionStrategy strategy;

};

#endif
