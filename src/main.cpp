#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <vector>

struct SNAKE {
    SNAKE() = delete;
    SNAKE(const sf::RectangleShape & shape, size_t initial_size) :
    m_initSize(initial_size), m_segments() {
        int size_x = shape.getSize().x;
        int size_y = shape.getSize().y;
        int pos_x = shape.getPosition().x;
        int pos_y = shape.getPosition().y;

        for(size_t i = 0; i < initial_size; ++i) {
            sf::RectangleShape segment(shape);
            segment.setPosition(pos_x - (i*size_x), pos_y);
            segment.setFillColor(sf::Color::Black);
            m_segments.push_back(segment);
        }
    }

    void draw(sf::RenderWindow & window) {
        for(auto & segment : m_segments)
            window.draw(segment);
    }

private:
    size_t m_initSize;
    std::vector<sf::RectangleShape> m_segments;

};

int main(int argc, char * argv[])
{
    sf::RenderWindow window(sf::VideoMode(400, 400), "Snake Game");
    
    sf::RectangleShape initial_shape(sf::Vector2f(10, 10));
    initial_shape.setPosition(200, 200);
    SNAKE player(initial_shape, 3);

    while(window.isOpen()) {
        sf::Event event;

        while(window.pollEvent(event)) {
            if(event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::White);
        player.draw(window);
        window.display();
    }

    return 0;
}
