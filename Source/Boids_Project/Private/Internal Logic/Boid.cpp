#include "Internal Logic/Boid.h"
#include "Subsystems/BoidManagerSubsystem.h"


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

	Velocity = FMath::VRand() * UBoidManagerSubsystem::BOID_MAX_SPEED;
}

Boid::~Boid()
{
}

void Boid::Update()
{
	Position += Velocity;
}


