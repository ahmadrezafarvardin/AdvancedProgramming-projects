#include "../include/Animal.hpp"

Animal::Animal(AnimalConfig const &config)
    : name(config.name), damage(config.damage), strength(config.strength),
      displayProb(config.displayProb), standingPath(config.standingPath),
      movingPath(config.movingPath), reachedTarget(false) {
  currentState = AnimalState::Standing;
  rect.height = 120;
  rect.width = 120;
  sprite.setPosition(position);
  sprite.setTexture(
      TextureManager::getInstance().getTexture(PICS_PATH + standingPath));
  sprite.setTextureRect(rect);
  sprite.setScale(1.0f, 1.0f);
  sprite.setOrigin(rect.width / 2.f, rect.height / 2.f);
  originalPosition = position;
}

Animal::~Animal() {}

void Animal::update(float deltaTime) {
  switch (currentState) {
  case AnimalState::Standing:
    setState(Standing);
    break;

  case AnimalState::Moving:
    updateMovement(deltaTime);
    break;
  }
}

void Animal::updateMovement(float deltaTime) {
  Vector2f currentPos = sprite.getPosition();

  if (isPushed) {
    float direction = (originalPosition.x > currentPos.x) ? 1.0f : -1.0f;
    float newX = currentPos.x + (direction * RETREAT_SPEED * deltaTime);

    if ((direction > 0 && newX >= originalPosition.x) ||
        (direction < 0 && newX <= originalPosition.x)) {
      sprite.setPosition(originalPosition.x, currentPos.y);
      isPushed = false;
      setState(AnimalState::Standing);
    } else {
      sprite.setPosition(newX, currentPos.y);
    }
    return;
  }

  if (abs(currentPos.x - targetPosition.x) < 0.1f) {
    sprite.setPosition(targetPosition.x, currentPos.y);
    setState(AnimalState::Standing);
    reachedTarget = true;
    return;
  }

  float direction = (targetPosition.x > currentPos.x) ? 1.0f : -1.0f;
  sprite.setOrigin(rect.width / 2.f, rect.height / 2.f);

  if (direction > 0) {
    sprite.setScale(-1.0f, 1.0f);
  } else {
    sprite.setScale(1.0f, 1.0f);
  }

  float newX = currentPos.x + (direction * CONSTANT_SPEED * deltaTime);
  sprite.setPosition(newX, currentPos.y);
}

void Animal::setState(AnimalState newState) {
  currentState = newState;

  switch (currentState) {
  case AnimalState::Standing:
    sprite.setTexture(
        TextureManager::getInstance().getTexture(PICS_PATH + standingPath));
    sprite.setTextureRect(rect);
    break;
  case AnimalState::Moving:
    sprite.setTexture(
        TextureManager::getInstance().getTexture(PICS_PATH + movingPath));
    sprite.setTextureRect(rect);
    break;
  }
}

void Animal::setPushedBack(bool pushed) {
  isPushed = pushed;
  if (pushed) {
    targetPosition.x = isPushed ? originalPosition.x : targetPosition.x;
  }
}

void Animal::setPosition(float x, float y) { sprite.setPosition(x, y); }

void Animal::render(RenderWindow &window) { window.draw(sprite); }

bool Animal::hasReachedTarget() { return reachedTarget; }

void Animal::setTargetPosition(float x, float y) {
  targetPosition.x = x, targetPosition.y = y;
}

bool Animal::hasReachedOwnZone(bool isLeftSide) {
  float boundaryX = isLeftSide ? 100.f : WIDTH - 100.f;
  float currentX = sprite.getPosition().x;
  return (isLeftSide && currentX <= boundaryX) ||
         (!isLeftSide && currentX >= boundaryX);
}

FloatRect Animal::getCollisionBounds() const {
  FloatRect spriteBounds = sprite.getGlobalBounds();
  float reduction = 20.0f;
  return FloatRect(spriteBounds.left + reduction, spriteBounds.top + reduction,
                   spriteBounds.width - (2 * reduction),
                   spriteBounds.height - (2 * reduction));
}

bool Animal::circularCollision(const Animal &other) const {
  Vector2f center1 = getCenter();
  Vector2f center2 = other.getCenter();
  float radius1 = getCollisionRadius();
  float radius2 = other.getCollisionRadius();

  float distanceSquared =
      pow(center1.x - center2.x, 2) + pow(center1.y - center2.y, 2);
  float radiusSum = radius1 + radius2;

  return distanceSquared <= radiusSum * radiusSum;
}

Vector2f Animal::getCenter() const {
  FloatRect bounds = getCollisionBounds();
  return Vector2f(bounds.left + bounds.width / 2,
                  bounds.top + bounds.height / 2);
}

float Animal::getCollisionRadius() const {
  FloatRect bounds = getCollisionBounds();
  return std::min(bounds.width, bounds.height) / 2;
}