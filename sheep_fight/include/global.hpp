#ifndef GLOBAL_HPP
#define GLOBAL_HPP

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <fstream>
#include <iostream>
#include <math.h>
#include <queue>
#include <random>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;
using namespace sf;

const int WIDTH = 1024;
const int HEIGHT = 1024;
const int FRAME_RATE = 144;
const int ANIMALS_PER_LINE = 3;

constexpr int COOLDOWN_MS = 2000;
constexpr int INITIAL_HEALTH = 400;
constexpr int CONSTANT_SPEED = 250;

const string PICS_PATH = "./files/pics/", AUDIO_PATH = "./files/audio/",
             FONTS_PATH = "./files/fonts/";

enum AnimalState { Standing, Moving };
enum PlayerNum { Player1, Player2 };
const float TARGET_LINES[]{200.f, 400.f, 600.f, 825.f};

const float RETREAT_SPEED = 200.0f;
struct AnimalConfig {
  std::string name;
  int damage;
  int strength;
  int displayProb;
  string movingPath;
  string standingPath;
  int playerNum;
};
const std::vector<AnimalConfig> ANIMAL_CONFIGS = {
    {"Black Sheep", 50, 50, 5, "move-black-sheep.png", "stand-black-sheep.png",
     1},
    {"Black Goat", 30, 150, 3, "move-black-goat.png", "stand-black-goat.png",
     1},
    {"Black Pig", 10, 250, 2, "move-black-pig.png", "stand-black-pig.png", 1},
    {"White Sheep", 50, 50, 5, "move-white-sheep.png", "stand-white-sheep.png",
     0},
    {"White Goat", 30, 150, 3, "move-white-goat.png", "stand-white-goat.png",
     0},
    {"White Pig", 10, 250, 2, "move-white-pig.png", "stand-white-pig.png", 0},

};

#define debug(x) cout << x << endl, exit(0);

#endif