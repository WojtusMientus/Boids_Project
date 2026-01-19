
#include "Core/Boid.h"


FBoid::FBoid(const FVector& InitPosition, const FVector& InitVelocity)
			: Position(InitPosition), Velocity(InitVelocity)
{
}

void FBoid::Update(float DeltaTime)
{
	Position += Velocity * DeltaTime;
}

void FBoid::Initialize(const FVector& InitPosition, const FVector& InitVelocity)
{
	Position = InitPosition;
	Velocity = InitVelocity;
	Acceleration = FVector::ZeroVector;
}

void FBoid::Reset()
{
	Position = FVector::ZeroVector;
	Velocity = FVector::ZeroVector;
	Acceleration = FVector::ZeroVector;
}


