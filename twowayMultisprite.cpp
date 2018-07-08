#include "twowayMultisprite.h"
#include "gamedata.h"
#include "renderContext.h"

void TwoWayMultiSprite::advanceFrame(Uint32 ticks) {
	timeSinceLastFrame += ticks;
	if (timeSinceLastFrame > frameInterval) {
    if(currentMode == RIGHT_MODE) 
      currentFrame = (currentFrame+1) % numberOfFramesA;
    else
      currentFrame = (currentFrame+1) % numberOfFramesB;
	  timeSinceLastFrame = 0;
	}
}

TwoWayMultiSprite::TwoWayMultiSprite( const std::string& name) :
  Drawable(name, 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"), 
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")), 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/speedX"),
                    Gamedata::getInstance().getXmlInt(name+"/speedY"))
           ),
  imagesA( RenderContext::getInstance()->getImages(name+"/modeA") ),
  imagesB( RenderContext::getInstance()->getImages(name+"/modeB") ),

  currentMode(RIGHT_MODE),
  currentFrame(0),
  numberOfFramesA( Gamedata::getInstance().getXmlInt(name+"/modeA/frames") ),
  numberOfFramesB( Gamedata::getInstance().getXmlInt(name+"/modeB/frames") ),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval")),
  timeSinceLastFrame( 0 ),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  explosion(nullptr)
{ }

TwoWayMultiSprite::TwoWayMultiSprite(const TwoWayMultiSprite& s) :
  Drawable(s), 
  imagesA(s.imagesA),
  imagesB(s.imagesB),
  currentMode(s.currentMode),
  currentFrame(s.currentFrame),
  numberOfFramesA( s.numberOfFramesA ),
  numberOfFramesB( s.numberOfFramesB ),
  frameInterval( s.frameInterval ),
  timeSinceLastFrame( s.timeSinceLastFrame ),
  worldWidth( s.worldWidth ),
  worldHeight( s.worldHeight ),
  explosion( s.explosion )
  { }

TwoWayMultiSprite& TwoWayMultiSprite::operator=(const TwoWayMultiSprite& s) {
  Drawable::operator=(s);
  imagesA = (s.imagesA);
  imagesB = (s.imagesB);
  currentMode = (s.currentMode);
  currentFrame = (s.currentFrame);
  numberOfFramesA = ( s.numberOfFramesA );
  numberOfFramesB = ( s.numberOfFramesB );
  frameInterval = ( s.frameInterval );
  timeSinceLastFrame = ( s.timeSinceLastFrame );
  worldWidth = ( s.worldWidth );
  worldHeight = ( s.worldHeight );
  explosion = ( s.explosion );
  return *this;
}

void TwoWayMultiSprite::draw() const { 
  if(explosion)
    explosion->draw();
  else {
    if(currentMode == RIGHT_MODE)
      imagesA[currentFrame]->draw(getX(), getY(), getScale());
    else
      imagesB[currentFrame]->draw(getX(), getY(), getScale());
  }
}

void TwoWayMultiSprite::update(Uint32 ticks) { 
  if(explosion) {
    explosion->update(ticks);
    if(explosion->chunkCount() == 0) {
      delete explosion;
      explosion = NULL;
    }
    return;
  }

  advanceFrame(ticks);

  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);

  if ( getY() < 0) {
    setVelocityY( fabs( getVelocityY() ) );
  }
  if ( getY() > worldHeight-getScaledHeight()) {
    setVelocityY( -fabs( getVelocityY() ) );
  }

  if ( getX() < 0) {
    setVelocityX( fabs( getVelocityX() ) );
    //currentMode = RIGHT_MODE;
    TwoWayMultiSprite::switchMode();
  }
  if ( getX() > worldWidth-getScaledWidth()) {
    setVelocityX( -fabs( getVelocityX() ) );
    //currentMode = LEFT_MODE;
    TwoWayMultiSprite::switchMode();
  }  

}

void TwoWayMultiSprite::switchMode() {
  if(currentMode == RIGHT_MODE)
    currentMode = LEFT_MODE;
  else
    currentMode = RIGHT_MODE;
}

void TwoWayMultiSprite::explode() {
  if(!explosion) {
    Image* img = currentMode == RIGHT_MODE ? imagesA[currentFrame] : imagesB[currentFrame];
    Sprite sprite(getName(), getPosition(), getVelocity(), img);
    explosion = new ExplodingSprite(sprite);
  }
}

void TwoWayMultiSprite::reset() {
  Drawable::reset();
  currentMode = RIGHT_MODE;
  currentFrame = 0;
  timeSinceLastFrame = 0;
  delete explosion;
  explosion = nullptr;
}