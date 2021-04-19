#include <Snake.hpp>

int main()
{
    sf::RenderWindow window(sf::VideoMode(400, 400), "Snake Game");

    while(window.isOpen()) {
        sf::Event event;

        while(window.pollEvent(event)) {
            if(event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::White);

        window.display();
    }

    return 0;
}
