// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#include "GameFramework/Actor.h"

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

	AActor * my_parent = GetOwner();
	FRotator OwnerRotator = my_parent->GetActorRotation();
	
	UE_LOG(LogTemp, Warning, TEXT("%s is at %s"), *my_parent->GetName(), *OwnerRotator.ToString());

	FRotator NewRotation = FRotator(0, -90, 0);

	FQuat QuatRotation = FQuat(NewRotation);
	my_parent->AddActorLocalRotation(NewRotation, false, 0, ETeleportType::None);

	UE_LOG(LogTemp, Warning, TEXT("%s is at %s"), *my_parent->GetName(), *OwnerRotator.ToString());
	
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

