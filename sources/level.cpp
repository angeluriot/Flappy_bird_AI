#include "level.h"
#include "bird.h"
#include "utils.h"

Level::Level()
{
	birds.clear();
	nb_birds = 0;
	pipes.clear();
	next_pipe = 0;
	scored_pipe = 0;
	temp = 0;
	score = 0;
	neurons.clear();
	lines.clear();
	bird_shown = 0;
	generation = 1;
}

Level::Level(const Level& level)
{
	bird_texture = level.bird_texture;
	birds = level.birds;
	nb_birds = level.nb_birds;
	pipe_texture_up = level.pipe_texture_up;
	pipes = level.pipes;
	next_pipe = level.next_pipe;
	scored_pipe = level.scored_pipe;
	temp = level.temp;
	score = level.score;
	font = level.font;
	score_text = level.score_text;
	nb_birds_text = level.nb_birds_text;
	neurons = level.neurons;
	lines = level.lines;
	bird_shown = level.bird_shown;
	generation = level.generation;
	generation_text = level.generation_text;
}

Level::Level(int nb_birds)
{
	birds.clear();
	pipes.clear();
	neurons.clear();
	lines.clear();

	bird_texture.loadFromFile("dependencies/resources/bird_texture.png");
	pipe_texture_up.loadFromFile("dependencies/resources/pipe_texture_up.png");
	pipe_texture_down.loadFromFile("dependencies/resources/pipe_texture_down.png");
	font.loadFromFile("dependencies/resources/GoogleSans-Bold.ttf");

	bird_texture.setSmooth(true);
	pipe_texture_up.setSmooth(true);
	pipe_texture_down.setSmooth(true);

	for (int i = 0; i < nb_birds; i++)
		birds.push_back(Bird(bird_texture));

	pipes.push_back(Pipe(pipe_texture_up, pipe_texture_down));

	this->nb_birds = nb_birds;
	next_pipe = 0;
	scored_pipe = 0;
	temp = 0;
	score = 0;
	bird_shown = 0;
	generation = 0;

	score_text.setString(std::to_string(score));
	score_text.setCharacterSize(150. * RESIZE);
	score_text.setFont(font);
	score_text.setFillColor(sf::Color::White);
	score_text.setOrigin(score_text.getLocalBounds().width / 2., score_text.getLocalBounds().height / 2.);
	score_text.setPosition((1920. * RESIZE) / 2., 100. * RESIZE);

	nb_birds_text = score_text;
	nb_birds_text.setCharacterSize(100. * RESIZE);
	nb_birds_text.setString(std::to_string(nb_birds));
	nb_birds_text.setOrigin(0., nb_birds_text.getLocalBounds().height);
	nb_birds_text.setPosition(50. * RESIZE, (1080. * RESIZE) - (80. * RESIZE));

	generation_text = score_text;
	generation_text.setCharacterSize(70. * RESIZE);
	generation_text.setString("Gen : " + std::to_string(generation));
	generation_text.setOrigin(generation_text.getLocalBounds().width, 0.);
	generation_text.setPosition((1920. * RESIZE) - (50. * RESIZE), 20. * RESIZE);

	std::vector<int> layers = NETWORK_STRUCTURE;

	double position_y;
	double position_y_2;

	for (int i = 0; i < layers.size(); i++)
	{
		neurons.push_back({});
		lines.push_back({});

		for (int j = 0; j < layers[i]; j++)
		{
			if (layers[i] > 1)
				position_y = NETWORK_POSITION_Y + (NETWORK_SIZE_Y / ((double)layers[i] - 1)) * j;

			else
				position_y = NETWORK_POSITION_Y + NETWORK_SIZE_Y / 2.;

			neurons[i].push_back(create_neuron(NETWORK_POSITION_X + (NETWORK_SIZE_X / ((double)layers.size() - 1)) * i, position_y));

			if (i != layers.size() - 1)
			{
				for (int k = 0; k < layers[i + 1]; k++)
				{
					if (layers[i + 1] > 1)
						position_y_2 = NETWORK_POSITION_Y + (NETWORK_SIZE_Y / ((double)layers[i + 1] - 1)) * k;

					else
						position_y_2 = NETWORK_POSITION_Y + NETWORK_SIZE_Y / 2.;

					lines[i].push_back(create_line(NETWORK_POSITION_X + (NETWORK_SIZE_X / (double)(layers.size() - 1)) * i, position_y,
						                           NETWORK_POSITION_X + (NETWORK_SIZE_X / (double)(layers.size() - 1)) * (i + 1), position_y_2));
				}
			}
		}
	}

	draw_network(neurons, lines, birds, bird_shown);
}

void Level::operator=(const Level& level)
{
	bird_texture = level.bird_texture;
	birds = level.birds;
	nb_birds = level.nb_birds;
	pipe_texture_up = level.pipe_texture_up;
	pipe_texture_down = level.pipe_texture_down;
	pipes = level.pipes;
	next_pipe = level.next_pipe;
	scored_pipe = level.scored_pipe;
	temp = level.temp;
	score = level.score;
	font = level.font;
	score_text = level.score_text;
	nb_birds_text = level.nb_birds_text;
	neurons = level.neurons;
	lines = level.lines;
	bird_shown = level.bird_shown;
	generation = level.generation;
	generation_text = level.generation_text;
}

