// Fill out your copyright notice in the Description page of Project Settings.


#include "MovimientoAleatorio_CA.h"

// Sets default values for this component's properties
UMovimientoAleatorio_CA::UMovimientoAleatorio_CA()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	RadioMovimiento = 5.0f;
	AnchoMovimiento = 0.0f;
	AltoMovimiento = 5.0f;
	ProfundidadMovimiento = 5.0f;

	TiempoAcumulado = 0.0;
	TiempoLimite = 0.1;
	TiempoMaximo = 0.0;
	direccion = 0;
}


// Called when the game starts
void UMovimientoAleatorio_CA::BeginPlay()
{
	Super::BeginPlay();
	direccion = FMath::RandRange(0, 3);
	// ...
	
}


// Called every frame
void UMovimientoAleatorio_CA::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	auto NuevaPosicion = FVector(0, 0, 0);
	AActor* Padre;
	// ...
	TiempoAcumulado = TiempoAcumulado + DeltaTime;
	TiempoMaximo = TiempoMaximo + DeltaTime;
	if(TiempoMaximo<=3.1f){
		if (TiempoAcumulado >= TiempoLimite) {
			TiempoAcumulado = 0.0f;
			Padre = GetOwner();
			switch (direccion)
			{
			case 0:
				NuevaPosicion = Padre->GetActorLocation() + FVector(AnchoMovimiento, AltoMovimiento, ProfundidadMovimiento);
				break;
			case 1:
				NuevaPosicion = Padre->GetActorLocation() + FVector(AnchoMovimiento, ProfundidadMovimiento, AltoMovimiento);
				break;
			case 2:
				NuevaPosicion = Padre->GetActorLocation() + FVector(AnchoMovimiento, -ProfundidadMovimiento, AltoMovimiento);
				break;
			case 3:
				NuevaPosicion = Padre->GetActorLocation() + FVector(AnchoMovimiento, ProfundidadMovimiento, -AltoMovimiento);
				break;
			default:
				break;
			}
			Padre->SetActorLocation(NuevaPosicion);
		//if (direccion == 0) {
		//	
		//	if (Padre)
		//	{
		//		// Find a new position for the object to go to 
		//		
		//		// Update the object's position 
		//		
		//	}

		//}
		//else {
		//	Padre = GetOwner();
		//	if (Padre)
		//	{
		//		// Find a new position for the object to go to 
		//		
		//		// Update the object's position 
		//		Padre->SetActorLocation(NuevaPosicion);
		//	}

		//}
	}
	}
}

