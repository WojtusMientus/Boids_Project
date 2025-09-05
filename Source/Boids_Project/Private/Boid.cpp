#include "Boid.h"

#include "BoidsManager.h"

Boid::Boid()
{
}

Boid::Boid(const int& MaxBounds)
{
	int HalfWidth = MaxBounds / 2;

	float RandomPositionX = FMath::RandRange(-HalfWidth, HalfWidth);
	float RandomPositionY = FMath::RandRange(-HalfWidth, HalfWidth);
	float RandomPositionZ = FMath::RandRange(-HalfWidth, HalfWidth);
	Position = FVector(RandomPositionX, RandomPositionY, RandomPositionZ);

	Velocity = FMath::VRand() * BoidsManager::BOID_MAX_SPEED;
	Acceleration = FVector(0, 0, 0);
}

Boid::~Boid()
{
}

void Boid::Update()
{
	Position += Velocity;
	Velocity += Acceleration;
}


