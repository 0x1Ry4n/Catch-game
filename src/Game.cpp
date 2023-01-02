#include "Headers/Game.h"

// Private Functions

void Game::initVariables()
{
	this->window = nullptr;

	// Game logic
	this->endGame = false;
	this->points = 0;
	this->health = 15;
	this->enemySpawnTimerMax = 5.f;
	this->enemySpawnTimer = this->enemySpawnTimerMax;
	this->maxEnemies = 5;
	this->mouseHeld = false;
}

void Game::initWindow()
{
	this->videoMode.height = 800;
	this->videoMode.width = 600;
	this->window = new sf::RenderWindow(sf::VideoMode(this->videoMode), "SFML Catch Game", sf::Style::Titlebar | sf::Style::Close);
	this->window->setFramerateLimit(60);
}

void Game::initEnemies()
{
	this->enemy.setPosition(10.f, 10.f);
	this->enemy.setSize(sf::Vector2f(50.f, 50.f));
	this->enemy.setFillColor(sf::Color::Cyan);
	this->enemy.setOutlineColor(sf::Color::White);
	this->enemy.setOutlineThickness(1.f);
}

void Game::initCursor()
{
	if (this->cursor.loadFromSystem(sf::Cursor::Cross))
		this->window->setMouseCursor(this->cursor);
}

void Game::initFont()
{
	if (!this->font.loadFromFile("content/fonts/Arial.TTF"))
		std::cout << "Error loading font file 'Arial' " << std::endl;

	this->healthText.setFont(this->font);
	this->healthText.setCharacterSize(24.f);
	this->healthText.setFillColor(sf::Color::White);
	this->healthText.setStyle(sf::Text::Bold);
	this->healthText.setPosition(10.f, 10.f);

	this->pointText.setFont(this->font);
	this->pointText.setCharacterSize(24.f);
	this->pointText.setFillColor(sf::Color::White);
	this->pointText.setStyle(sf::Text::Bold);
	this->pointText.setPosition(10.f, 40.f);
}

// Constructors / Destructors

Game::Game()
{
	this->initVariables();
	this->initWindow();
	this->initCursor();
	this->initFont();
	this->initEnemies();
}

Game::~Game()
{
	delete this->window;
}

// Accessors

const bool Game::running() const
{
	return this->window->isOpen();
}

const bool Game::getEndGame() const
{
	return this->endGame;
}

// Functions

void Game::spawnEnemy()
{
	/*
		@return void
		Spawns enemies and sets their colors and positions
		- Sets a random position
		- Sets a random color
		- Adds enemy to the vector
	*/
	this->enemy.setPosition(
		static_cast<float>(rand() % static_cast<int>(this->window->getSize().x - this->enemy.getSize().x)),
		0.f);

	this->enemy.setFillColor(sf::Color(rand() % 255, rand() % 255, rand() % 255));

	// Spawn Enemy
	this->enemies.push_back(this->enemy);

	// Remove enemies at end of screen
}

void Game::pollEvents()
{
	while (this->window->pollEvent(this->event))
	{
		switch (this->event.type)
		{
			case sf::Event::Closed:
				this->window->close();
				break;
			case sf::Event::KeyPressed:
				if (this->event.key.code == sf::Keyboard::Escape)
					this->window->close();
				break;
			default:
				break;
		}
	}
}

void Game::setHealthText(uint health)
{
	this->healthText.setString("Health: " + std::to_string(health));
}

void Game::setPointsText(uint points)
{
	this->pointText.setString("Points: " + std::to_string(points));
}

void Game::updateMousePositions()
{
	/*
		@return void
		Updates the mouse positions:
		- Mouse positions relative to window (Vector2i)
	*/

	this->mousePosWindow = sf::Mouse::getPosition(*this->window);
	this->mousePosView = this->window->mapPixelToCoords(this->mousePosWindow);
}

void Game::updateEnemies()
{
	/*
		@return void
		Updates the enemy spawn timer and spawns enemies
		when the total amount of enemies is smaller than the maximum
		Moves the enemies downwards
		Removes the enemies at the edge of the screen
	*/

	// Updating the timer for enemy spawning

	this->setPointsText(this->points);
	this->setHealthText(this->health);

	if (this->enemies.size() < static_cast<size_t>(this->maxEnemies))
	{
		if (this->enemySpawnTimer >= this->enemySpawnTimerMax)
		{
			// Spawn the enemy and reset the timer
			this->spawnEnemy();
			this->enemySpawnTimer = 0.f;
		}
		else
		{
			this->enemySpawnTimer += 1.f;
		}
	}

	// Moving and updating enemies

	for (size_t i = 0; i < this->enemies.size(); i++)
	{

		this->enemies[i].move(0.f, 2.f);

		// Check if enemy is at the bottom
		if (this->enemies[i].getPosition().y > this->window->getSize().y)
		{
			this->enemies.erase(this->enemies.begin() + i);

			this->health -= 1;

			std::cout << "Health: " << this->health << "\n";

			this->setHealthText(this->health);
		}
	}

	// Check if clicked up on

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		if (this->mouseHeld == false)
		{
			this->mouseHeld = true;
			bool deleted = false;

			for (size_t i = 0; i < this->enemies.size() && deleted == false; i++)
			{
				// Check click in shape enemy
				if (this->enemies[i].getGlobalBounds().contains(this->mousePosView))
				{
					// Delete the enemy
					deleted = true;
					this->enemies.erase(this->enemies.begin() + i);

					// Gain points
					this->points += 1;

					std::cout << "Points: " << this->points << "\n";

					if (this->points >= 20 && this->points < 50)
						this->maxEnemies = 8;
					else if (this->points >= 50 && this->points < 80)
						this->maxEnemies = 10;
					else if (this->points >= 80)
						this->maxEnemies = 12;
					else if (this->points >= 200)
						this->maxEnemies = 15;
					else if (this->points >= 400)
						this->maxEnemies = 20;

					this->setPointsText(this->points);
				}
			}
		}
		else
		{
			this->mouseHeld = false;
		}
	}
}

void Game::update()
{
	this->pollEvents();

	if (this->endGame == false)
	{
		this->updateMousePositions();
		this->updateEnemies();
	}

	// End Game condition
	if (this->health <= 0)
	{
		this->endGame = true;
	}
}

void Game::renderText()
{
	this->window->draw(this->pointText);
	this->window->draw(this->healthText);
}

void Game::renderEnemies()
{
	for (auto& e : this->enemies)
	{
		this->window->draw(e);
	}
}

void Game::render()
{
	/*
		@return void
		- Clear old frame
		- Display frame in window
		- Renders the game objects
	*/

	this->window->clear();
	this->renderText();
	this->renderEnemies();
	this->window->display();
}