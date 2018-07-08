#ifndef GLOB__H
#define GLOB__H
#include "sprite.h"

class Glob : public Sprite {

public:

  Glob(std::string&, Image *, const Vector2f&, const Vector2f&);
  Glob(const Glob&);
  virtual void update(Uint32);
  bool isExpired() const { return expired; }
  void reset();

  Glob& operator=(const Glob&) = delete;

private:

  std::string owner;
  float distance;
  float maxDistance;
  bool expired;

};

#endif
