#include <sstream>
#include "viewport.h"
#include "ioMod.h"
#include "clock.h"

Viewport& Viewport::getInstance() {
  static Viewport viewport;
  return viewport;
}

Viewport::Viewport() : 
  gdata(Gamedata::getInstance()),
  position(0, 0),
  worldWidth(gdata.getXmlInt("world/width")),
  worldHeight(gdata.getXmlInt("world/height")),
  viewWidth(gdata.getXmlInt("view/width")), 
  viewHeight(gdata.getXmlInt("view/height")),
  objWidth(0), objHeight(0),
  objectToTrack(NULL),
  clock(Clock::getInstance())//,
  //HUDShown(true),
  //colliding(false)
{}

void Viewport::setObjectToTrack(const Drawable *obj) { 
  objectToTrack = obj; 
  objWidth = objectToTrack->getScaledWidth();
  objHeight = objectToTrack->getScaledHeight();
}

void Viewport::draw() const {

  IOmod::getInstance().setWriteColor({255, 0, 0, 0});

  std::stringstream fpsString;
  fpsString << "FPS: " << clock.getFpsWrapper();
  IOmod::getInstance().writeText(fpsString.str(), 30, 30);

  SDL_Color color({0, 0, 255, 0});
  IOmod::getInstance().setWriteColor(color);
  std::string myName = "Matthew David Rodgers (mdrodge)";
  IOmod::getInstance().writeText(myName, 30, 575);

  //if(colliding) drawCollisionText();
}

void Viewport::drawEndText() const {
  IOmod::getInstance().setWriteColor({255, 0, 0, 0});

  std::string line_one = "CONGRATULATIONS! YOU WIN!";
  std::string line_two = "press \"r\" to restart";

  IOmod::getInstance().writeText(line_one, 125, 300);
  IOmod::getInstance().writeText(line_two, 225, 330);
}

void Viewport::drawBeginText(SDL_Renderer * renderer) const {
  SDL_Rect r = {0, 0, 800, 637};
  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
  SDL_SetRenderDrawColor( renderer, 255, 255, 255, 255/2 );
  SDL_RenderFillRect(renderer, &r);

  IOmod::getInstance().setWriteColor({100, 100, 175, 0});
  //master:
  //In the famous children's story, an \"ugly duckling\" feels out of place because he's not as pretty as his siblings. In the happy (?) ending, the duckling learns he's not a duck at all, but a swan. He finds his new family and leaves his old life behind.
  //What you probably didn't know was that the young swan was a real jerk. And as soon as he figured out he was a swan, he ditched the family that raised him and turned into a pretentious a$$hole. He thought he was better than everyone who wasn't a swan like him. Really didn't learn any lessons from the whole ordeal.
  //He got so obsessed with his superiority that he convinced a bunch of other swans to make sure they were always the greatest, by whatever means necessary. Last night, the swan and his cronies snuck into a duck home and took all their eggs!
  //You need to get all the eggs! But watch out for the swans, who've been swayed by this real Hitler of a swan. And also the birds. They're just kind of crazy. We're not really sure why.
  //Press <space> to play!

  std::string line_1 = "THE UGLY DUCKLING: LOOK WHO'S UGLY NOW";
  std::string line_2 = "by Matthew David Rodgers";
  
  std::string line_3 = "You've all heard the children's tale \"The Ugly Duckling\".";

  std::string line_7 = "What you probably didn't know is that the young swan was";
  std::string line_8 = "a real jerk. And as soon as he figured out he was a swan,";
  std::string line_9 = "he ditched the family that raised him and turned into a";
  std::string line_10 = "serious a$$hole. He thought he was better than everyone";
  std::string line_11 = "who wasn't a swan like him. Really didn't learn any lessons";
  std::string line_mid = "from the whole ordeal.";

  std::string line_12 = "He got so obsessed with his superiority that he convinced";
  std::string line_13 = "a bunch of other swans to make sure they were always the";
  std::string line_14 = "best, by whatever means necessary. Last night, the swan";
  std::string line_15 = "and his cronies snuck into a duck home and took their eggs!";

  std::string line_16 = "You need to get all the eggs! But watch out for the swans,";
  std::string line_17 = "who've been swayed by this real Hitler of a swan. And also";
  std::string line_18 = "the birds. They're just kind of crazy. We're not really";
  std::string line_othermid = "sure why.";

  std::string line_19 = "PRESS <SPACE> TO PLAY!";

  IOmod::getInstance().writeText(line_1, 50, 50);
  IOmod::getInstance().writeText(line_2, 175, 75);

  IOmod::getInstance().writeText(line_3, 5, 125);

  IOmod::getInstance().writeText(line_7, 5, 175);
  IOmod::getInstance().writeText(line_8, 5, 200);
  IOmod::getInstance().writeText(line_9, 5, 225);
  IOmod::getInstance().writeText(line_10, 5, 250);
  IOmod::getInstance().writeText(line_11, 5, 275);
  IOmod::getInstance().writeText(line_mid, 5, 300);

  IOmod::getInstance().writeText(line_12, 5, 350);
  IOmod::getInstance().writeText(line_13, 5, 375);
  IOmod::getInstance().writeText(line_14, 5, 400);
  IOmod::getInstance().writeText(line_15, 5, 425);

  IOmod::getInstance().writeText(line_16, 5, 475);
  IOmod::getInstance().writeText(line_17, 5, 500);
  IOmod::getInstance().writeText(line_18, 5, 525);
  IOmod::getInstance().writeText(line_othermid, 25, 550);

  IOmod::getInstance().setWriteColor({255, 0, 0, 0});
  IOmod::getInstance().writeText(line_19, 150, 600);
}

