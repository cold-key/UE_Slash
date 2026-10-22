// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

class USkeletalMeshComponent;
class USphereComponent;

UCLASS()
class SLASH_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItem();
	virtual void Tick(float DeltaTime) override;
	void DrawAxisInSingleFrame();
	void DrawSphereSingleFrame();

protected:

	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* skCom;

	UPROPERTY(VisibleAnywhere)
	USphereComponent* sphereCom;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sin Parameters")
	float amplitude = 1.0f;

	UFUNCTION(BlueprintPure)
	float TransformSin();

	UFUNCTION(BlueprintPure)
	float TransformCos();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float runningTime = 0.f;

	UFUNCTION()
	void OnComponentBeginOverlapCallBack(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnComponentEndOverlapCallBack(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
