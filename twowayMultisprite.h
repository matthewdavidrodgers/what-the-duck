#ifndef TWOWAYMULTISPRITE__H
#define TWOWAYMULTISPRITE__H
#include <string>
#include <vector>
#include <cmath>
#include "drawable.h"
#include "explodingSprite.h"

class TwoWayMultiSprite : public Drawable {
public:
  TwoWayMultiSprite(const std::string&);
  TwoWayMultiSprite(const TwoWayMultiSprite&);

  virtual void draw() const;
  virtual void update(Uint32 ticks);

  virtual const Image* getImage() const { 
    if(currentMode == RIGHT_MODE)
      return imagesA[currentFrame]; 
    else
      return imagesB[currentFrame]; 
  }
  int getScaledWidth()  const { 
    if(currentMode == RIGHT_MODE)
      return getScale()*imagesA[currentFrame]->getWidth(); 
    else
      return getScale()*imagesB[currentFrame]->getWidth(); 
  } 
  int getScaledHeight()  const { 
    if(currentMode == RIGHT_MODE)
      return getScale()*imagesA[currentFrame]->getHeight(); 
    else
      return getScale()*imagesB[currentFrame]->getHeight(); 
  } 
  virtual const SDL_Surface* getSurface() const { 
    if(currentMode == RIGHT_MODE)
      return imagesA[currentFrame]->getSurface();
    else
      return imagesB[currentFrame]->getSurface();
  }

  virtual void explode();

  void reset();

protected:
  std::vector<Image *> imagesA;
  std::vector<Image *> imagesB;

  enum Mode {RIGHT_MODE, LEFT_MODE};
  Mode currentMode;
  unsigned currentFrame;
  unsigned numberOfFramesA;
  unsigned numberOfFramesB;
  unsigned frameInterval;
  float timeSinceLastFrame;
  int worldWidth;
  int worldHeight;

  ExplodingSprite* explosion;

  void advanceFrame(Uint32 ticks);
  void switchMode();
  TwoWayMultiSprite& operator=(const TwoWayMultiSprite&);
};
#endif
