#include "../include/Player.hpp"

Player::Player(string name, PlayerNum playerNum)
    : name(name), life(INITIAL_HEALTH), playerNum(playerNum), selectedLine(0),
      canSendAnimal(true) {
  isLeftSide = (playerNum == Player1);

  setPlayerLine();
  initLine();
  setCursor();

  if (!font.loadFromFile(FONTS_PATH + "GAME_glm.ttf")) {
    debug("Failed to load font");
  }
}

Player::~Player() {}

void Player::setCursor() {
  cursorSprite.setTexture(
      TextureManager::getInstance().getTexture(PICS_PATH + "pointer.png"));
  cursorSprite.setScale(1.0f, 1.0f);
  sf::FloatRect bounds = cursorSprite.getLocalBounds();
  cursorSprite.setOrigin(bounds.width / 2, bounds.height / 2);

  if (isLeftSide) {
    cursorSprite.setRotation(180);
  }
  updateCursorPosition();
}

void Player::renderCursor(RenderWindow &window) {
  if (checkCooldown()) {
    cursorSprite.setColor(sf::Color::White);
  } else {
    float cooldownProgress =
        cooldownClock.getElapsedTime().asMilliseconds() / (float)COOLDOWN_MS;
    sf::Uint8 alpha = static_cast<sf::Uint8>(255 * cooldownProgress);
    cursorSprite.setColor(sf::Color(255, 255, 255, alpha));
  }
  window.draw(cursorSprite);
}

void Player::updateCursorPosition() {
  float cursorX = isLeftSide ? 30.f : WIDTH - 70.f;
  float cursorY = TARGET_LINES[selectedLine];
  cursorSprite.setPosition(cursorX, cursorY);
}

void Player::addAnimalToLine() {
  float randomNum = (float)rand() / (float)RAND_MAX;
  AnimalConfig chosenConfig;
  bool configFound = false;

  vector<AnimalConfig> validConfigs;
  for (const auto &config : ANIMAL_CONFIGS) {
    if (config.playerNum == playerNum) {
      validConfigs.push_back(config);
    }
  }

  if (randomNum < 0.5) {
    for (const auto &config : validConfigs) {
      if (config.displayProb == 5) {
        chosenConfig = config;
        configFound = true;
        break;
      }
    }
  } else if (randomNum < 0.8) {
    for (const auto &config : validConfigs) {
      if (config.displayProb == 3) {
        chosenConfig = config;
        configFound = true;
        break;
      }
    }
  }

  if (!configFound) {
    for (const auto &config : validConfigs) {
      if (config.displayProb == 2) {
        chosenConfig = config;
        break;
      }
    }
  }

  Animal newAnimal(chosenConfig);
  float xOffset = isLeftSide ? (animalsInLine.size() * 100.f)
                             : -(animalsInLine.size() * 100.f);

  newAnimal.setPosition(startX + xOffset, startY);
  newAnimal.setState(AnimalState::Standing);
  animalsInLine.push_back(newAnimal);
}

void Player::initLine() {
  for (int i = 0; i < ANIMALS_PER_LINE; i++) {
    addAnimalToLine();
  }
}

void Player::showLife(RenderWindow &window) {
  Text lifeText;
  lifeText.setFont(font);
  lifeText.setString("Life: " + to_string(life));
  lifeText.setCharacterSize(48);
  lifeText.setFillColor(Color::White);
  if (isLeftSide) {
    lifeText.setPosition(170.f, 60.f);
  } else {
    lifeText.setPosition(664.f, 60.f);
  }

  window.draw(lifeText);
}

void Player::render(RenderWindow &window) {
  showLife(window);
  for (auto &animal : animalsInLine) {
    animal.render(window);
  }

  for (auto &animal : movingAnimals) {
    animal.render(window);
  }
  renderCursor(window);
}

void Player::update(float deltaTime) {
  for (auto it = movingAnimals.begin(); it != movingAnimals.end();) {
    it->update(deltaTime);
    if (it->hasReachedTarget()) {
      ++it;
    } else if (it->hasReachedOwnZone(isLeftSide)) {
      it = movingAnimals.erase(it);
    } else {
      ++it;
    }
  }
  handleInput();
}

