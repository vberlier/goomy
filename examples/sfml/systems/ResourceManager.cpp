#include "main.h"

#ifndef EXAMPLE_SFML_RESOURCES_DIR
#define EXAMPLE_SFML_RESOURCES_DIR ""
#endif

ResourceManager::ResourceManager()
    : directory(EXAMPLE_SFML_RESOURCES_DIR), shouldReload(false) {
    loadResources();
}

void ResourceManager::postupdate() {
    if (shouldReload) {
        loadResources();
    }
}

void ResourceManager::reload() {
    shouldReload = true;
}

void ResourceManager::reload(std::string changeDirectory) {
    directory = std::move(changeDirectory);
    reload();
}

void ResourceManager::loadResources() {
    defaultFont.loadFromFile(directory + "/Roboto-Regular.ttf");
    shouldReload = false;
}

const std::string &ResourceManager::getDirectory() const {
    return directory;
}

const sf::Font &ResourceManager::getDefaultFont() const {
    return defaultFont;
}