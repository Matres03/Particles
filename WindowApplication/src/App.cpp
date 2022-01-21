#include "App.h"

float distance(Particle p1, Particle p2)
{
	float d = sqrt(pow(p1.object.getPosition().x - p2.object.getPosition().x, 2) + pow(p1.object.getPosition().y - p2.object.getPosition().y, 2));
	
	return d;
}

float angle(Particle p1, Particle p2)
{

	float angle = atan(-1*(p1.object.getPosition().y - p2.object.getPosition().y) / (p1.object.getPosition().x - p2.object.getPosition().x));
	angle = angle * 180 / 3.14159265;
	
	if (p1.object.getPosition().x < p2.object.getPosition().x)
	{
		if (p1.object.getPosition().y < p2.object.getPosition().y)
			angle = 360 - angle * (-1);
	}
	else
		angle = angle + 180;

	return angle;
}

void removeElement(std::vector<Particle>& vector, int index)
{
	std::vector<Particle> temp;
	for (size_t i = 0; i < vector.size(); i++)
	{
		if (i != index)
			temp.push_back(vector[i]);
	}
	vector = temp;
}

void insertElement(std::vector<Particle>& vector, Particle p, int index)
{
	std::vector<Particle> temp;
	for (size_t i = 0; i < vector.size(); i++)
	{
		if (i != index)
			temp.push_back(vector[i]);
		else
		{
			
		}
	}
	vector = temp;
}

App::App()
{
	initWindow();
	initView();
	initVariables();
	initParticles();
}

App::~App()
{
	delete this->window;
}

void App::initWindow()
{
	std::cout << "[@]: Initializing Window_1 ..." << std::endl;

	videoMode.height = unsigned int(780);				//setta l'altezza della finestra
	videoMode.width = unsigned int(1000);				//setta la larghezza della finestra

	window = new sf::RenderWindow(this->videoMode, "Particles Simulation", sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize);

	window->setFramerateLimit(60);						// framerate
	window->setVerticalSyncEnabled(false);				// v-sync

}

void App::initView()
{
	_res = window->getSize();
	view.setSize(_res.x, _res.y);
	view.setCenter(_res.x/2, _res.y/2);
	view.move(_res.x / 2, _res.y / 2);
	view.zoom(190);
	window->setView(view);
}

void App::initVariables()
{
	// GRAPHIC
	scale.x = view.getSize().x / _res.x;
	scale.y = view.getSize().y / _res.y;
	view.zoom(1.5);

	// ENV
	speed = 9 * scale.x;

	//Particles
	N = 1;
	lN = 200;
	G = 900.8;
}

void App::initParticles()
{

	// RESET
	particles.clear();
	std::cout << "\n >>> Init Particles ...\n";

	// CENTRAL START
	particles.push_back(Particle());
	particles[0].object.setPosition(0, 0);
	particles[0].velocity = { 0,0 };
	particles[0].mass = 1500000000; // 15000
	particles[0].object.setRadius(900);
	particles[0].object.setOrigin(particles[0].object.getRadius(), particles[0].object.getRadius());
	particles[0].object.setFillColor(sf::Color(sf::Color(230, 50, 0)));

	// SATELLITES
	for (size_t i = 1; i < N; i++)
	{
		particles.push_back(Particle());
		particles[i].object.setPosition(rand() % 60000000000 + 30000, 0);
		//particles[i].object.setPosition(2000*i + 4000, 0);
		particles[i].velocity.x = 0;
		particles[i].mass = 1;
		particles[i].nDots = 200;
		particles[i].object.setRadius(100);
		particles[i].velocity.y = particles[i].getOrbitalSpeed(G, particles[0].mass, distance(particles[i], particles[0]));
		particles[i].trail.setPrimitiveType(sf::LineStrip);
	}

	// LIGHT PARTICLES
	for (size_t i = 0; i < lN; i++)
	{
		lparticles.push_back(Particle());
		if(rand()%2 == 0)
			lparticles[i].object.setPosition(rand() % 600000000 + 30000, 0);
		else
			lparticles[i].object.setPosition(-1*(rand() % 600000000 + 30000), 0);
		lparticles[i].mass =1;
		lparticles[i].velocity.x = 0;
		if(rand()%2 == 0)
			lparticles[i].velocity.y = (float(rand()%30 + 90)/100.00) * lparticles[i].getOrbitalSpeed(G, particles[0].mass, distance(lparticles[i], particles[0]));
		else
			lparticles[i].velocity.y = -1*(float(rand()%30 + 90)/100.00) * lparticles[i].getOrbitalSpeed(G, particles[0].mass, distance(lparticles[i], particles[0]));
		lparticles[i].nDots = 10;
		lparticles[i].trail.setPrimitiveType(sf::LineStrip);
	}

	// INIT FORCE
	gravity();
	lGravity();
}

