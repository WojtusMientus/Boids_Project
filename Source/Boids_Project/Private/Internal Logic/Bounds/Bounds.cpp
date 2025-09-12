// Fill out your copyright notice in the Description page of Project Settings.


#include "Internal Logic/Bounds/Bounds.h"


FBounds::FBounds()
{
}

FBounds::FBounds(const FVector& UpperForwardRightCorner, const FVector& BottomBackLeftCorner)
{
	BoundsBottomLeftBackCorner = BottomBackLeftCorner;
	BoundsUpperRightForwardCorner = UpperForwardRightCorner;
}

FBounds::~FBounds()
{
}

