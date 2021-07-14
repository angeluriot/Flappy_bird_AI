#include "utils.h"
#include "bird.h"
#include <cmath>

int screen_width;

double sigmoid(const double& number)
{
	return 1. / (1. + exp(-number));
}

double random()
{
	return (((double)rand() / (double)RAND_MAX) * 2.) - 1.;
}

double normalize(const double& number, const double& min, const double& max)
{
	return sigmoid(((number - min) / (max - min)) * 8. - 4.);
}

sf::CircleShape create_neuron(const double& position_x, const double& position_y)
{
	sf::CircleShape neuron;

	neuron.setFillColor(sf::Color::White);
	neuron.setRadius(NEURON_RADIUS);
	neuron.setOrigin(NEURON_RADIUS, NEURON_RADIUS);
	neuron.setPosition(position_x, position_y);

	return neuron;
}

sf::RectangleShape create_line(const double& position_1_x, const double& position_1_y, const double& position_2_x, const double& position_2_y)
{
	sf::RectangleShape line;
	double angle = atan2(position_2_y - position_1_y, position_2_x - position_1_x);
	
	line.setFillColor(sf::Color::White);
	line.setSize(sf::Vector2f((position_2_x - position_1_x) / cos(angle), LINE_MAX));
	line.setOrigin(0., LINE_MAX / 2.);
	line.setPosition(position_1_x, position_1_y);
	line.setRotation(angle * (180. / PI));

	return line;
}

void draw_network(std::vector<std::vector<sf::CircleShape>>& neurons, std::vector<std::vector<sf::RectangleShape>>& lines, const std::vector<Bird>& birds, int bird_shown)
{
	for (int i = 1; i < birds[bird_shown].brain.layers.size(); i++)
	{
		for (int j = 0; j < birds[bird_shown].brain.layers[i].size(); j++)
		{
			if (birds[bird_shown].brain.layers[i][j].add_value > 0.)
				neurons[i][j].setFillColor(sf::Color::Blue);

			else
				neurons[i][j].setFillColor(sf::Color::Red);

			for (int k = 0; k < birds[bird_shown].brain.layers[i][j].weights.size(); k++)
			{
				if (birds[bird_shown].brain.layers[i][j].weights[k] > 0.)
				{
					lines[i - 1][k * birds[bird_shown].brain.layers[i].size() + j].setFillColor(sf::Color::Blue);
					lines[i - 1][k * birds[bird_shown].brain.layers[i].size() + j].setSize(sf::Vector2f(lines[i - 1][k * birds[bird_shown].brain.layers[i].size() + j].getSize().x,
						birds[bird_shown].brain.layers[i][j].weights[k] * LINE_MAX));
					lines[i - 1][k * birds[bird_shown].brain.layers[i].size() + j].setOrigin(lines[i - 1][k * birds[bird_shown].brain.layers[i].size() + j].getOrigin().x,
						lines[i - 1][k * birds[bird_shown].brain.layers[i].size() + j].getSize().y / 2.);
				}

				else
				{
					lines[i - 1][k * birds[bird_shown].brain.layers[i].size() + j].setFillColor(sf::Color::Red);
					lines[i - 1][k * birds[bird_shown].brain.layers[i].size() + j].setSize(sf::Vector2f(lines[i - 1][k * birds[bird_shown].brain.layers[i].size() + j].getSize().x,
						birds[bird_shown].brain.layers[i][j].weights[k] * LINE_MAX * -1));
					lines[i - 1][k * birds[bird_shown].brain.layers[i].size() + j].setOrigin(lines[i - 1][k * birds[bird_shown].brain.layers[i].size() + j].getOrigin().x,
						lines[i - 1][k * birds[bird_shown].brain.layers[i].size() + j].getSize().y / 2.);
				}
			}
		}
	}
}