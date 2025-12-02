
#include "Core/Boid.h"
#include "Core/BoidManagerSubsystem.h"


FBoid::FBoid(const FVector& InitPosition, const FVector& InitVelocity, const int32 InitID)
			: Position(InitPosition), Velocity(InitVelocity)
{
}

void FBoid::Update(float DeltaTime)
{
	Position += Velocity * DeltaTime;
}


