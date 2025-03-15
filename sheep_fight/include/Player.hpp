#ifndef PLAYER_HPP
#define PLAYER_HPP
#include "Animal.hpp"
#include "global.hpp"

class Player {
private:
  string name;
  int life;
  float startX;
  float startY;
  Font font;
  PlayerNum playerNum;
  int selectedLine;
  bool isLeftSide;
  void setPlayerLine();
  bool upKeyPressed = false;
  bool downKeyPressed = false;
  bool actionKeyPressed = false;
  Sprite cursorSprite;
  Texture cursorTexture;
  Clock cooldownClock;
  bool canSendAnimal;

public:
  Player(string name, PlayerNum playerNum);
  ~Player();
  vector<Animal> animalsInLine;
  vector<Animal> movingAnimals;
  void showLife(RenderWindow &window);
  void render(RenderWindow &window);
  void update(float deltaTime);
  void selectAnimal();
  void handleInput();
  void cahngeLife(int damage);
  void initLine();
  void addAnimalToLine();
  int getLife();
  void checkOpponentZone(Player &opponent);
  vector<vector<Animal *>> makePerLineGroup(int lineIdx);
  void updateCursorPosition();
  void renderCursor(RenderWindow &window);
  void setCursor();
  bool checkCooldown();
  void resetCooldown();
  string getName() { return name; }
  void reset();
};

#endif