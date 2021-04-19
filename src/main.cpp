#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <vector>

enum PLAYER_DIRECTION {
    UP,
    DOWN,
    LEFT,
    RIGHT
}direction;

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

    void update() {
        for(size_t i = (m_segments.size() - 1); i > 0; --i) {
            sf::Vector2f prev_segment_pos = m_segments[i-1].getPosition();
            m_segments[i].setPosition(prev_segment_pos);
        }
    }

    void set_head_pos(const sf::Vector2f & pos) {
        m_segments[0].setPosition(pos);
    }

    void draw(sf::RenderWindow & window) {
        for(auto & segment : m_segments)
            window.draw(segment);
    }

private:
    size_t m_initSize;
    std::vector<sf::RectangleShape> m_segments;

};

void keyboard_input() {
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::W) && direction != DOWN)
        direction = UP;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::A) && direction != RIGHT)
        direction = LEFT;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::D) && direction != LEFT)
        direction = RIGHT;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::S) && direction != UP)
        direction = DOWN;
}

int main(int argc, char * argv[])
{
    sf::RenderWindow window(sf::VideoMode(400, 400), "Snake Game");
    
    sf::Vector2f player_position(200, 200);
    sf::RectangleShape initial_shape(sf::Vector2f(10, 10));
    initial_shape.setPosition(player_position);
    SNAKE player(initial_shape, 5);

    while(window.isOpen()) {
        sf::Event event;

        while(window.pollEvent(event)) {
            if(event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::White);
        keyboard_input();
        switch(direction) {
            case UP:
                player_position.y -= initial_shape.getSize().y;
                break;
            case DOWN:
                player_position.y += initial_shape.getSize().y;
                break;
            case LEFT:
                player_position.x -= initial_shape.getSize().x;
                break;
            case RIGHT:
                player_position.x += initial_shape.getSize().x;
                break;
        };

        if(player_position.x > 400)
            player_position.x = 0;
        if(player_position.x < 0)
            player_position.x = 400;

        if(player_position.y > 400)
            player_position.y = 0;
        if(player_position.y < 0)
            player_position.y = 400;

        player.set_head_pos(player_position);
        player.update();
        player.draw(window);
        window.display();
        sf::sleep(sf::milliseconds(100));
    }

    return 0;
}
