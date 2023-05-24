// Fill out your copyright notice in the Description page of Project Settings.


#include "Board.h"
#include "EngineUtils.h"
#include "DrawDebugHelpers.h"
#include "DirectorPiezas.h"
#include "PiezaCruz.h"
#include "PiezaDispersa.h"
#include "PiezaTetramino.h"
#include "PiezaLineaPunteada.h"
#include "Pieza3Bloques.h"
#include "Pieza2Bloques.h"
#include "Pieza.h"

#include "Kismet/GameplayStatics.h"
// Sets default values

ABoard::ABoard()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABoard::BeginPlay()
{
    flagPrimeraCondicion = false;
    selecActual = 0;
    selecSiguiente = 0;
    selecSubSiguiente = 0;
	Super::BeginPlay();
    /*CurrentPiece->puntuacion = 0;*/
    PiezaCruzBuilder = GetWorld()->SpawnActor<APiezaCruz>(APiezaCruz::StaticClass());
    PiezaDispersaBuilder = GetWorld()->SpawnActor<APiezaDispersa>(APiezaDispersa::StaticClass());
    PiezaTetraminoBuilder = GetWorld()->SpawnActor<APiezaTetramino>(APiezaTetramino::StaticClass()); 
    PiezaLineaPunteadaBuilder = GetWorld()->SpawnActor<APiezaLineaPunteada>(APiezaLineaPunteada::StaticClass());
    Pieza3BloquesBuilder = GetWorld()->SpawnActor<APieza3Bloques>(APieza3Bloques::StaticClass());
    Pieza2BloquesBuilder = GetWorld()->SpawnActor<APieza2Bloques>(APieza2Bloques::StaticClass());
    Director = GetWorld()->SpawnActor<ADirectorPiezas>(ADirectorPiezas::StaticClass());
    //CREACION DE FACTORY DE NIVELES
    FabricaEscenas = GetWorld()->SpawnActor<AFactoryNivelesConcreta>(AFactoryNivelesConcreta::StaticClass());
    if (vectorPiezas.Num() == 0) {
        llenarVector();
    }
    for (TActorIterator<APieza> it(GetWorld()); it; ++it)
    {
        
        if (it->GetFName() == TEXT("DissmissPieces"))
        {
            it->Dismiss(0);
            it->Destroy();
        }
    }
}

// Called every frame
void ABoard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    if (bGameOver)
    {
        return;
    }

    switch (Status)
    {
    case PS_NOT_INITED:
        NewPiece();
        CoolLeft = CoolDown;
        Status = PS_MOVING;
        break;
    case PS_MOVING:
        CoolLeft -= DeltaTime;
        if (CoolLeft <= 0.0f)
        {
            MoveDown();
        }
        break;
    case PS_GOT_BOTTOM:
        CoolLeft -= DeltaTime;
        if (CoolLeft <= 0.0f)
        {
            if (CurrentPiece)
            {
                CurrentPiece->Dismiss(0);
                CurrentPiece->Destroy();
            }
            CurrentPiece = nullptr;
            NewPiece();
            CoolLeft = CoolDown;
            Status = PS_MOVING;
        }
        break;
    default:
        break;
    }
}

// Called to bind functionality to input
void ABoard::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAction("Rotate", IE_Pressed, this, &ABoard::Rotate);
    PlayerInputComponent->BindAction("MoveLeft", IE_Pressed, this, &ABoard::MoveLeft);
    PlayerInputComponent->BindAction("MoveRight", IE_Pressed, this, &ABoard::MoveRight);
    PlayerInputComponent->BindAction("MoveDownToEnd", IE_Pressed, this, &ABoard::MoveDownToEnd);
    PlayerInputComponent->BindAction("CambiarNivel1", IE_Pressed, this, &ABoard::CambiarNivel1);
    PlayerInputComponent->BindAction("CambiarNivel2", IE_Pressed, this, &ABoard::CambiarNivel2);
    PlayerInputComponent->BindAction("CambiarNivel3", IE_Pressed, this, &ABoard::CambiarNivel3);
    /*PlayerInputComponent->BindAction("CambiarNivel3", IE_Pressed, this, &ABoard::CambiarNivel1);*/
  

}
void ABoard::CambiarNivel1() {
    FabricaEscenas->cargarNivel("MapaTetris");
}
void ABoard::CambiarNivel2() {
    FabricaEscenas->cargarNivel("MapaTetris2");
}
void ABoard::CambiarNivel3() {
    FabricaEscenas->cargarNivel("MapaTetris3");
}

