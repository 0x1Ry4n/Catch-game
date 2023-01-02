#pragma once

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <ctime>
#include <iostream>
#include <random>
#include <vector>

class Game
{
private:
	// Variables
	sf::RenderWindow* window;
	sf::VideoMode videoMode;
	sf::Event event;
	sf::Font font;
	sf::Text healthText;
	sf::Text pointText;

	// Mouse positions
	sf::Vector2i mousePosWindow;
	sf::Vector2f mousePosView;

	// Cursor handler
	sf::Cursor cursor;

	// Game logic
	bool endGame;
	uint points;
	uint health;
	float enemySpawnTimer;
	float enemySpawnTimerMax;
	uint maxEnemies;
	bool mouseHeld;

	// Game Objects
	std::vector<sf::RectangleShape> enemies;
	sf::RectangleShape enemy;

	// Private Functions
	void initVariables();
	void initWindow();
	void initFont();
	void initEnemies();
	void initCursor();

public:
	// Constructors / Destructors
	Game();
	virtual ~Game();

	// Accessors
	const bool running() const;
	const bool getEndGame() const;

	// Functions
	void spawnEnemy();
	void pollEvents();
	void updateMousePositions();
	void setPointsText(uint points);
	void setHealthText(uint health);

	void updateEnemies();
	void update();

	void renderText();
	void renderEnemies();
	void render();
};