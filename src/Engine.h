#pragma once
#pragma once
#include <SFML/Graphics.hpp>
#include "Particle.h"

class Engine
{
private:
	// A regular RenderWindow
	sf::RenderWindow m_Window;

	//vector for Particles
	std::vector<Particle> m_particles;

	// Private functions for internal use only
	void input();
	void update(float dt);
	void draw();

public:
	// The Engine constructor
	Engine();

	// Run will call all the private functions
	void run();

};