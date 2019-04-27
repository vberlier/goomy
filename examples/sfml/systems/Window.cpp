#include "main.hpp"

Window::Window() : sf::RenderWindow(sf::VideoMode(600, 400), "example_sfml") {
    setPosition(sf::Vector2i{1200, 300});
}

void Window::onBeforeUpdate() {
    clear();
}

void Window::onUpdate(Engine &engine) {
    sf::Event event{};

    while (pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            close();
        } else if (event.type == sf::Event::Resized) {
            sf::View view = getView();

            view.setSize(event.size.width, event.size.height);
            view.setCenter((float)event.size.width / 2,
                           (float)event.size.height / 2);

            setView(view);
        } else if (event.type == sf::Event::MouseMoved) {
            engine.dispatch<onMouseMove>(event);
        } else if (event.type == sf::Event::MouseButtonPressed) {
            engine.dispatch<onMouseDown>(event);
        } else if (event.type == sf::Event::MouseButtonReleased) {
            engine.dispatch<onMouseUp>(event);
        }
    }
}

void Window::onAfterUpdate(Engine &engine) {
    if (isOpen()) {
        display();
    } else {
        engine.shutdown();
    }
}