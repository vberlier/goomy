#include "main.h"

Window::Window() : sf::RenderWindow(sf::VideoMode(200, 200), "example_sfml") {
}

void Window::preupdate() {
    clear();
}

void Window::update(Engine &engine) {
    sf::Event event{};

    while (pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            close();
            engine.shutdown();
        } else if (event.type == sf::Event::Resized) {
            sf::View view = getView();
            view.setSize(event.size.width, event.size.height);
            setView(view);
        }
    }
}

void Window::postupdate() {
    display();
}