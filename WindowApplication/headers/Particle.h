#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

class Particle
{
public:

	Particle();

private:

public:

	// PHYSIC
	float mass;
	float elecCharge;

	// FORCES
	sf::Vector2f forceRes;
	sf::Vector2f force;

	// SPACE
	sf::Vector2f velocity;
	sf::Vector2f oldVelocity;
	sf::Vector2f acc;
	
	sf::Vector2f pos;
	sf::Vector2f oldPos;
	sf::Vector2f newPos;

	sf::Vector2f momentum;

	// GRAPHIC
	sf::Texture texture;
	sf::CircleShape object;

	// TRAIL
	float nDots;
	sf::Vertex dot;
	sf::VertexArray trail;

	void addForce(float force, float theta);
	void calcSpeed();
	void reset();

	void updateTrail();
	float getOrbitalSpeed(float G, float M, float d);
	float getEscapeSpeed(float G, float M, float d);
	float getOrbitalDistance(float G, float M, float v);

};

