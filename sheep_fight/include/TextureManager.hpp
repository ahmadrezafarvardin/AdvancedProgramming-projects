#include "global.hpp"

class TextureManager {
public:
  static TextureManager &getInstance() {
    static TextureManager instance;
    return instance;
  }

  Texture &getTexture(const string &path) {
    if (textures.find(path) == textures.end()) {
      textures[path] = Texture();
      if (!textures[path].loadFromFile(path)) {
        debug("Failed to load texture: " + path);
      }
    }
    return textures[path];
  }

private:
  TextureManager() {}
  map<string, sf::Texture> textures;
};