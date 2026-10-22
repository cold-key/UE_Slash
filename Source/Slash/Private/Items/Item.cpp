// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Item.h"
#include <DrawDebugHelpers.h>
#include "Components/SphereComponent.h"
#include<Components/SkeletalMeshComponent.h>
#include "Characters/SlashCharacter.h"

// Sets default values
AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	UE_LOG(LogTemp, Warning, TEXT("Item Construct in CPP"));
	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, 30.f, FColor::Green, FString("Item Construct in CPP"));
	}

	skCom = CreateDefaultSubobject<USkeletalMeshComponent>(FName("skCom"));
	SetRootComponent(skCom);

	sphereCom = CreateDefaultSubobject<USphereComponent>(FName("sphereCom"));
	sphereCom->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp,Warning, TEXT("Item BeginPlay in CPP"));

	//UWorld* world = GetWorld();
	//FVector pos = GetActorLocation();
	//FVector rightVector = GetActorRightVector();
	//FVector rightPoint = rightVector * 200 + pos;
	//FVector upVector = GetActorUpVector();
	//FVector upPoint = upVector * 200 + pos;
	//if (world) {
	//	DrawDebugSphere(world, pos, 400.f, 18, FColor::Cyan, false, 60.f);
	//	DrawDebugLine(world, pos, rightPoint, FColor::Blue, true);
	//	DrawDebugLine(world, pos, upPoint, FColor::Emerald, true);
	//	DrawDebugPoint(world, rightPoint, 20.f, FColor::Blue, true);
	//	DrawDebugPoint(world, upPoint, 20.f, FColor::Emerald, true);
	//}

	//if (GEngine && !world) {
	//	GEngine->AddOnScreenDebugMessage(2, 30.f, FColor::Cyan, TEXT("UWorld is nullptr"));
	//}

	sphereCom->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnComponentBeginOverlapCallBack);
	sphereCom->OnComponentEndOverlap.AddDynamic(this, &AItem::OnComponentEndOverlapCallBack);
	
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//UE_LOG(LogTemp, Warning, TEXT("Item Tick in CPP, and dt is %.2f"),DeltaTime);
	//if (GEngine) {
	//	FString msg = FString::Printf(TEXT("Item Tick in CPP, and dt is %.6f"), DeltaTime);
	//	GEngine->AddOnScreenDebugMessage(1, 30.f, FColor::Red, msg);
	//}
	//float xSpeed = 50.f;
	//float rotationSpeed = 90.f;
	runningTime += DeltaTime;
	//float z = amplitude * FMath::Sin(runningTime);
	//AddActorWorldOffset(FVector(xSpeed * DeltaTime, 0.f, z));
	//AddActorLocalRotation(FRotator(0.f, rotationSpeed * DeltaTime, 0.f));
	//DrawAxisInSingleFrame();
	//DrawSphereSingleFrame();
}

void AItem::DrawAxisInSingleFrame() {
	UWorld* world = GetWorld();
	FVector pos = GetActorLocation();
	FVector forwardVector = GetActorForwardVector();
	FVector forwardPoint = forwardVector * 200 + pos;
	FVector rightVector = GetActorRightVector();
	FVector rightPoint = rightVector * 200 + pos;
	FVector upVector = GetActorUpVector();
	FVector upPoint = upVector * 200 + pos;
	if (world) {
		DrawDebugLine(world, pos, forwardPoint, FColor::Red);
		DrawDebugPoint(world, forwardPoint, 20.f, FColor::Red);
		DrawDebugLine(world, pos, rightPoint, FColor::Green);
		DrawDebugPoint(world, rightPoint, 20.f, FColor::Green);
		DrawDebugLine(world, pos, upPoint, FColor::Blue);
		DrawDebugPoint(world, upPoint, 20.f, FColor::Blue);

		// test
		DrawDebugPoint(world, pos, 10.f, FColor::Red, false, 50.f);
	}
}

void AItem::DrawSphereSingleFrame() {
	UWorld* world = GetWorld();
	FVector pos = GetActorLocation();
	if (world) {
		DrawDebugSphere(world, pos, 400.f, 18, FColor::Cyan, false, -1.f);
	}
}

float AItem::TransformSin()
{
	return amplitude * FMath::Sin(runningTime);
}

float AItem::TransformCos()
{
	return amplitude * FMath::Cos(runningTime);
}

void AItem::OnComponentBeginOverlapCallBack(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(3, 60.f, FColor::Green, FString("OnComponentBeginOverlapCallBack"));
	}

	ASlashCharacter* character = Cast<ASlashCharacter>(OtherActor);
	if (character) {
		character->SetItemPtr(this);
	}
}

void AItem::OnComponentEndOverlapCallBack(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(3, 60.f, FColor::Green, FString("OnComponentEndOverlapCallBack"));
	}
	ASlashCharacter* character = Cast<ASlashCharacter>(OtherActor);
	if (character) {
		character->SetItemPtr(nullptr);
	}
}

