#pragma once

#include "Engine.hpp"
#include <SFML/Graphics.hpp>

class ResourceManager {
  public:
    ResourceManager();
    void onAfterUpdate();

    void reload();
    void reload(std::string directory);

    const std::string &getDirectory() const;
    const sf::Font &getDefaultFont() const;

  private:
    std::string directory;
    bool shouldReload;
    sf::Font defaultFont;

    void loadResources();
};
