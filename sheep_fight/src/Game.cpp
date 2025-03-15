#include "../include/Game.hpp"

int calcStrength(vector<Animal *> &group) {
  int sumStrength = 0;
  for (auto &animal : group) {
    sumStrength += animal->getStrength();
  };
  return sumStrength;
}

Game::Game(int width, int height)
    : player1("Player 1", Player1), player2("Player 2", Player2) {
  window.create(VideoMode(width, height), "Sheep Fight", Style::Close);
  window.setFramerateLimit(FRAME_RATE);
  state = IN_GAME;
  loadBackground(PICS_PATH + "background.png");
  loadAndPlayMusic();
}

Game::~Game() { backgroundMusic.stop(); }

void Game::run() {
  while (window.isOpen() && state != EXIT) {
    deltaTime = clock.restart().asSeconds();

    handleEvents();

    switch (state) {
    case IN_GAME:
      update();
      break;
    case GAME_OVER:
      break;
    default:
      break;
    }

    render();
  }
}

void Game::handleEvents() {
  Event event;
  while (window.pollEvent(event)) {
    switch (event.type) {
    case Event::Closed:
      window.close();
      state = EXIT;
      break;
    case Event::KeyPressed:
      if (state == GAME_OVER) {
        if (event.key.code == sf::Keyboard::R) {
          restartGame();
        } else if (event.key.code == sf::Keyboard::Escape) {
          state = EXIT;
        }
      }
      break;
    default:
      break;
    }
  }
}

void Game::restartGame() {
  player1.reset();
  player2.reset();
  state = IN_GAME;
  winner = nullptr;
}

void Game::loadBackground(string filePath) {
  if (!backgroundTexture.loadFromFile(filePath)) {
    debug("Failed to load background image");
  } else {
    backgroundSprite.setTexture(backgroundTexture);
  }
}

void Game::update() {
  if (state == IN_GAME) {
    player1.update(deltaTime);
    player2.update(deltaTime);

    checkCollision();
    checkGameOver();
  }
}

void Game::checkCollision() {
  checkReachedOpponentSide();

  for (int line = 0; line < 4; line++) {
    checkLineCollision(line);
  }
}

void Game::checkReachedOpponentSide() {
  player1.checkOpponentZone(player2);
  player2.checkOpponentZone(player1);
}

void Game::resolveOverlap(Animal *anim1, Animal *anim2) {
  FloatRect bounds1 = anim1->getGlobalBounds();
  FloatRect bounds2 = anim2->getGlobalBounds();

  float overlap = 0;
  if (anim1->getPosition().x < anim2->getPosition().x) {
    overlap = (bounds1.left + bounds1.width) - bounds2.left;
  } else {
    overlap = (bounds2.left + bounds2.width) - bounds1.left;
  }

  if (overlap > 0) {
    float totalStrength = anim1->getStrength() + anim2->getStrength();
    float anim1Ratio = anim1->getStrength() / totalStrength;
    float anim2Ratio = anim2->getStrength() / totalStrength;

    anim1->move(-overlap * (1 - anim1Ratio), 0);
    anim2->move(overlap * (1 - anim2Ratio), 0);
  }
}

void Game::checkLineCollision(int lineIndex) {
  vector<vector<Animal *>> player1Groups = player1.makePerLineGroup(lineIndex);
  vector<vector<Animal *>> player2Groups = player2.makePerLineGroup(lineIndex);

  for (auto &p1Group : player1Groups) {
    for (auto &p2Group : player2Groups) {
      bool groupsColliding = false;

      for (auto &p1Animal : p1Group) {
        for (auto &p2Animal : p2Group) {
          if (p1Animal->circularCollision(*p2Animal)) {
            resolveOverlap(p1Animal, p2Animal);
            groupsColliding = true;
            break;
          }
        }
        if (groupsColliding)
          break;
      }
      if (groupsColliding) {
        handleColliding(p1Group, p2Group);
      }
    }
  }
}

void Game::handleColliding(vector<Animal *> &p1Group,
                           vector<Animal *> &p2Goup) {
  int p1GroupStrength = calcStrength(p1Group);
  int p2GroupStrength = calcStrength(p2Goup);

  if (p1GroupStrength > p2GroupStrength) {
    for (auto &animal : p2Goup) {
      animal->setPushedBack(true);
      animal->setSpeed(-animal->getSpeed() * 1.5f);
    }
  } else if (p2GroupStrength > p1GroupStrength) {
    for (auto &animal : p1Group) {
      animal->setPushedBack(true);
      animal->setSpeed(-animal->getSpeed() * 1.5f);
    }
  }
}

void Game::render() {
  window.clear();
  window.draw(backgroundSprite);

  player1.render(window);
  player2.render(window);

  if (state == GAME_OVER) {
    renderGameOver();
  }

  window.display();
}
void Game::checkGameOver() {
  if (player1.getLife() <= 0) {
    state = State::GAME_OVER;
    winner = &player2;
  } else if (player2.getLife() <= 0) {
    state = State::GAME_OVER;
    winner = &player1;
  }
}

void Game::renderGameOver() {
  if (!font.loadFromFile(FONTS_PATH + "GAME_glm.ttf")) {
    debug("Failed to load font");
  }
  Text gameOverText;
  gameOverText.setFont(font);
  gameOverText.setCharacterSize(50);
  gameOverText.setFillColor(sf::Color::White);
  gameOverText.setString(winner->getName() + " Wins!");

  FloatRect textRect = gameOverText.getLocalBounds();
  gameOverText.setOrigin(textRect.left + textRect.width / 2.0f,
                         textRect.top + textRect.height / 2.0f);
  gameOverText.setPosition(sf::Vector2f(WIDTH / 2.0f, HEIGHT / 2.0f));

  window.draw(gameOverText);
}

void Game::loadAndPlayMusic() {
  if (!backgroundMusic.openFromFile(AUDIO_PATH + "sheepfight.ogg")) {
    debug("Failed to load background music");
  } else {
    backgroundMusic.setLoop(true);
    backgroundMusic.play();
  }
}