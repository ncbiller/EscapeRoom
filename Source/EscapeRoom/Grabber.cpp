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

	FindPhysicsHandleComponent();
	SetupInputComponent();
}

void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();

	if (InputComponent) {
		UE_LOG(LogTemp, Warning, TEXT("Input Component Found"))

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
	UE_LOG(LogTemp, Warning, TEXT("Grab Pressed"))
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


	

	//PhysicsHandle->GrabComponent(ActorHit,NAME_None,Component)

	if (ActorHit) {
		UE_LOG(LogTemp, Warning, TEXT("Actor hit %s"), *ActorHit->GetName())
	}



	///if we hit something attach physics handle
	//TODO attach physics handle


}

void UGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab Released"))
	//TODO release physics handle
		if (PhysicsHandle->GrabbedComponent) {
			PhysicsHandle->ReleaseComponent();
		}
}



// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// if physic handle attached move object we're holding
	// otherwise nothing.	
	if (PhysicsHandle->GrabbedComponent) {
		FVector PlayerViewPointLocation;
		FRotator PlayerViewPointRotation;

		GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(PlayerViewPointLocation, PlayerViewPointRotation);
		
		FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * ReachDistance;
		PhysicsHandle->SetTargetLocation(LineTraceEnd);
	}

}

FHitResult UGrabber::GetPhysicsBodyInReach() const
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(PlayerViewPointLocation, PlayerViewPointRotation);

	/*UE_LOG(LogTemp, Warning, TEXT("View Vector %s ViewRotator %s"), *ViewVector.ToString(), *ViewRotator.ToString());*/

	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * ReachDistance;

	DrawDebugLine(GetWorld(), PlayerViewPointLocation, LineTraceEnd, FColor::Red, false, -1.f, 0, 5.f);

	FHitResult hitResult;
	FCollisionQueryParams Params(FName(TEXT("")), false, GetOwner());

	///ray-cast out to  reach distance
	bool check = GetWorld()->LineTraceSingleByObjectType(
		hitResult,
		PlayerViewPointLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		Params);
	return hitResult;
}