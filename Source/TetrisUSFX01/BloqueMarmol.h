// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "bloqueCono.h"
#include "BloqueMarmol.generated.h"

/**
 * 
 */
UCLASS()
class TETRISUSFX01_API ABloqueMarmol : public AbloqueCono
{
	GENERATED_BODY()
	
public:
	ABloqueMarmol();
	virtual void BeginPlay() override;
	virtual void SetMesh() override;
	virtual void reducirVida() override;

};
