#ifndef HUD__H
#define HUD__H
#include <SDL.h>

class Hud {

public:

   Hud(SDL_Renderer*);
   void toggleActive() {active = !active;}
   //void update(int, int);
   void draw() const;

   Hud(const Hud&) = delete;
   ~Hud() = default;
   Hud& operator=(const Hud&);

private:

   SDL_Renderer* renderer;
   int viewWidth;
   int viewHeight;
   //int freeGlobs;
   //int usedGlobs;
   //int hudWidth;
   //int hudHeight;
   bool active;

   void drawHelper() const;
};

#endif
