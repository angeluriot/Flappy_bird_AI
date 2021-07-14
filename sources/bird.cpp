#include "bird.h"
#include "level.h"
#include "utils.h"
#include <iostream>

Bird::Bird()
{
	position_x = 0.;
	position_y = 0.;
	speed = 0.;
	score = 0;
	alive = false;
	brain = Network();
}

Bird::Bird(const Bird& bird)
{
	position_x = bird.position_x;
	position_y = bird.position_y;
	speed = bird.speed;
	score = bird.score;
	alive = bird.alive;
	brain = bird.brain;
	body = bird.body;
}

Bird::Bird(const sf::Texture& bird_texture)
{
	position_x = BIRD_POSITION_X;
	position_y = BIRD_POSITION_Y;
	speed = 0.;
	score = 0;
	alive = true;
	brain = Network(NETWORK_STRUCTURE);
	body.setTexture(&bird_texture);
	body.setSize(sf::Vector2f(BIRD_SIZE_X, BIRD_SIZE_Y));
	body.setOrigin(BIRD_SIZE_X / 2., BIRD_SIZE_Y / 2.);
	body.setPosition(position_x, this->position_y);
}

void Bird::operator=(const Bird& bird)
{
	position_x = bird.position_x;
	position_y = bird.position_y;
	speed = bird.speed;
	score = bird.score;
	alive = bird.alive;
	brain = bird.brain;
	body = bird.body;
}

void Bird::jump()
{
	speed = JUMP_SPEED;
}

void Bird::update_position()
{
	position_y += speed * TIME_STEP;
}

void Bird::update_speed()
{
	speed += GRAVITY * TIME_STEP;
}

void Bird::update_rotation()
{
	if (speed < 0)
		body.rotate((speed / RESIZE) * TIME_STEP);

	if (speed > 0)
		body.rotate((speed / RESIZE) * TIME_STEP * 0.3);

	if (body.getRotation() > 70 && body.getRotation() < 70 + 20)
		body.setRotation(70);

	if (body.getRotation() < 360 - 30 && body.getRotation() > 360 - (30 + 20))
		body.setRotation(360 - 30);
}

void Bird::update_alive(Level& level)
{
	if (position_x + BIRD_SIZE_X / 2. > level.pipes[level.next_pipe].position_x + PIPE_ADJUSTMENT &&
		position_x - BIRD_SIZE_X / 2. < level.pipes[level.next_pipe].position_x + PIPE_SIZE_X - PIPE_ADJUSTMENT)
	{
		if (position_y - BIRD_SIZE_Y / 2. < level.pipes[level.next_pipe].position_y ||
			position_y + BIRD_SIZE_Y / 2. > level.pipes[level.next_pipe].position_y + PIPE_GAP_Y)
		{
			alive = false;
			level.nb_birds -= 1;
		}
	}

	else if (position_y - BIRD_SIZE_Y / 2. < 0 || position_y + BIRD_SIZE_Y / 2. > 1080 * RESIZE)
	{
		alive = false;
		level.nb_birds -= 1;
	}
}


void Bird::think(const Level& level)
{
	double input_1 = normalize(speed, -30 * RESIZE, 100 * RESIZE);
	double input_2 = level.pipes[level.next_pipe].position_x - (position_x + BIRD_SIZE_X / 2.);

	if (input_2 < 0.)
		input_2 = 0.;

	input_2 = normalize(input_2, 0, PIPE_GAP_X - BIRD_SIZE_X);

	double input_3 = (position_y - BIRD_SIZE_Y / 2.) - level.pipes[level.next_pipe].position_y;
	input_3 = normalize(input_3, - ((1080 * RESIZE) - PIPE_RAND_LIMIT - PIPE_GAP_Y), (1080 * RESIZE) - PIPE_RAND_LIMIT - BIRD_SIZE_Y);

	double input_4 = (level.pipes[level.next_pipe].position_y + PIPE_GAP_Y) - (position_y + BIRD_SIZE_Y / 2.);
	input_4 = normalize(input_4, -((1080 * RESIZE) - PIPE_RAND_LIMIT - PIPE_GAP_Y), (1080 * RESIZE) - PIPE_RAND_LIMIT - BIRD_SIZE_Y);

	brain.update_outputs({ input_1, input_2, input_3, input_4 });
}

void Bird::update(bool manual_input, Level& level)
{
	if (alive)
	{
		score++;
		update_speed();
		update_position();
		update_rotation();

		think(level);

		if (manual_input or brain.is_positive(0))
			jump();

		if (level.next_pipe != -1)
			update_alive(level);
	}

	else
		position_x -= PIPE_SPEED * TIME_STEP;

	body.setPosition(position_x, position_y);
}

void Bird::recreate(bool mutate)
{
	position_x = BIRD_POSITION_X;
	position_y = BIRD_POSITION_Y;
	speed = 0.;
	alive = true;

	if (mutate)
		brain.mutate(score / (PIPE_SIZE_X + PIPE_GAP_X));

	score = 0;
	body.setPosition(position_x, this->position_y);
}

void Bird::draw(sf::RenderWindow& window)
{
	window.draw(body);
}