#ifndef BIRD_H
#define BIRD_H
#include "network.h"
#include <SFML/Graphics.hpp>

class Level;

class Bird
{

public:

	double position_x;
	double position_y;
	double speed;
	int score;
	bool alive;
	Network brain;
	sf::RectangleShape body;

	Bird();
	Bird(const Bird& bird);
	Bird(const sf::Texture& bird_texture);
	
	void operator=(const Bird& bird);

	void jump();
	void update_position();
	void update_speed();
	void update_rotation();
	void update_alive(Level& level);
	void think(const Level& level);
	void update(bool manual_input, Level& level);
	void recreate(bool mutate);
	void draw(sf::RenderWindow& window);

};

#endif