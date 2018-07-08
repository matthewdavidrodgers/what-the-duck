#include <iostream>
#include <cmath>
#include "globManager.h"
#include "imageFactory.h"

GlobManager::GlobManager(const std::string& n) :
   owner(n),
   globImage(ImageFactory::getInstance().getImage(owner+"/glob")),
   freeList(),
   globList(),
   strategy()
{}

GlobManager::GlobManager(const GlobManager& other) :
   owner(other.owner),
   globImage(other.globImage),
   freeList(other.freeList),
   globList(other.globList),
   strategy(other.strategy)
{}

GlobManager::~GlobManager() {
   std::list<Glob*>::iterator globIt = globList.begin();
   while(globIt != globList.end()) {
      delete *globIt;
      globIt++;
   }

   std::list<Glob*>::iterator freeIt = freeList.begin();
   while(freeIt != freeList.end()) {
      delete *freeIt;
      freeIt++;
   }
}

void GlobManager::draw() const {
   for( Glob* glob : globList ) {
      glob->draw();
   }
}

void GlobManager::update(Uint32 ticks) {
   std::list<Glob*>::iterator it = globList.begin();
   while(it != globList.end()) {
      (*it)->update(ticks);
      if((*it)->isExpired()) {
         freeList.push_back(*it);
         it = globList.erase(it);
      }
      else {
         it++;
      }
   }
}

void GlobManager::shoot(const Vector2f& pos, const Vector2f& vel) {
   if(freeList.empty()) {
      globList.push_back(new Glob(owner, globImage, pos, vel));
   }
   else {
      Glob* glob = freeList.front();
      freeList.pop_front();
      glob->reset();
      glob->setPosition(pos);
      glob->setVelocity(vel);
      globList.push_back(glob);
   }
}

void GlobManager::reset() {
      std::list<Glob*>::iterator it = globList.begin();
      while(it != globList.end()) {
            freeList.push_back(*it);
            it = globList.erase(it);
      }
      for(Glob* glob : freeList) {
            glob->reset();
      }
}

/*
bool GlobManager::collided(const Drawable* target) const {
   for(Glob* glob : globList) {
      if(strategy.execute(*glob, *target))
         return true;
   }
   return false;
}
*/
