#include "hud.h"
#include "gamedata.h"
#include "ioMod.h"

Hud::Hud(SDL_Renderer* r) :
   renderer(r),
   viewWidth(Gamedata::getInstance().getXmlInt("view/width")),
   viewHeight(Gamedata::getInstance().getXmlInt("view/height")),
   active(true)
{}

void Hud::draw() const {
   if(active)
      drawHelper();
}

void Hud::drawHelper() const {
   int x = viewWidth - 230;

   SDL_Rect r = { x-20, 25, 230, 300};
   SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
   SDL_SetRenderDrawColor( renderer, 255, 255, 255, 255/2 );
   SDL_RenderFillRect(renderer, &r);

   IOmod::getInstance().setWriteColor({255, 0, 0, 0});
   IOmod::getInstance().writeText("CONTROLS", x, 30);
   IOmod::getInstance().writeText("w:", x, 60);
   IOmod::getInstance().writeText("a:", x, 90);
   IOmod::getInstance().writeText("s:", x, 120);
   IOmod::getInstance().writeText("d:", x, 150);
   IOmod::getInstance().writeText("g:", x, 180);
   IOmod::getInstance().writeText("r:", x, 210);
   IOmod::getInstance().writeText("ESC:", x, 240);
   IOmod::getInstance().writeText("F1:", x, 270);

   IOmod::getInstance().writeText("up", x+80, 60);
   IOmod::getInstance().writeText("left", x+80, 90);
   IOmod::getInstance().writeText("down", x+80, 120);
   IOmod::getInstance().writeText("right", x+80, 150);
   IOmod::getInstance().writeText("god mode", x+80, 180);
   IOmod::getInstance().writeText("reset", x+80, 210);
   IOmod::getInstance().writeText("exit", x+80, 240);
   IOmod::getInstance().writeText("show this", x+80, 270);
   IOmod::getInstance().writeText("menu again", x+80, 290);
}