const bool App::isRunning() const
{
	// Return wheter the window is running for the main while loop
	return window->isOpen();
}

void App::update()
{
	cycleStart();

	pollEvenets();
	keys();
	updateDeltaTime();
	updateVariables();
	updateMousePos();
	updateView();

	cycleStop();
	updateParticles();
}

void App::pollEvenets()
{
	while (window->pollEvent(ev)) //save action in ev
	{
		if (ev.type == sf::Event::Resized)
		{
			// update the view to the new size of the window
			view.setSize(ev.size.width, ev.size.height);
		}

		switch (ev.type) //save type of action
		{
		case sf::Event::Closed:
		{
			window->close();
			break;
		}
		case sf::Event::KeyPressed:
		{
			if (ev.key.code == sf::Keyboard::Escape)
			{
				window->close();
			}
			
			break;
		}
		case sf::Event::MouseWheelMoved:
		{
			if (ev.mouseWheel.delta == 1)
				//scale.x > 0.2 ? view.zoom(0.9) : void();
				view.zoom(0.9);

			if (ev.mouseWheel.delta == -1)
				scale.x < 300 ? view.zoom(1.1) : void();
		}
		
		}
	}
}

void App::keys()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::J))
		particles[1].object.move(-scale.x, 0);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::L))
		particles[1].object.move(scale.x, 0);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::K))
		particles[1].object.move(0,scale.y);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::I))
		particles[1].object.move(0,-scale.y);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		view.move(speed, 0.f);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		view.move(-speed, 0.f);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		view.move(0.f, speed);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		view.move(0.f, -speed);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		view.zoom(0.95);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		view.zoom(1.05);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		view.rotate(float(0.5 * (-1)));
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		view.rotate(0.5);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::C)) 
	{
		for (size_t i = 0; i < particles.size(); i++)
		{
			particles[0].mass += pow(2,c.getElapsedTime().asSeconds());
		}
		std::cout << "\nADDED";
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Subtract))
		time -= 10;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Tab))
	{
		view.reset(sf::FloatRect(_res.x / int(-2), _res.y / int(-2), _res.x, _res.y));
		view.setCenter(0, 0);

	}
}

void App::cycleStart()
{
	for (size_t i = 0; i < particles.size(); i++)
	{
		particles[i].oldPos = particles[i].object.getPosition();
	}
}

void App::updateDeltaTime()
{
	currentTime = c.getElapsedTime().asSeconds();
	deltaTime = currentTime - lastTime;
	lastTime = currentTime;
	fps = 1.f / (deltaTime);
}

void App::updateVariables()
{
	speed = 9 * scale.x;
}

void App::updateMousePos()
{
	mousePos = sf::Mouse::getPosition(*window);
	worldPos = window->mapPixelToCoords(mousePos);

}

void App::updateView()
{
	_res = window->getSize();
	scale.x = view.getSize().x / _res.x;
	scale.y = view.getSize().y / _res.y;
	/*
	tempCoords.x = 0;
	tempCoords.y = 0;

	for (size_t i = 0; i < particles.size(); i++)
	{
		tempCoords.x += int(particles[i].oldPos.x);
		tempCoords.y += int(particles[i].oldPos.y);
	}

	tempCoords.x = tempCoords.x / N;
	tempCoords.y = tempCoords.y / N;
	view.setCenter(tempCoords);
	//std::cout <<"\n View Coords:" << view.getCenter().x << ", " << view.getCenter().y;
	std::cout <<"\n View Coords:" << tempCoords.x << ", " << tempCoords.y;*/
	window->setView(view);
}

