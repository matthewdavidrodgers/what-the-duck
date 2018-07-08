#ifndef ENEMY__H
#define ENEMY__H

#include <string>
#include <vector>
#include <list>
#include "drawable.h"
#include "globManager.h"

class Enemy : public Drawable {

public:

Enemy(const std::string&, const Vector2f, int, int);
Enemy(const Enemy&);

virtual void draw() const;
virtual void update(Uint32 ticks);
virtual const Image* getImage() const;
int getScaledWidth() const;
int getScaledHeight() const;
virtual const SDL_Surface* getSurface() const;

void setPlayerPosition(const Vector2f& pos) { playerPosition = pos; }
bool inTerritory(const Drawable *) const;
void calm();
void getAngry();

int getFreeGlobs() const { return globs.freeCount(); }
int getUsedGlobs() const { return globs.globCount(); }

void reset();

std::list<Glob *>& getGlobs() { return globs.getGlobs(); }

private:

std::string name;

enum MODE {NORMAL, ANGRY};
enum DIRECTION {LEFT, RIGHT};
MODE currentMode;
DIRECTION currentDirection;

std::vector<Image *> imagesLeft;
std::vector<Image *> imagesRight;
std::vector<Image *> imagesAngry;

unsigned frameNumLeft;
unsigned frameNumRight;
unsigned frameNumAngry;
unsigned currentFrame;
unsigned frameInterval;

Vector2f playerPosition;
int playerWidth;
int playerHeight;
float territoryRadius;
Vector2f territoryOrigin;
Vector2f velocityNormal;

float timeSinceLastFrame;
int worldWidth;
int worldHeight;

GlobManager globs;
int ticksSinceSpit;

void advanceFrame(Uint32 ticks);
Enemy& operator=(const Enemy&);

};

#endif
