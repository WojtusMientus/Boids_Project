
#include "Core/Boid.h"
#include "Core/BoidManagerSubsystem.h"


FBoid::FBoid(const FVector& InitPosition, const FVector& InitVelocity): Position(InitPosition), Velocity(InitVelocity)
{
}

void FBoid::Update(float DeltaTime)
{
	Position += Velocity * DeltaTime;
}


