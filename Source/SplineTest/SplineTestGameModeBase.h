// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SplineTestGameModeBase.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class SPLINETEST_API ASplineTestGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ASplineTestGameModeBase();
	virtual void StartPlay() override;

private:
	void LoadBluePrintTest();
	void SpawnTimeSpline();

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AActor> BoxBlueprint;
};
