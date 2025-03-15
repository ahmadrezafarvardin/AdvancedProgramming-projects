#ifndef GAME_HPP
#define GAME_HPP

#include "Animal.hpp"
#include "Player.hpp"
#include "global.hpp"

enum State { IN_GAME, GAME_OVER, EXIT };

class Game {
public:
  Game(int width, int height);
  ~Game();
  void run();
  void checkGameOver();
  void renderGameOver();

private:
  State state;
  Player *winner;
  RenderWindow window;
  Texture backgroundTexture;
  Sprite backgroundSprite;
  Font font;
  void render();
  void update();
  void handleEvents();
  void loadBackground(string filePath);
  Player player1;
  Player player2;
  Clock clock;
  float deltaTime;
  void checkCollision();
  void checkReachedOpponentSide();
  void checkLineCollision(int lineIndex);
  void resolveOverlap(Animal *anim1, Animal *anim2);
  void handleColliding(vector<Animal *> &p1Group, vector<Animal *> &p2Goup);
  void restartGame();
  Music backgroundMusic;
  void loadAndPlayMusic();
};
#endif