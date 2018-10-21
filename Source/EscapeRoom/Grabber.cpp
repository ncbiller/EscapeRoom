// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"


// Sets default values for this component's properties
UGrabber::UGrabber()
{
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	FindPhysicsHandleComponent();
	SetupInputComponent();
}

void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();

	if (InputComponent) {
		InputComponent->BindAction(FName(TEXT("Grab")), IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction(FName(TEXT("Grab")), IE_Released, this, &UGrabber::Release);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Input Component Not Found on %s"), *GetOwner()->GetName())
	}
}

void UGrabber::FindPhysicsHandleComponent()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if (!PhysicsHandle) {
		UE_LOG(LogTemp, Error, TEXT("Physics Handle Not Found on %s"), *GetOwner()->GetName())
	}
}

void UGrabber::Grab()
{

	/// Try and reach actors in physics collision channel set
	auto HitResult = GetPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent();
	auto ActorHit = HitResult.GetActor();

	if (ActorHit) {
		PhysicsHandle->GrabComponentAtLocationWithRotation(
			ComponentToGrab,
			NAME_None,
			ActorHit->GetActorLocation(),
			ActorHit->GetActorRotation()
		);
	}

}

void UGrabber::Release()
{
		if (PhysicsHandle->GrabbedComponent) {
			PhysicsHandle->ReleaseComponent();
		}
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PhysicsHandle->GrabbedComponent) {
		FVector PlayerViewPointLocation;
		FRotator PlayerViewPointRotation;
		FVector LineTraceEnd = GetReachLineEnd();
		PhysicsHandle->SetTargetLocation(LineTraceEnd);
	}
}

FHitResult UGrabber::GetPhysicsBodyInReach()
{
	FHitResult hitResult;
	FCollisionQueryParams Params(FName(TEXT("")), false, GetOwner());

	///ray-cast out to  reach distance
	GetWorld()->LineTraceSingleByObjectType(
		hitResult,
		GetReachLineStart(),
		GetReachLineEnd(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		Params);
	return hitResult;
}

FVector UGrabber::GetReachLineStart()
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(PlayerViewPointLocation, PlayerViewPointRotation);
	return PlayerViewPointLocation;
}
// Helper Returns Postion of End of Reach Line
FVector UGrabber::GetReachLineEnd()
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(PlayerViewPointLocation, PlayerViewPointRotation);
	return PlayerViewPointLocation + PlayerViewPointRotation.Vector() * ReachDistance;
}