void App::laws()
{
	double y;
	particles[1].object.move(10, 0);
	y = particles[1].velocity.y * c.getElapsedTime().asSeconds() + 0.5*2*c.getElapsedTime().asMicroseconds();
	particles[1].object.move(10, -y);
}

void App::gravity()
{
	x, y = 0;

	for (size_t i = 0; i < particles.size(); i++)
	{
		particles[i].reset();
		for (size_t j = 0; j < particles.size(); j++)
		{
			if (i != j)
			{
				if (distance(particles[i], particles[j]) > (particles[i].object.getRadius() + particles[j].object.getRadius()))
				{
					// Angle
					theta = angle(particles[i], particles[j]);
					theta = theta * 3.14159265 / 180;

					// Force
					tempForce = G * (particles[i].mass) * (particles[j].mass) / pow(distance(particles[i], particles[j]), 2);
					//std::cout << "\n P" << i << " to P" << j << " : " << tempForce;
					particles[i].forceRes.x += tempForce * cos(theta);
					particles[i].forceRes.y += tempForce * -1 * sin(theta);
				}
				else 
				{
					//removeElement(particles, i);
					//removeElement(particles, j);
				}
			}
		}
		particles[i].acc.x += particles[i].forceRes.x / particles[i].mass;
		particles[i].acc.y += particles[i].forceRes.y / particles[i].mass;

		x = (particles[i].velocity.x ) + 0.5 * particles[i].acc.x ;
		y = (particles[i].velocity.y ) + 0.5 * particles[i].acc.y ;
		//x = 0.5 * particles[i].acc.x * pow(c.getElapsedTime().asMilliseconds()/1000, 2);
		//y = 0.5 * particles[i].acc.y * pow(c.getElapsedTime().asMilliseconds()/1000, 2);
		particles[i].object.move(x, y);

	}
}

void App::lGravity()
{
	x, y = 0;

	for (size_t i = 0; i < lparticles.size(); i++)
	{
		lparticles[i].reset();
		for (size_t j = 0; j < particles.size(); j++)
		{
			if (distance(lparticles[i], particles[j]) > (lparticles[i].object.getRadius() + particles[j].object.getRadius()))
			{
				// Angle
				theta = angle(lparticles[i], particles[j]);
				theta = theta * 3.14159265 / 180;

				// Force
				tempForce = G * (lparticles[i].mass) * (particles[j].mass) / pow(distance(lparticles[i], particles[j]), 2);
				lparticles[i].forceRes.x += tempForce * cos(theta);
				lparticles[i].forceRes.y += tempForce * -1 * sin(theta);
			}
		}
		lparticles[i].acc.x += lparticles[i].forceRes.x / lparticles[i].mass;
		lparticles[i].acc.y += lparticles[i].forceRes.y / lparticles[i].mass;

		x = (lparticles[i].velocity.x) + 0.5 * lparticles[i].acc.x;
		y = (lparticles[i].velocity.y) + 0.5 * lparticles[i].acc.y;
		lparticles[i].object.move(x, y);

	}
}

