#include "main.h"
#include <SFML/Graphics.hpp>
#include <cmath>
#include <string>

void Overlay::update(Engine &engine) {
    auto &window = engine.get<Window>();
    auto &resourceManager = engine.get<ResourceManager>();

    sf::Text text(getString(engine), resourceManager.getDefaultFont());
    text.setFillColor(sf::Color(100, 250, 50));
    text.setCharacterSize(16);

    window.draw(text);
}

std::string Overlay::getString(Engine &engine) {
    using namespace std::chrono;

    auto elapsed = duration_cast<seconds>(engine.getAge()).count();

    auto frameDuration =
        duration_cast<milliseconds>(engine.getFrameDuration()).count();

    auto framesPerSecond =
        std::round((float)duration_cast<milliseconds>(seconds(1)).count() /
                   (float)frameDuration);

    return "Elapsed: " + std::to_string(elapsed) + "s\n" +
           "Frame duration: " + std::to_string(frameDuration) + "ms\n" +
           "Frames per second: " + std::to_string((int)framesPerSecond);
}
