#include "Particle.h"

void popHead(sf::VertexArray& v)
{
	sf::VertexArray temp;

	for (size_t i = 1; i < v.getVertexCount(); i++)
	{
		temp.append(v[i]);
	}
	v = temp;
}

Particle::Particle()
{
	// POS
	object.setOrigin(object.getRadius() / 2, object.getRadius() / 2);
	object.setPosition(rand()%5000, rand()%5000);

	// VELOCITY
	velocity.x = rand() % 300 - 150;
	velocity.y = rand() % 300 - 150;
	oldVelocity.x = 0;
	oldVelocity.y = 0;

	// FORCES
	acc.x = 0;
	acc.y = 0;
	forceRes.x = 0;
	forceRes.y = 0;

	// ELEC CHARGE
	if (rand() % 2 == 0)
		elecCharge = 500;
	else
		elecCharge = -1 * 500;

	// MASS
	mass = rand() % 80 + 100;
	object.setRadius(mass/5);
	object.setOrigin(object.getRadius(), object.getRadius());

	// MOMENTUM
	momentum.x = mass * velocity.x;
	momentum.y = mass * velocity.y;


	// COLOR
	//elecCharge < 0 ? object.setFillColor(sf::Color::Blue) : object.setFillColor(sf::Color::Red);
	object.setFillColor(sf::Color(rand()%100 +110, rand() % 100 + 110,rand() % 100 + 110));

	// TRAIL
	nDots = 200;
	dot.color = object.getFillColor();

	std::cout << "\n - Particle Loaded";
}

void Particle::addForce(float force, float theta)
{
	float forceX = cos(theta);
	float forceY = sin(theta);

	forceRes.x += forceX;
	forceRes.y += forceY;
}

void Particle::calcSpeed()
{

}

void Particle::reset()
{
	forceRes = { 0, 0 };
	acc = { 0, 0 };
}

void Particle::updateTrail()
{
	dot.position = object.getPosition();
	trail.append(dot);
	if (trail.getVertexCount() > nDots)
	{
		popHead(trail);
		trail.setPrimitiveType(sf::LineStrip);
	}
	for (size_t i = 0; i < trail.getVertexCount(); i++)
	{
		trail[i].color = sf::Color(object.getFillColor().r, object.getFillColor().g, object.getFillColor().b, int(i*255/nDots));
	}
}

float getEscapeSpeed(float G, float M, float d)
{
	float v = sqrt(2 * G * M / d);
	return v;
}

float Particle::getOrbitalSpeed(float G, float M, float d)
{
	float v = sqrt(G * M / d);
	return v;
}

float Particle::getEscapeSpeed(float G, float M, float d)
{
	return 0.0f;
}

float Particle::getOrbitalDistance(float G, float M, float v)
{
	float d = pow(v,2)/(G*M);
	return d;
}
