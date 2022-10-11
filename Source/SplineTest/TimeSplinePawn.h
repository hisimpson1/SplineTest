// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "TimeSplinePawn.generated.h"

UCLASS(Blueprintable, BlueprintType)
class SPLINETEST_API ATimeSplinePawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATimeSplinePawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void StartMovieTimeline();
	bool SetOtherActor(); //false이면 블루프린트를 사용 한다.

protected:
	// Called when the game starts or when spawned
	void CreateTimeLine();
	void LoadSpline();
	void LoadCurve();
	void AddTimeLineEvent();

	UFUNCTION()
	void MoveTimelineStep(float Value);
	UFUNCTION()
	void MoveTimelineFinished();
	UFUNCTION()
	void MoveTimelinePostUpdate();

	float GetSplineDistanceByAlpha(float Alpha);
	
protected:
	UPROPERTY()
	class AActor* OtherActor;
	UPROPERTY()
	class UTimelineComponent* MoveTimeline;
	UPROPERTY()
	class USplineComponent* SplineComponent;
	UPROPERTY()
	class UCurveFloat*  TimelineCurve;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Timeline", Meta = (AllowPrivateAccess = "true"))
	float Duration;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Timeline", Meta = (AllowPrivateAccess = "true"))
	float StartOffset;

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AActor> BoxBlueprint;
};
