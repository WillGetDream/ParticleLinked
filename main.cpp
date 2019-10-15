// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <Windows.h>
#include <chrono>
#include <assert.h>
#include <time.h>
#include <stdio.h>
using namespace std;


BOOL setPosition(int x, int y) {
	COORD c;
	c.X = x;
	c.Y = y;

	return SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

static void DRAW(float x, float y) {
	setPosition(x, y);
	cout << "*";
}


class Particle
{
public:

	Particle * getNext() const { return state_.next; }
	void setNext(Particle* next) { state_.next = next; }

	Particle()
		: framesLeft_(0)
	{}

	bool draw();

	void init(double x, double y,
		double xVel, double yVel, int lifetime)
	{
		x = x; y = y;
		vx = xVel; vy = yVel;
		framesLeft_ = lifetime;
	}

	bool inUse() const { return framesLeft_ > 0; }


private:
	int framesLeft_;
	double x, y;
	double vx, vy;
	union
	{
		// State when it's in use.
		struct
		{
			double x, y;
			double xVel, yVel;
		} live;

		// State when it's available.
		Particle* next;
	} state_;
};

bool Particle::draw()
{
	if (!inUse()) return false;

	framesLeft_--;
	x += vx;
	y += vy;

	DRAW(x, y);
	//printf("\n[p %f %f]", x, y);

	return framesLeft_ == 0;
}



class ParticlePool
{
public:
	void create(double x, double y,
		double xVel, double yVel,
		int lifetime);

	ParticlePool();
	void draw();


private:
	static const int POOL_SIZE = 100;
	Particle particles_[POOL_SIZE];
	Particle* firstAvailable_;

};
ParticlePool::ParticlePool()
{
	// The first one is available.
	firstAvailable_ = &particles_[0];

	// Each particle points to the next.
	for (int i = 0; i < POOL_SIZE - 1; i++)
	{
		particles_[i].setNext(&particles_[i + 1]);
	}

	// The last one terminates the list.
	particles_[POOL_SIZE - 1].setNext(NULL);
}

void ParticlePool::create(double x, double y,
	double xVel, double yVel,
	int lifetime)
{
	// Make sure the pool isn't full.
	assert(firstAvailable_ != NULL);

	// Remove it from the available list.
	Particle* newParticle = firstAvailable_;
	firstAvailable_ = newParticle->getNext();

	newParticle->init(x, y, xVel, yVel, lifetime);
}

void ParticlePool::draw()
{
	for (int i = 0; i < POOL_SIZE; i++)
	{
		if (particles_[i].draw())
		{
			// Add this particle to the front of the list.
			particles_[i].setNext(firstAvailable_);
			firstAvailable_ = &particles_[i];
		}
	}
}




int main() {
	while (true) {
		//if (GetAsyncKeyState(VK_DOWN) < 0) {
		if (1) {

			srand(static_cast<unsigned int>(time(0)));
			int FRAMES = 30;

			ParticlePool *ps = new ParticlePool();

			ps->create(0,0,rand()%3+1,rand()%3+1,10);


			for (int i = 0; i < FRAMES; i++) {

			   //printf("\n\n[FRAME %d]", i);
				ps->draw();
				ps->create(0, 0, rand() % 3 + 1, rand() % 3 + 1, 5);
				ps->create(0, 0, rand() % 3 + 1, rand() % 3 + 1, 5);
				ps->create(0, 0, rand() % 3 + 1, rand() % 3 + 1, 5);
				ps->create(0, 0, rand() % 3 + 1, rand() % 3 + 1, 5);
			
			}


			break;
		}
	}



	return 0;

}