void Viewport::update() {
  const float x = objectToTrack->getX();
  const float y = objectToTrack->getY();

  position[0] = (x + objWidth/2) - viewWidth/2;
  position[1] = (y + objHeight/2) - viewHeight/2;
  if (position[0] < 0) position[0] = 0;
  if (position[1] < 0) position[1] = 0;
  if (position[0] > (worldWidth - viewWidth)) {
    position[0] = worldWidth-viewWidth;
  }
  if (position[1] > (worldHeight - viewHeight)) {
    position[1] = worldHeight-viewHeight;
  }
}

/*
void Viewport::drawHUD(SDL_Renderer * renderer) const {

  SDL_Rect r = { viewWidth - 255, 25, 215, 235};
  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
  SDL_SetRenderDrawColor( renderer, 255, 255, 255, 255/2 );
  SDL_RenderFillRect(renderer, &r);

  int x = viewWidth - 250;
  IOmod::getInstance().setWriteColor({255, 0, 0, 0}); IOmod::getInstance().writeText("CONTROLS", x, 30);
  IOmod::getInstance().writeText("w:", x, 60);
  IOmod::getInstance().writeText("a:", x, 90);
  IOmod::getInstance().writeText("s:", x, 120);
  IOmod::getInstance().writeText("d:", x, 150);
  IOmod::getInstance().writeText("ESC:", x, 180);
  IOmod::getInstance().writeText("F1:", x, 210);

  IOmod::getInstance().writeText("up", x+80, 60);
  IOmod::getInstance().writeText("left", x+80, 90);
  IOmod::getInstance().writeText("down", x+80, 120);
  IOmod::getInstance().writeText("right", x+80, 150);
  IOmod::getInstance().writeText("exit", x+80, 180);
  IOmod::getInstance().writeText("show this", x+80, 210);
  IOmod::getInstance().writeText("menu again", x+80, 230);
}
*/

// void Viewport::drawCollisionText() const {
//   IOmod::getInstance().setWriteColor({255, 0, 0, 0});
//   IOmod::getInstance().writeText("COLLISION DETECTED", 30, worldHeight-30);
// }