void Level::update_pipes()
{
	for (int i = 0; i < pipes.size(); i++)
		pipes[i].update();

	if (pipes[pipes.size() - 1].position_x < (1920. * RESIZE) - (PIPE_SIZE_X + PIPE_GAP_X))
		pipes.push_back(Pipe(pipe_texture_up, pipe_texture_down));

	if (pipes[0].position_x < -PIPE_SIZE_X)
	{
		pipes.erase(pipes.begin());
		temp = temp - 1;
	}

	for (int i = 0; i < pipes.size(); i++)
	{
		if (pipes[i].position_x + PIPE_SIZE_X < BIRD_POSITION_X - BIRD_SIZE_X / 2.)
		{
			next_pipe = i + 1;
			break;
		}

		else if (i == 0)
		{
			next_pipe = i;
			break;
		}
	}

	for (int i = 0; i < pipes.size(); i++)
	{
		if (pipes[i].position_x + PIPE_SIZE_X / 2. < BIRD_POSITION_X + BIRD_SIZE_X / 2.)
		{
			scored_pipe = i + 1;
			break;
		}

		else if (i == 0)
		{
			scored_pipe = i;
			break;
		}
	}

	if (scored_pipe != temp)
		score++;

	temp = scored_pipe;
}

void Level::update_birds(bool manual_input)
{
	bool chosen = false;

	for (int i = 0; i < birds.size(); i++)
	{
		birds[i].update(manual_input, *this);

		if (birds[i].alive and !chosen)
		{
			bird_shown = i;
			chosen = true;
		}
	}
}

void Level::restart()
{
	pipes.clear();
	nb_birds = birds.size();
	bool sorted = false;
	Bird bird_temp;

	while (!sorted)
	{
		sorted = true;

		for (int i = 0; i < birds.size() - 1; i++)
		{
			if (birds[i].score < birds[i + 1].score)
			{
				sorted = false;
				bird_temp = birds[i + 1];
				birds[i + 1] = birds[i];
				birds[i] = bird_temp;
			}
		}
	}

	for (int i = 0; i < nb_birds / 2; i++)
	{
		birds[nb_birds / 2 + i] = birds[i];
		birds[nb_birds / 2 + i].recreate(true);
		birds[i].recreate(false);
	}
		
	pipes.push_back(Pipe(pipe_texture_up, pipe_texture_down));

	next_pipe = 0;
	scored_pipe = 0;
	temp = 0;
	score = 0;
	generation++;

	draw_network(neurons, lines, birds, bird_shown);
}

void Level::update(bool manual_input)
{
	update_pipes();
	update_birds(manual_input);

	score_text.setString(std::to_string(score));
	score_text.setOrigin(score_text.getLocalBounds().width / 2., score_text.getLocalBounds().height / 2.);

	nb_birds_text.setString(std::to_string(nb_birds));
	nb_birds_text.setOrigin(0., nb_birds_text.getLocalBounds().height);

	generation_text.setString("Gen : " + std::to_string(generation));
	generation_text.setOrigin(generation_text.getLocalBounds().width, 0.);

	draw_network(neurons, lines, birds, bird_shown);
}

void Level::draw(sf::RenderWindow& window)
{
	for (int i = 0; i < birds.size(); i++)
	{
		if (birds[i].position_x + BIRD_SIZE_X > 0)
			birds[i].draw(window);
	}

	for (int i = 0; i < pipes.size(); i++)
		pipes[i].draw(window);

	score_text.setFillColor(sf::Color(0, 0, 0, 50));
	score_text.move(0, score_text.getLocalBounds().height / 6.);
	window.draw(score_text);

	score_text.setFillColor(sf::Color::White);
	score_text.move(0, - score_text.getLocalBounds().height / 6.);
	window.draw(score_text);

	nb_birds_text.setFillColor(sf::Color(0, 0, 0, 50));
	nb_birds_text.move(0, nb_birds_text.getLocalBounds().height / 6.);
	window.draw(nb_birds_text);

	nb_birds_text.setFillColor(sf::Color::White);
	nb_birds_text.move(0, -nb_birds_text.getLocalBounds().height / 6.);
	window.draw(nb_birds_text);

	generation_text.setFillColor(sf::Color(0, 0, 0, 50));
	generation_text.move(0, generation_text.getLocalBounds().height / 6.);
	window.draw(generation_text);

	generation_text.setFillColor(sf::Color::White);
	generation_text.move(0, -generation_text.getLocalBounds().height / 6.);
	window.draw(generation_text);

	for (int i = 0; i < lines.size(); i++)
	{
		for (int j = 0; j < lines[i].size(); j++)
			window.draw(lines[i][j]);
	}

	for (int i = 0; i < neurons.size(); i++)
	{
		for (int j = 0; j < neurons[i].size(); j++)
			window.draw(neurons[i][j]);
	}
}