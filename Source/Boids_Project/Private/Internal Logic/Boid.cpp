#include "Internal Logic/Boid.h"
#include "Subsystems/BoidManagerSubsystem.h"


Boid::Boid()
{
}

Boid::Boid(float MaxBounds)
:	Position(FMath::RandRange(-MaxBounds / 2, MaxBounds / 2),
				FMath::RandRange(-MaxBounds / 2, MaxBounds / 2),
				FMath::RandRange(-MaxBounds / 2, MaxBounds / 2)),
	  Velocity(FMath::VRand() * UBoidManagerSubsystem::BOID_MAX_SPEED)
{
}

void Boid::Update(float DeltaTime, float MaxSpeed)
{
	Velocity.Normalize();
	Velocity *= DeltaTime * MaxSpeed;
	Position += Velocity;
}