void Player::setPlayerLine() {
  if (isLeftSide) {
    startX = 160.f;
    startY = 950.f;
  } else {
    startX = 764.f;
    startY = 950.f;
  }
}

void Player::selectAnimal() {
  if (animalsInLine.empty() || !checkCooldown())
    return;

  Animal frontAnimal = animalsInLine.front();
  animalsInLine.erase(animalsInLine.begin());

  float targetX = isLeftSide ? 924.f : -890.f;
  float starterX = isLeftSide ? 100.f : 890.f;
  float targetY = TARGET_LINES[selectedLine];

  frontAnimal.setPosition(starterX, targetY);
  frontAnimal.setTargetPosition(targetX, targetY);
  frontAnimal.setState(Moving);
  movingAnimals.push_back(frontAnimal);

  addAnimalToLine();

  for (size_t i = 0; i < animalsInLine.size(); i++) {
    float xOffset = isLeftSide ? (i * 100.f) : -(i * 100.f);
    animalsInLine[i].setPosition(startX + xOffset, startY);
  }
  resetCooldown();
}

void Player::handleInput() {
  Keyboard::Key moveUp, moveDown, actionKey;

  if (playerNum == PlayerNum::Player2) {
    moveUp = Keyboard::Up;
    moveDown = Keyboard::Down;
    actionKey = Keyboard::Return;
  } else {
    moveUp = Keyboard::W;
    moveDown = Keyboard::S;
    actionKey = Keyboard::Space;
  }

  if (Keyboard::isKeyPressed(moveDown)) {
    if (!downKeyPressed) {
      if (selectedLine < 3) {
        selectedLine++;
        updateCursorPosition();
      }
      downKeyPressed = true;
    }
  } else {
    downKeyPressed = false;
  }

  if (Keyboard::isKeyPressed(moveUp)) {
    if (!upKeyPressed) {
      if (selectedLine > 0) {
        selectedLine--;
        updateCursorPosition();
      }
      upKeyPressed = true;
    }
  } else {
    upKeyPressed = false;
  }

  if (Keyboard::isKeyPressed(actionKey)) {
    if (!actionKeyPressed) {
      selectAnimal();
      actionKeyPressed = true;
    }
  } else {
    actionKeyPressed = false;
  }
}

void Player::checkOpponentZone(Player &opponent) {
  float boundry = ((playerNum == Player1) ? WIDTH - 100 : 100);
  for (auto it = movingAnimals.begin(); it != movingAnimals.end();) {
    if (((playerNum == Player1) and it->getPosition().x >= boundry) ||
        ((playerNum == Player2) and it->getPosition().x <= boundry)) {
      opponent.cahngeLife(it->getDamage());
      it = movingAnimals.erase(it);
    } else {
      ++it;
    }
  }
}

void Player::cahngeLife(int damage) { life -= damage; }

int Player::getLife() { return life; }

vector<vector<Animal *>> Player::makePerLineGroup(int lineIdx) {
  vector<vector<Animal *>> playerGroups;
  for (auto &animal : movingAnimals) {
    if (abs(animal.getPosition().y - TARGET_LINES[lineIdx]) < 10.0f) {
      bool addedToGroup = false;
      for (auto &group : playerGroups) {
        for (auto &groupAnimal : group) {
          if (abs(animal.getPosition().x - groupAnimal->getPosition().x) <
              50.0f) {
            group.push_back(&animal);
            addedToGroup = true;
            break;
          }
        }
        if (addedToGroup)
          break;
      }

      if (!addedToGroup) {
        playerGroups.push_back({&animal});
      }
    }
  }
  return playerGroups;
}

bool Player::checkCooldown() {
  if (canSendAnimal) {
    return true;
  }
  if (cooldownClock.getElapsedTime().asMilliseconds() >= COOLDOWN_MS) {
    canSendAnimal = true;
    return true;
  }
  return false;
}

void Player::resetCooldown() {
  canSendAnimal = false;
  cooldownClock.restart();
}

void Player::reset() {
  life = INITIAL_HEALTH;
  selectedLine = 0;
  animalsInLine.clear();
  movingAnimals.clear();
  initLine();
  canSendAnimal = true;
  cooldownClock.restart();
  updateCursorPosition();
}
