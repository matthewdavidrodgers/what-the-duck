#include <iostream>
#include <algorithm>
#include <sstream>
#include <string>
#include <random>
#include <iomanip>
#include "sprite.h"
#include "multisprite.h"
#include "twowayMultisprite.h"
#include "gamedata.h"
#include "engine.h"
#include "frameGenerator.h"

Engine::~Engine() { 
  delete player;
  //delete enemy;
  delete collision_strategy;
  delete water;
  delete path; 
  for(Drawable* spr : sprites) {
    delete spr;
  }
  for(Drawable* egg : eggs) {
    delete egg;
  }
  for(Drawable* egg : consumedEggs) {
    delete egg;
  }
  for(Enemy* enemy : enemies) {
    delete enemy;
  } 
  std::cout << "Terminating program" << std::endl;
}

Engine::Engine() :
  rc( RenderContext::getInstance() ),
  io( IOmod::getInstance() ),
  clock( Clock::getInstance() ),
  collision_strategy( new PerPixelCollisionStrategy() ),
  renderer( rc->getRenderer() ),
  water(new World("water", Gamedata::getInstance().getXmlInt("water/factor")) ),
  path(new World("path", Gamedata::getInstance().getXmlInt("path/factor")) ),
  viewport( Viewport::getInstance()),
  player(new Player("DuckTwoWay")),
  //enemy(new Enemy("Swan", player->getPosition(), player->getScaledWidth(), player->getScaledHeight())),
  enemies(),
  eggs(),
  consumedEggs(),
  sprites(),
  makeVideo( Gamedata::getInstance().getXmlBool("frameCapOn")),
  godMode( false ),
  endGame(false),
  beginGame(true),
  hud(renderer),
  eggCount( Gamedata::getInstance().getXmlInt("EggCount") ),
  sounds( SDLSound::getInstance() )
{
  //player->attach(enemy);
  sprites.reserve(5);
  enemies.reserve(7);
  eggs.reserve(eggCount);
  
  //enemies.push_back(new Enemy("Swan", player->getPosition(), player->getScaledWidth(), player->getScaledHeight()));
  //enemies.push_back(new Enemy("SwanLower", player->getPosition(), player->getScaledWidth(), player->getScaledHeight()));
  for(int i = 1; i <= (int)sprites.capacity(); i++) {
    std::stringstream str;
    str << "Bird_" << i;
    sprites.push_back(new MultiSprite(str.str()));
  }
  for(int i = 1; i <= (int)enemies.capacity(); i++) {
    std::stringstream str;
    str << "Swan_" << i;
    enemies.push_back(new Enemy(str.str(), player->getPosition(), player->getScaledWidth(), player->getScaledHeight()));
  }
  for(int i = 1; i <= eggCount; i++) {
    std::stringstream str;
    str << "Egg_" << i;
    eggs.push_back(new MultiSprite(str.str()));
  }
  for(Enemy* enemy : enemies) {
    player->attach(enemy);
  }
  Viewport::getInstance().setObjectToTrack(player);
  std::cout << "Loading complete" << std::endl;
}

void Engine::spriteCollisionCheck() {
  bool spriteCollision = false;
  Drawable* eggPtr = nullptr;
  for(Drawable * d : sprites) {
    if(collision_strategy->execute(*player, *d))
      spriteCollision = true;
  }
  for(Enemy * enemy : enemies) {
    for(Drawable * glob : enemy->getGlobs()) {
      if(collision_strategy->execute(*player, *glob))
        spriteCollision = true;
    }
  }
  for(Drawable * egg : eggs) {
    if(collision_strategy->execute(*player, *egg)) {
      eggPtr = egg;
    }
  }
  if(!godMode && spriteCollision)
    player->collided();
  else
    player->missed();

  if(eggPtr != nullptr) {
    sounds[0];
    player->incrementEggCount();
    std::vector<Drawable*>::iterator it = eggs.begin();
    while(it != eggs.end()) {
      if(*it == eggPtr) {
        consumedEggs.push_back(eggPtr);
        it = eggs.erase(it);
        break;
      }
      else {
        it++;
      }
    }
  }
}

void Engine::enemyTerritoryCheck() {
  for(Enemy * enemy : enemies) {
    if(enemy->inTerritory(player)) {
      enemy->getAngry();
    }
    else {
      enemy->calm();
    }
  }
}