void App::electricForce()
{
	for (size_t i = 0; i < particles.size(); i++)
	{
		particles[i].reset();
		for (size_t j = 0; j < particles.size(); j++)
		{
			if (i != j)
			{
				if (distance(particles[i], particles[j]) > (particles[i].object.getRadius() + particles[j].object.getRadius()))
				{
					// Angle
					theta = angle(particles[i], particles[j]);
					theta = theta * 3.14159265 / 180;

					// Force
					tempForce = -100 * (particles[i].elecCharge) * (particles[j].elecCharge) / pow(distance(particles[i], particles[j]), 2);
					//std::cout << "\n P" << i << " to P" << j << " : " << tempForce;
					particles[i].forceRes.x += tempForce * cos(theta);
					particles[i].forceRes.y += tempForce * -1 * sin(theta);
				}
				else
				{
					//removeElement(particles, i);
					//removeElement(particles, j);
				}
			}
		}
		particles[i].acc.x += particles[i].forceRes.x / particles[i].mass;
		particles[i].acc.y += particles[i].forceRes.y / particles[i].mass;

		time = c.getElapsedTime().asSeconds() / 10;

		x = particles[i].object.getPosition().x + (particles[i].velocity.x / 1000) + 0.5 * particles[i].acc.x;
		y = particles[i].object.getPosition().y + (particles[i].velocity.y / 1000) + 0.5 * particles[i].acc.y;
		//x = 0.5 * particles[i].acc.x * pow(c.getElapsedTime().asMilliseconds()/1000, 2);
		//y = 0.5 * particles[i].acc.y * pow(c.getElapsedTime().asMilliseconds()/1000, 2);

		particles[i].object.setPosition(x, y);
	}
}

void App::updateParticles()
{
	
	for (size_t i = 0; i < particles.size(); i++)
	{
		// CALC VELOCITY
		particles[i].velocity.x += particles[i].acc.x / particles[i].mass;
		particles[i].velocity.y += particles[i].acc.y / particles[i].mass;

		//particles[i].momentum.x = particles[i].mass * particles[i].velocity.x;
		//particles[i].momentum.y = particles[i].mass * particles[i].velocity.y;

		// CALC ACC
		//particles[i].acc.x = (particles[i].velocity.x - particles[i].oldVelocity.x) / deltaTime;
		//particles[i].oldVelocity.x = particles[i].velocity.x;
		//particles[i].acc.y = (particles[i].velocity.y - particles[i].oldVelocity.y) / deltaTime;
		//particles[i].oldVelocity.y = particles[i].velocity.y;

		// TRAIL
		particles[i].updateTrail();
		//if (i != 0)
			//particles[i].mass = particles[i].mass * 1.01;
	}
	for (size_t i = 0; i < lparticles.size(); i++)
	{
		lparticles[i].velocity.x += lparticles[i].acc.x / lparticles[i].mass;
		lparticles[i].velocity.y += lparticles[i].acc.y / lparticles[i].mass;
		lparticles[i].updateTrail();

	}
	
	//std::cout << "\n Angle :" << angle(particles[0], particles[1]);
	//std::cout << "\n Vel :" << particles[1].velocity.x << " : " << -1*particles[1].velocity.y;
	//std::cout << "\n Pos : " << particles[1].object.getPosition().x << ", " << particles[0].object.getPosition().y;
	//std::cout << "\n F : " << particles[1].forceRes.x << ", " << particles[0].forceRes.y;
	//std::cout << "\n ACC : " << particles[1].acc.x << ", " << particles[0].acc.y;
	//std::cout << "\n F Comp : " << particles[1].forceRes.x << ", " << particles[0].forceRes.y;
	//std::cout << "\n FPS : " << fps;;
	gravity();
	lGravity();
	//gravity();
	//gravity();
	//std::cout << "\n Mass 0 : " << particles[0].mass;;

	//electricForce();
	//laws();

}

void App::cycleStop()
{
	for (size_t i = 0; i < particles.size(); i++)
	{
		particles[i].newPos = particles[i].object.getPosition();
	}
}

void App::renderParticles()
{
	for (size_t i = 0; i < particles.size(); i++)
	{
		this->window->draw(particles[i].trail);
		this->window->draw(particles[i].object);
	}

	for (size_t i = 0; i < lparticles.size(); i++)
	{
		this->window->draw(lparticles[i].trail);
		std::cout << "\n Vcount : " << lparticles[i].trail.getVertexCount();
		this->window->draw(lparticles[i].object);
	}
}

void App::render()
{
	// CLEARS WINDOW
	//window->clear(sf::Color(220, 220, 220));				// pulisce il frame precedente
	window->clear(sf::Color(40, 40, 50));				// pulisce il frame precedente

	// RENDERE OBJECT
	renderParticles();

	//DISPLAY
	window->display();				// renderizza nuova finestra
}
