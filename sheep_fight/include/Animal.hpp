#ifndef Animal_HPP
#define Animal_HPP

#include "TextureManager.hpp"
#include "global.hpp"

class Animal {

private:
  Vector2f position;
  Sprite sprite;
  AnimalState currentState;
  string name;
  int damage;
  int strength;
  float displayProb;
  string standingPath;
  string movingPath;
  IntRect rect;
  Vector2f targetPosition;
  bool reachedTarget;
  bool isPushed = false;
  Vector2f originalPosition;
  float speed;

public:
  Animal(AnimalConfig const &config);
  ~Animal();
  void render(RenderWindow &window);
  void update(float deltaTime);
  void setState(AnimalState newState);
  void setPosition(float x, float y);
  void setTargetPosition(float x, float y);
  bool hasReachedTarget();
  void updateMovement(float deltaTime);
  Vector2f getPosition() const { return sprite.getPosition(); }
  int getStrength() const { return strength; }
  int getDamage() const { return damage; }
  void setPushedBack(bool pushed);
  FloatRect getGlobalBounds() const { return sprite.getGlobalBounds(); }
  void setSpeed(float newSpeed) { speed = newSpeed; }
  float getSpeed() const { return speed; }
  void move(float offsetX, float offsetY) { sprite.move(offsetX, offsetY); }
  bool hasReachedOwnZone(bool isLeftSide);
  FloatRect getCollisionBounds() const;
  bool circularCollision(const Animal &other) const;
  Vector2f getCenter() const;
  float getCollisionRadius() const;
};
#endif
