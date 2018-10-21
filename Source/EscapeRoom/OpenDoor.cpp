// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/World.h"


// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	owner = GetOwner();
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (GetTotalMassOfActorsOnPlate() > DoorTriggerMass) {
		OnOpenRequest.Broadcast();
		
	}
	else {

		OnCloseRequest.Broadcast();
	}
}

float UOpenDoor::GetTotalMassOfActorsOnPlate()
{
	if (!PressurePlate) { return 0.0f; }
	
	float TotalMass = 0.f;
	TArray<AActor*> OverlappingActors;
	// Find all the overlapping actors
	PressurePlate->GetOverlappingActors(OverlappingActors);

	for (auto * f : OverlappingActors) {
		TotalMass += f->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		//UE_LOG(LogTemp,Warning,TEXT("%s is on Pressure Plate"), *f->GetName())
	}
	//UE_LOG(LogTemp, Warning, TEXT("%f is on Pressure Plate"), TotalMass)

	return TotalMass;
}

