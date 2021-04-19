#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <vector>
#include <random>
#include <iostream>
#include <iomanip>

//Direction the player is moving.
enum PLAYER_DIRECTION {
    UP,
    DOWN,
    LEFT,
    RIGHT
}direction(RIGHT);

struct SNAKE {

    //We need some variables to create the snake.
    //So, the default constructor is rendered useless
    //and is therefore deleted.
    SNAKE() = delete;

    //Constructor
    SNAKE(const sf::RectangleShape & shape, size_t initial_size) :
    m_initSize(initial_size), m_segments() {
        int size_x = shape.getSize().x;
        int size_y = shape.getSize().y;
        int pos_x = shape.getPosition().x;
        int pos_y = shape.getPosition().y;

        //Add the initial segments to the segments vector
        //and create the initial body and the including the head.
        for(size_t i = 0; i < initial_size; ++i) {
            sf::RectangleShape segment(shape); //Current segment
            segment.setPosition(pos_x - (i*size_x), pos_y); //Set the position based on 
                                                            //the index, size and position.
            sf::Color color;//Colour of the segment.
                            //Create a gradient-like effect on the snake.
            color.b = i * 5;
            color.g = 256 - color.b;

            segment.setFillColor(color);
            m_segments.push_back(segment);
        }
    }

    //Updates the position of the head and the body(segments).
    void update(const sf::Vector2f & pos) {
        //Access the segments from the back and assign the position of each segment(except the head)
        //the position of the segment before it(m_segments[i-1]).
        for(size_t i = (m_segments.size() - 1); i > 0; --i) {
            sf::Vector2f prev_segment_pos = m_segments[i-1].getPosition();
            m_segments[i].setPosition(prev_segment_pos);
        }
        //Set the position of the head
        m_segments[0].setPosition(pos);
    }

    //Draws all the segments.
    void draw(sf::RenderWindow & window) {
        for(auto & segment : m_segments)
            window.draw(segment);
    }

    sf::Vector2f getPos() const {
        return m_segments[0].getPosition();
    }

    int getScore() const {
        return m_segments.size() - m_initSize;
    }

    void addScore() {
        sf::RectangleShape segment(m_segments[m_segments.size() - 1]);
        sf::Vector2f position = segment.getPosition();
        int size = segment.getSize().x;

        switch(direction) {
            case UP:
                position.y -= size;
                break;
            case DOWN:
                position.y += size;
                break;
            case LEFT:
                position.x -= size;
                break;
            case RIGHT:
                position.x += size;
                break;
        };

        segment.setPosition(position);
        m_segments.push_back(segment);
    }

private:
    //Initial size of the player.
    size_t m_initSize;

    //Segments is a vector of RectangleShape objects. The snake is drawn by drawing
    //each RectangleShape in this vector.
    std::vector<sf::RectangleShape> m_segments;
};

//Food object
struct FOOD {
    //Since the food is created by copying a sf::RectangleShape,
    //and a seed for getting random positions,
    //the default constructor is deleted. 
    FOOD() = delete;

    //Constructor
    FOOD(const sf::RectangleShape & shape, int seed, int roof, int floor) :
    m_food(shape), m_engine(), m_roof(roof), m_floor(floor) {
        m_engine.seed(seed);
    }

    //Get a random position
    void update() {
        sf::Vector2f position;
        int size_x = m_food.getSize().x;
        
        //Random distribution
        std::uniform_int_distribution<int> distribution(m_floor/size_x, m_roof/size_x);
        position.x = distribution(m_engine) * size_x;
        position.y = distribution(m_engine) * size_x;
        m_food.setPosition(position);
    }

    void draw(sf::RenderWindow & window) {
        window.draw(m_food);
    }

    sf::Vector2f getPos() const {
        return m_food.getPosition();
    }

private:
    sf::RectangleShape m_food; //Food shape

    int m_roof;
    int m_floor;

    std::default_random_engine m_engine; //Random engine
};

//Manages keyboard input. Changes direction based on the keyboard input
void keyboard_input() {
    if((sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) && direction != DOWN)
        direction = UP;
    if((sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) && direction != RIGHT)
        direction = LEFT;
    if((sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) && direction != LEFT)
        direction = RIGHT;
    if((sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) && direction != UP)
        direction = DOWN;
}

int main(int argc, char * argv[])
{
    sf::RenderWindow window(sf::VideoMode(400, 400), "Snake Game");
    
    //By player position, I mean the position of the 
    //snake(player)'s head (or the first segment).
    //The initial position is half the window width and window height.
    sf::Vector2f player_position(200, 200);

    //Initial shape defines each segment of the player.
    //Its position is where the player's head comes in
    //the first frame.
    sf::RectangleShape initial_shape(sf::Vector2f(10, 10));
    initial_shape.setPosition(player_position);

    int seed = 5;
    if(argc > 1) {
        if(std::atoi(argv[1]))
            seed = std::atoi(argv[1]);
    }
    else
        std::cout << "You can input your own seed as argument!" << std::endl;
    
    SNAKE player(initial_shape, 5); //Player object
    initial_shape.setFillColor(sf::Color::Red);

    std::cout << "Current seed: " << seed << std::endl;

    FOOD food(initial_shape, seed, 399, 0);
    food.update();

    std::ios::sync_with_stdio(false);

    while(window.isOpen()) {
        sf::Event event;

        while(window.pollEvent(event)) {
            if(event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::Black);

        keyboard_input();
        //Update the player's position based on the direction
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

        //Maintain the player's position inside the window and make sure that the player doesn't
        //go outside the viewport
        if(player_position.x >= 400)
            player_position.x = 0;
        if(player_position.x < 0)
            player_position.x = 400;

        if(player_position.y >= 400)
            player_position.y = 0;
        if(player_position.y < 0)
            player_position.y = 400;

        if(player.getPos() == food.getPos()) {
            food.update();
            player.addScore();
        }
        //Update the player's segments'(body) positions
        player.update(player_position);
        player.draw(window);
        food.draw(window);
        window.display();
        std::cout << "Score: " << std::setw(4) << std::right << player.getScore() << "\r";
        sf::sleep(sf::milliseconds(75));//Sleep for some time
    }

    std::cout << "Score: " << std::setw(4) << std::right << player.getScore() << std::endl;

    return 0;
}
