// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


class BOIDS_PROJECT_API Boid
{
	
public:
	
	Boid();
	Boid(const int& MaxBounds);
	
	~Boid();

	void Update();
	
	FVector Position;
	FVector Velocity;
	
};
