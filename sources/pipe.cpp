#include "pipe.h"
#include "utils.h"

Pipe::Pipe()
{
	position_x = 0.;
	position_y = 0.;
}

Pipe::Pipe(const Pipe& pipe)
{
	position_x = pipe.position_x;
	position_y = pipe.position_y;
	up = pipe.up;
	down = pipe.down;
}

Pipe::Pipe(const sf::Texture& pipe_texture_up, const sf::Texture& pipe_texture_down)
{
	position_x = 1920. * RESIZE;
	position_y = ((double)rand() / (double)RAND_MAX) * ((1080. * RESIZE) - PIPE_RAND_LIMIT * 2. - PIPE_GAP_Y) + PIPE_RAND_LIMIT;

	up.setTexture(&pipe_texture_up);
	up.setSize(sf::Vector2f(PIPE_SIZE_X, PIPE_SIZE_Y));
	up.setOrigin(0., PIPE_SIZE_Y);
	up.setPosition(position_x, position_y);

	down.setTexture(&pipe_texture_down);
	down.setSize(sf::Vector2f(PIPE_SIZE_X, PIPE_SIZE_Y));
	down.setOrigin(0., 0.);
	down.setPosition(position_x, position_y + PIPE_GAP_Y);
}

void Pipe::operator=(const Pipe& pipe)
{
	position_x = pipe.position_x;
	position_y = pipe.position_y;
	up = pipe.up;
	down = pipe.down;
}

void Pipe::update()
{
	position_x -= PIPE_SPEED * TIME_STEP;
	up.setPosition(position_x, up.getPosition().y);
	down.setPosition(position_x, down.getPosition().y);
}

void Pipe::draw(sf::RenderWindow& window)
{
	window.draw(up);
	window.draw(down);
}
