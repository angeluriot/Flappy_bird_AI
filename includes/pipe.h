#ifndef PIPE_H
#define PIPE_H
#include <SFML/Graphics.hpp>

class Pipe
{

public:

	double position_x;
	double position_y;
	sf::RectangleShape up;
	sf::RectangleShape down;

	Pipe();
	Pipe(const Pipe& pipe);
	Pipe(const sf::Texture& pipe_texture_up, const sf::Texture& pipe_texture_down);

	void operator=(const Pipe& pipe);

	void update();
	void draw(sf::RenderWindow& window);
};

#endif
