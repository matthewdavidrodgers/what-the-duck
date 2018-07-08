#include <vector>
#include <SDL.h>
#include "ioMod.h"
#include "renderContext.h"
#include "clock.h"
#include "world.h"
#include "viewport.h"
#include "player.h"
#include "enemy.h"
#include "collisionStrategy.h"
#include "hud.h"
#include "sound.h"

class Engine {
public:
  Engine ();
  ~Engine ();
  void play();

  Engine(const Engine&) = delete;
  Engine& operator=(const Engine&) = delete;

private:
  const RenderContext* rc;
  const IOmod& io;
  Clock& clock;
  CollisionStrategy* collision_strategy;

  SDL_Renderer * const renderer;
  World * water;
  World * path; 
  Viewport& viewport;

  Player* player;
  //Enemy* enemy;
  std::vector<Enemy*> enemies;
  std::vector<Drawable*> eggs;
  std::vector<Drawable*> consumedEggs;
  std::vector<Drawable*> sprites;

  bool makeVideo;
  bool godMode;
  bool endGame;
  bool beginGame;

  Hud hud;

  int eggCount;
  SDLSound& sounds;

  bool checkCollisions();
  void draw() const;
  void update(Uint32);

  void spriteCollisionCheck();
  void enemyTerritoryCheck();
  void resetActors();
};
