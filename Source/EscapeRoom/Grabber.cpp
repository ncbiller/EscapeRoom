// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Get player viewpoint
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(PlayerViewPointLocation,PlayerViewPointRotation);

	/*UE_LOG(LogTemp, Warning, TEXT("View Vector %s ViewRotator %s"), *ViewVector.ToString(), *ViewRotator.ToString());*/

	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * ReachDistance;

	DrawDebugLine(GetWorld(), PlayerViewPointLocation, LineTraceEnd, FColor::Red,false,-1.f,0,5.f);

	//GetWorld()->LineTraceSingleByObjectType()

	// Dr

	// Ray cast out to reach distance

	// Return object hit
}

