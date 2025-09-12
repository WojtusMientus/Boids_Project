// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Internal Logic/Boid.h"

class BOIDS_PROJECT_API BoidsManager
{
	
public:

	static constexpr float SEPARATION_FORCE = .7;  
	static constexpr float ALIGNMENT_FORCE = .5;  
	static constexpr float COHESION_FORCE = .35;  

	static constexpr float PERCEPTION_DISTANCE = 125;
	static constexpr float BOID_MAX_SPEED = 2;
	
	static constexpr int BOIDS_COUNT = 100;
	static constexpr int BOIDS_BOUNDS = 1000;
	
	BoidsManager();
	~BoidsManager();

	void InitializeBoids();
	void UpdateBoids();

	FVector GetBoidPositionAt(int Index);
	FVector GetBoidVelocityAt(int Index);
	
private:

	TArray<TSharedPtr<Boid>> GetNeighbourBoids(int BoidIndexToCheckNeighbours);
	void CheckBoidsSubarrayForValidBoids(int StartIndex, int EndIndex, int BoidIndexToCheckNeighbours, TArray<TSharedPtr<Boid>>& ValidBoids);

	void TestUpdateAllInOne();
	
	void UpdateSeparation();
	FVector SeparationResultPerBoid(int BoidIndexToCalculate);
	
	void UpdateAlignment();
	FVector AlignmentResultPerBoid(int BoidIndexToCalculate);
	
	void UpdateCohesion();
	FVector CohesionResultPerBoid(int BoidIndexToCalculate);
	
	void CheckOutOfBounds();

	
	TArray<TSharedPtr<Boid>> Boids;
	TArray<FVector> NewCalculatedVelocityPerBoid;
};
