// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"
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

	PressurePlateActor = GetWorld()->GetFirstPlayerController()->GetPawn();


	my_parent = GetOwner();
	

}

void UOpenDoor::OpenDoor()
{
	
	my_parent->SetActorRelativeRotation(FRotator(0, OpenDoorAngle, 0));
}

void UOpenDoor::CloseDoor()
{

	my_parent->SetActorRelativeRotation(FRotator(0, -180, 0));
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PressurePlate->IsOverlappingActor(PressurePlateActor)) {
		OpenDoor();
		LastDoorOpenTime = GetWorld()->GetTimeSeconds();
	}
	
	if (GetWorld()->GetTimeSeconds() > LastDoorOpenTime + DoorCloseDelay) {
		CloseDoor();
	}
}

