// Fill out your copyright notice in the Description page of Project Settings.


#include "MovimientoTriangular.h"

// Sets default values for this component's properties
UMovimientoTriangular::UMovimientoTriangular()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	velMovimiento = 0.5;
	// ...
}


// Called when the game starts
void UMovimientoTriangular::BeginPlay()
{
	Super::BeginPlay();
	intervaloCambio = 0.0f;
	direccion = 0;
	Parent = GetOwner();

	// ...
	
}


// Called every frame
void UMovimientoTriangular::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (Parent)
	{
		intervaloCambio += DeltaTime;
		if (intervaloCambio >= 1.0f) {
			intervaloCambio = 0;
			direccion += 1;
			if (direccion > 2) direccion = 0;
		}
		auto NewPos=FVector(0,0,0);
		switch (direccion)
		{
		case 0:
			NewPos = Parent->GetActorLocation() + FVector(0, 1 * velMovimiento, 1 * velMovimiento);
			break;
		case 1:
			NewPos = Parent->GetActorLocation() + FVector(0, -1 * velMovimiento, 1 * velMovimiento);
			break;
		case 2:
			NewPos = Parent->GetActorLocation() + FVector(0, 0, -2 * velMovimiento);
			break;
		default:
			NewPos = Parent->GetActorLocation();
			break;
		}
		// Find a new position for the object to go to
		
		// Update the object's position
		Parent->SetActorLocation(NewPos);
	}	
	// ...
}

