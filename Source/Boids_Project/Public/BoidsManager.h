// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Boid.h"

class BOIDS_PROJECT_API BoidsManager
{
	
public:

	static constexpr float SEPARATION_FORCE = .03;  
	static constexpr float ALIGNMENT_FORCE = .01;  
	static constexpr float COHESION_FORCE = .01;  

	static constexpr float PERCEPTION_DISTANCE = 200;
	static constexpr float BOID_MAX_SPEED = 4;
	
	static constexpr int BOIDS_COUNT = 100;
	static constexpr int BOIDS_BOUNDS = 1200;
	
	BoidsManager();
	~BoidsManager();

	void InitializeBoids();
	void UpdateBoids();

	FVector GetBoidPositionAt(int Index);
	FVector GetBoidVelocityAt(int Index);
	
private:
	
	void UpdateSeparation();
	FVector SeparationResultPerBoid(int BoidIndexToCalculate);
	
	void UpdateAlignment();
	FVector AlignmentResultPerBoid(int BoidIndexToCalculate);
	
	void UpdateCohesion();
	FVector CohesionResultPerBoid(int BoidIndexToCalculate);

	void CheckOutOfBounds();
	
	TArray<TSharedPtr<Boid>> Boids;
};
