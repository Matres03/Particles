#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include "Particle.h"

class App
{	

public:

	App();
	virtual ~App();

private:

	//CLOCK
	float time;
	sf::Clock c;
	float deltaTime = 0;
	float currentTime = 0;
	float lastTime = 0;
	float fps;

	// WINDOW
	sf::Vector2u _res;
	sf::RenderWindow* window;
	sf::VideoMode videoMode;
	sf::Event ev;

	// VIEW
	sf::Vector2f tempCoords;
	sf::View view;
	sf::Vector2f scale;

	// Movement / Env
	float speed;
	sf::Vector2i mousePos;
	sf::Vector2f worldPos;

	// PARTICLE
	int N;									// Number of particles
	int lN;
	float G;								// Gravitational Constant
	float tempForce;
	float tempAcc;

	std::vector<Particle> particles;
	std::vector<Particle> lparticles;

	// Temp Values
	Particle P;
	double x;
	double y;
	double theta;
	
	//	INIT
	void initWindow();
	void initView();
	void initVariables();
	void initParticles();
	
	// UPDATE
	void updateDeltaTime(); 
	void updateVariables();
	void updateMousePos();
	void updateView();
	void updateParticles();

		// APP 
		void pollEvenets();
		void keys();

		// CALCULATIONS
		void cycleStart();
		void cycleStop();

		// FORCES
		void laws();
		void gravity();
		void lGravity();
		void electricForce();
	
	// RENDER 
	void renderParticles();

public:

	// BOOL
	const bool isRunning() const;

	// UPDATE
	void update();

	// RENDER
	void render();

};

