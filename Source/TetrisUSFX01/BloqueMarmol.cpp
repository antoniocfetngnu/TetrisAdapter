// Fill out your copyright notice in the Description page of Project Settings.


#include "BloqueMarmol.h"
ABloqueMarmol::ABloqueMarmol() {
	PrimaryActorTick.bCanEverTick = true;
	vida = 1; //reducimos la vida para que se destruya mas rapido que su bloque padre
	//imprimirNombre();
	SetMesh();
}
void ABloqueMarmol::BeginPlay()
{
	Super::BeginPlay();
	
}
void ABloqueMarmol::SetMesh() {
	UMaterialInterface* MyMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("Material'/Game/StarterContent/Materials/M_AssetPlatform.M_AssetPlatform'"));
	UMaterialInterface* MyMaterial1 = LoadObject<UMaterialInterface>(nullptr, TEXT("Material'/Game/Mesh/M_CobbleStone_Rough.M_CobbleStone_Rough'"));
	BlockMesh->SetMaterial(0, MyMaterial1);
	BlockMesh->SetMaterial(1, MyMaterial);
	//if (BlockMeshAsset.Succeeded())
	//{
	//	BlockMesh->SetStaticMesh(BlockMeshAsset.Object);
	//	/*FVector NuevaEscala = FVector(0.03f, 0.03f, 0.03f);*/
	//	BlockMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	//	//BlockMesh->SetWorldScale3D(NuevaEscala);
	//	
	//}
	//else {
	//	UE_LOG(LogTemp, Warning, TEXT("No existe la malla para Block"));
	//}
}
void ABloqueMarmol::reducirVida() {
	UE_LOG(LogTemp, Log, TEXT("Bloque Cono, Vidas: %d"), vida);
	--vida;
	if (vida <= 0) {
		DestroyActor();
	}
}