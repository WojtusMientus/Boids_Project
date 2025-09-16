// Fill out your copyright notice in the Description page of Project Settings.


#include "Interfaces/SelectableBoid.h"
#include "Structs/BoidSelection.h"

// Add default functionality here for any ISelectable functions that are not pure virtual.
FBoidSelection ISelectableBoid::OnBoidSelection()
{
	return FBoidSelection();
}