void ABoard::Rotate()
{
    if (CurrentPiece && Status != PS_GOT_BOTTOM)
    {
        CurrentPiece->TestRotate();
    }
}


void ABoard::MoveLeft()
{
    if (CurrentPiece)
    {
        CurrentPiece->MoveLeft();
        if (Status == PS_GOT_BOTTOM)
        {
            MoveDownToEnd();
        }
    }
}

void ABoard::MoveRight()
{
    if (CurrentPiece)
    {
        CurrentPiece->MoveRight();
        if (Status == PS_GOT_BOTTOM)
        {
            MoveDownToEnd();
        }
    }
}

void ABoard::MoveDown()
{
    if (CurrentPiece)
    {
        if (!CurrentPiece->MoveDown())
        {
            Status = PS_GOT_BOTTOM;
        }
        CoolLeft = CoolDown;
    }
}
void ABoard::llenarVector() {
    int32 RandomInt;
    bool flagRepetido;
    //While:Repetiremos el proceso hasta tener 3 numeros diferentes sin repetir.
    while (vectorPiezas.Num() < 4) {
        RandomInt = FMath::RandRange(0, 3); 
        flagRepetido = false;       //Seteamos el Flag como falso para verificar si se repite un numero
        if (vectorPiezas.Num() != 0) {
            for (const int32& Value : vectorPiezas) //Iteramos entre todos los valores actuales del Array
            {
                if (Value == RandomInt) {       //Si councide un valor, flag=true
                    flagRepetido = true;
                    break;                      //Basta que se repita un valor para ya dejar de iterar en el for.
                }
            }
            if (flagRepetido == false) {    //Si despues de iterar todo el for, no hay repetidos, entonces agregamos a Array.
                vectorPiezas.Add(RandomInt);
            }
        }
        else        //Si el vector se encuentra vacio, entonces directamente agregamos un valor aleatorio dentro del rango
        {
            vectorPiezas.Add(RandomInt);
        }   
    }
}
void ABoard::NewPiece()
{
    CheckLine();
    UE_LOG(LogTemp, Warning, TEXT("GENERANDO NUEVA PIEZA!"));
    if (PiezaSiguiente) {
        UE_LOG(LogTemp, Warning, TEXT("HAY PIEZA SIGUIENTEEEE"));
        PiezaSiguiente->Dismiss(1);
        PiezaSiguiente->Destroy();
    }
    if (PiezaSubSiguiente) {
        UE_LOG(LogTemp, Warning, TEXT("HAY PIEZA SIGUIENTEEEE"));
        PiezaSubSiguiente->Dismiss(1);
        PiezaSubSiguiente->Destroy();
    }
    if (CurrentPiece)
    {
        UE_LOG(LogTemp, Warning, TEXT("SI HAY CURRENT PIECE, ELIMINANDOOOO!"));
        CurrentPiece->Dismiss(0);
        CurrentPiece->Destroy();
       
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("NO HAY CURRENT PIECE!!!!"));
    }
   
    if (vectorPiezas.Num() == 0) {      // Si se vacia la bolsa, volverla a llenar
        llenarVector();
       
    }
    auto NewPos = FVector(0, 0, 0);
    if (flagPrimeraCondicion == false) {
        selecActual = vectorPiezas.Last();
        vectorPiezas.Pop();
        selectTipoPieza(selecActual);
        if (vectorPiezas.Num() == 0) {  // Si se vacia la bolsa, volverla a llenar
            llenarVector();
        }
        Director->ConstruirPieza();
        CurrentPiece = Director->GetPieza();
        //-----
        selecSiguiente = vectorPiezas.Last();
        vectorPiezas.Pop();
        selectTipoPieza(selecSiguiente);
        if (vectorPiezas.Num() == 0) {  // Si se vacia la bolsa, volverla a llenar
            llenarVector();
        }
        Director->ConstruirPieza();
        PiezaSiguiente = Director->GetPieza();
        NewPos = PiezaSiguiente->SceneComponent->GetComponentLocation() + FVector(0, 100, 0);
        PiezaSiguiente->SetActorLocation(NewPos);
        //----
        selecSubSiguiente = vectorPiezas.Last();
        vectorPiezas.Pop();
        selectTipoPieza(selecSubSiguiente);
        if (vectorPiezas.Num() == 0) {  // Si se vacia la bolsa, volverla a llenar
            llenarVector();
        }
        Director->ConstruirPieza();
        PiezaSubSiguiente = Director->GetPieza();
        NewPos = PiezaSubSiguiente->SceneComponent->GetComponentLocation() + FVector(0, 100, -80);
        PiezaSubSiguiente->SetActorLocation(NewPos);
        flagPrimeraCondicion = true;
    }
    else {
        selecActual = selecSiguiente;
        selecSiguiente = selecSubSiguiente;
        selecSubSiguiente = vectorPiezas.Last();
        vectorPiezas.Pop();
        
        if (vectorPiezas.Num() == 0) { // Si se vacia la bolsa, volverla a llenar
            llenarVector();
        }

        selectTipoPieza(selecActual);
        Director->ConstruirPieza();
        CurrentPiece = Director->GetPieza();
        //-----
        selectTipoPieza(selecSiguiente);
        Director->ConstruirPieza();
        PiezaSiguiente = Director->GetPieza();
        NewPos = PiezaSiguiente->SceneComponent->GetComponentLocation() + FVector(0, 100, 0);
        PiezaSiguiente->SetActorLocation(NewPos);
        //--
        selectTipoPieza(selecSubSiguiente);
        Director->ConstruirPieza();
        PiezaSubSiguiente = Director->GetPieza();
        NewPos = PiezaSubSiguiente->SceneComponent->GetComponentLocation() + FVector(0, 100, -80);
        PiezaSubSiguiente->SetActorLocation(NewPos);
    }
    if (CurrentPiece->Blocks.Num() <= 2) {      //solamente si CurrentPiece tiene 2 o menos bloques, se aplica el componente.
        int condicionAleatorio;

        FString mensajeBloques;
        mensajeBloques = TEXT("Pieza con 2 o 1 bloque");
        auto Message = FString::Printf(TEXT("%s entered me"),*mensajeBloques);
        GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, Message);
        for (ABlock* Bloque : CurrentPiece->Blocks)
        {
            
            condicionAleatorio = FMath::RandRange(0, 10);
            auto mensajeAleatoriedad = FString::Printf(TEXT("Condicion Aleatoria Cumplida"));
            if(condicionAleatorio<5){
            CurrentPiece->permitirRotacion = false;
            GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Green, mensajeAleatoriedad);
            AdaptadorMovimientoAleatorio1 = NewObject<UAdaptadorMovimientoAleatorio_CA>(Bloque);
            AdaptadorMovimientoAleatorio1->SetLimitesMovimiento(0.0f, 1.0f, 0.0f);
            AdaptadorMovimientoAleatorio1->RegisterComponent();
            }
        }
    }

    //selecSubSiguiente = vectorPiezas.Last();

    //Explicacion funcionamiento de logica
    //Asignamos el ultimo valor del vector a CurrentPiece Mediante el director.
    //Luego, eliminamos ese valor del vector, por tanto, el antepenultimo se asigna a piezasiguiente (es el nuevo ultimo)
    //Despues de eliminar un elemento del vector, siempre revisamos si se quedo vacio para volver a llenarlo
    
    
    //----------------
    
    
    /*NewPos = PiezaSiguiente->GetActorLocation() + FVector(0, 100, 0);*/
    bGameOver = CheckGameOver();
    if (bGameOver)
    {
        UE_LOG(LogTemp, Warning, TEXT("Game Over!!!!!!!!"));
        /*if (GameOverSoundCue)
        {
            UGameplayStatics::PlaySoundAtLocation(GetWorld(), GameOverSoundCue, GetActorLocation(), GetActorRotation());
        }*/
    }
}

