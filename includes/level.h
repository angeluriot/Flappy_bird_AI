#ifndef LEVEL_H
#define LEVEL_H
#include <vector>
#include "bird.h"
#include "pipe.h"
#include <SFML/Graphics.hpp>

class Level
{

public:

	sf::Texture bird_texture;
	std::vector<Bird> birds;
	int nb_birds;
	sf::Texture pipe_texture_up;
	sf::Texture pipe_texture_down;
	std::vector<Pipe> pipes;
	int next_pipe;
	int scored_pipe;
	int temp;
	int score;
	sf::Font font;
	sf::Text score_text;
	sf::Text nb_birds_text;
	std::vector<std::vector<sf::CircleShape>> neurons;
	std::vector<std::vector<sf::RectangleShape>> lines;
	int bird_shown;
	int generation;
	sf::Text generation_text;

	Level();
	Level(const Level& level);
	Level(int nb_birds);

	void operator=(const Level& level);

	void update_pipes();
	void update_birds(bool manual_input);
	void restart();
	void update(bool manual_input);
	void draw(sf::RenderWindow& window);
};

#endif