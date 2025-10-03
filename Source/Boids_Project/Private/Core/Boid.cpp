
#include "Core/Boid.h"
#include "Core/BoidManagerSubsystem.h"


FBoid::FBoid()
{
}

FBoid::FBoid(const FVector& Position, const FVector& Velocity): Position(Position), Velocity(Velocity)
{
}

void FBoid::Update(float DeltaTime, float MaxSpeed)
{
	Velocity.Normalize();
	Velocity *= DeltaTime * MaxSpeed;
	Position += Velocity;
}