void Engine::draw() const {

  if(endGame) {
    viewport.drawEndText();
    SDL_RenderPresent(renderer);
    return;
  }

  water->draw();
  path->draw();

  if(beginGame) {
    viewport.drawBeginText(renderer);
    hud.draw();
    SDL_RenderPresent(renderer);
    return;
  }

  player->draw();
  //enemy->draw();
  for(Enemy * enemy : enemies) {
    enemy->draw();
  }
  for(Drawable* spr : sprites) {
    spr->draw();
  }
  for(Drawable* egg : eggs) {
    egg->draw();
  }

  viewport.draw();
  hud.draw();
  SDL_RenderPresent(renderer);
}

void Engine::update(Uint32 ticks) {
  if(endGame) {
    viewport.update();
    return;
  }
  spriteCollisionCheck();
  enemyTerritoryCheck();
  player->update(ticks);
  //enemy->update(ticks);
  for(Enemy * enemy : enemies) {
    enemy->update(ticks);
  }
  for(Drawable* spr : sprites) {
    spr->update(ticks);
  }
  for(Drawable* egg : eggs) {
    egg->update(ticks);
  }
  water->update();
  path->update();
  //hud.update(enemy->getFreeGlobs(), enemy->getUsedGlobs());
  //hud.update(0, 0);
  //viewport.isColliding(false);
  viewport.update(); // always update viewport last
}

void Engine::resetActors() {
  player->reset();
  for(Enemy * enemy : enemies) {
    enemy->reset();
  }
  std::vector<Drawable*>::iterator eggIt = consumedEggs.begin();
  while(eggIt != consumedEggs.end()) {
    eggs.push_back(*eggIt);
    eggIt = consumedEggs.erase(eggIt);
  }
  for(Drawable * egg : eggs) {
    egg->reset();
  }
  for(Drawable * spr : sprites) {
    spr->reset();
  }
}

void Engine::play() {
  SDL_Event event;
  const Uint8* keystate;
  bool done = false;
  bool haventSwitchedHUDOff = false;
  Uint32 ticks = clock.getElapsedTicks();
  FrameGenerator frameGen;
  int secondsPassed = 0;
  int start = 0;
  hud.toggleActive();
  int HUDtime = Gamedata::getInstance().getXmlInt("initialHUDtime");

  while ( !done ) {
    // The next loop polls for events, guarding against key bounce:
    while ( SDL_PollEvent(&event) ) {
      keystate = SDL_GetKeyboardState(NULL);
      if (event.type ==  SDL_QUIT) { done = true; break; }
      if(event.type == SDL_KEYDOWN) {
        if (keystate[SDL_SCANCODE_ESCAPE] || keystate[SDL_SCANCODE_Q]) {
          done = true;
          break;
        }
        if ( keystate[SDL_SCANCODE_P] ) {
          if ( clock.isPaused() ) clock.unpause();
          else clock.pause();
        }
        if(keystate[SDL_SCANCODE_E]) {
          player->explode();
        }
        if(keystate[SDL_SCANCODE_F1]) {
          hud.toggleActive();
        }
        if(keystate[SDL_SCANCODE_R]) {
          resetActors();
          endGame = false;
        }
        if(keystate[SDL_SCANCODE_G]) {
          godMode = !godMode;
        }
        if(beginGame && keystate[SDL_SCANCODE_SPACE]) {
          haventSwitchedHUDOff = true;
          beginGame = false;
          start = Clock::getInstance().getTicks();
          hud.toggleActive();
        }
        if (keystate[SDL_SCANCODE_F4] && !makeVideo) {
          std::cout << "Initiating frame capture" << std::endl;
          makeVideo = true;
        }
        else if (keystate[SDL_SCANCODE_F4] && makeVideo) {
          std::cout << "Terminating frame capture" << std::endl;
          makeVideo = false;
        }
      }
    }
    if(player->getEggCount() == eggCount) {
      endGame = true;
    }
    
    secondsPassed = (Clock::getInstance().getTicks() - start) / 1000;
    if(haventSwitchedHUDOff && secondsPassed >= HUDtime) {
      haventSwitchedHUDOff = false;
      hud.toggleActive();
    }

    // In this section of the event loop we allow key bounce:

    ticks = clock.getElapsedTicks();
    if ( ticks > 0 ) {
      clock.incrFrame();
      if(keystate[SDL_SCANCODE_W])
        player->up();
      if(keystate[SDL_SCANCODE_S])
        player->down();
      if(keystate[SDL_SCANCODE_A])
        player->left();
      if(keystate[SDL_SCANCODE_D])
        player->right();
      draw();
      update(ticks);
      if ( makeVideo ) {
        frameGen.makeFrame();
      }
    }
  }
}