void ABoard::selectTipoPieza(int valor) {
    switch (valor)
    {
    case 0:
        Director->AsignarConstructorDePieza(PiezaTetraminoBuilder);
        break;
    case 1:
        Director->AsignarConstructorDePieza(Pieza3BloquesBuilder);
        break;
    case 2:
        Director->AsignarConstructorDePieza(PiezaLineaPunteadaBuilder);
        break;
    case 3:
        Director->AsignarConstructorDePieza(Pieza2BloquesBuilder);
    default:
        break;
    }
}
void ABoard::CheckLine()
{
    auto MoveDownFromLine = [this](int z) {
        FVector Location(0.0f, 0.0f, 5.0 * z + 100.0);
        FRotator Rotation(0.0f, 0.0f, 0.0f);
        TArray<struct FOverlapResult> OutOverlaps;
        FCollisionShape CollisionShape;
        FVector Extent(4.5f, 49.5f, 95.0 + 4.5 - 5.0 * z);
        CollisionShape.SetBox(Extent);
        DrawDebugBox(GetWorld(), Location, Extent, FColor::Purple, false, 1, 0, 1);
        FCollisionQueryParams Params;
        FCollisionResponseParams ResponseParam;
        if (GetWorld()->OverlapMultiByChannel(OutOverlaps,
            Location, Rotation.Quaternion(), ECollisionChannel::ECC_WorldDynamic,
            CollisionShape, Params, ResponseParam))
        {
            for (auto&& Result : OutOverlaps)
            {
                FVector NewLocation = Result.GetActor()->GetActorLocation();
                NewLocation.Z -= 10.0f;
                Result.GetActor()->SetActorLocation(NewLocation);
            }
        }
    };

    int z = 0;
    while (z < 20)
    {
        FVector Location(0.0f, 0.0f, 10.0f * z + 5.0f);
        FRotator Rotation(0.0f, 0.0f, 0.0f);
        TArray<struct FOverlapResult> OutOverlaps;
        FCollisionShape CollisionShape;
        CollisionShape.SetBox(FVector(4.5f, 49.0f, 4.5f));
        //DrawDebugBox(GetWorld(), Location, FVector(4.5f, 10.0f, 4.5f), FColor::Purple, false, 1, 0, 1);
        FCollisionQueryParams Params;
        FCollisionResponseParams ResponseParam;
        bool b = GetWorld()->OverlapMultiByChannel(OutOverlaps,Location, Rotation.Quaternion(), ECollisionChannel::ECC_WorldDynamic, CollisionShape, Params, ResponseParam);
        if (!b || OutOverlaps.Num() < 10)
        {
            ++z;
            continue;
        }
        else // this line is full, remove the line
        {
            UE_LOG(LogTemp, Warning, TEXT("Find FULL LINE at z=%d"), z);
            for (auto&& Result : OutOverlaps)
            {
                /*Result.GetActor()->Destroy();*/
                AActor* actorPrueba=nullptr;
                actorPrueba = Result.GetActor();
                
                ((ABlock*)actorPrueba)->reducirVida();
                //actorPrueba->Destroy();  //OJO esto tenemos que eliminarlo una vez corregido linea completa
            }
            MoveDownFromLine(z);

            /*if (LineRemoveSoundCue)
            {
                UGameplayStatics::PlaySoundAtLocation(GetWorld(), LineRemoveSoundCue, GetActorLocation(), GetActorRotation());
            }*/
        }
    }
}

void ABoard::MoveDownToEnd()
{
    if (!CurrentPiece)
    {
        return;
    }

    while (CurrentPiece->MoveDown())
    {
    }

    /*if (MoveToEndSoundCue)
    {
        UGameplayStatics::PlaySoundAtLocation(GetWorld(), MoveToEndSoundCue, GetActorLocation(), GetActorRotation());
    }*/

    switch (Status)
    {
    case PS_MOVING:
        Status = PS_GOT_BOTTOM;
        CoolLeft = CoolDown;
        break;
    case PS_GOT_BOTTOM:
        break;
    default:
        UE_LOG(LogTemp, Warning, TEXT("Wrong status for MoveDownToEnd"));
        break;
    }
}

bool ABoard::CheckGameOver()
{
    if (!CurrentPiece)
    {
        UE_LOG(LogTemp, Warning, TEXT("NoPieces"));
        return true;
    }

    return CurrentPiece->CheckWillCollision([](FVector OldVector) { return OldVector; });
}
