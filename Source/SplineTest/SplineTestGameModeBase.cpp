// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.


#include "SplineTestGameModeBase.h"

#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Components/SplineComponent.h"
#include "TimeSplinePawn.h"

#pragma optimize( "", off )

//블루프린트의 인티저 변수를 리턴한다.
bool GetBluePrintVariable(UObject* Target, FString VariableName, int& OutValue)
{
	UProperty* Prop = Target->GetClass()->FindPropertyByName(*VariableName);
	if (Prop->GetClass() == UIntProperty::StaticClass())
	{
		void* ValueAddr = Prop->ContainerPtrToValuePtr<void>(Target);
		UIntProperty* objectProp = Cast<UIntProperty>(Prop);

		OutValue = (int)objectProp->GetSignedIntPropertyValue(ValueAddr); //casting: int64 --> int
		//FString ValueStr = objectProp->GetNumericPropertyValueToString(ValueAddr);
		//int Value = objectProp->GetPropertyValue_InContainer(ValueAddr);  이건 이상한값이 나옴
		//UE_LOG(LogTemp, Log, TEXT("Have UObjectProperty: %s, %d!!!"), *(Target->GetName()), Value);

		return true;
	}

	return false;
}

//블루프린트의 bool 변수를 리턴한다.
bool GetBluePrintVariable(UObject* Target, FString VariableName, bool& OutValue)
{
	UProperty* Prop = Target->GetClass()->FindPropertyByName(*VariableName);
	if (Prop->GetClass() == UBoolProperty::StaticClass())
	{
		void* ValueAddr = Prop->ContainerPtrToValuePtr<void>(Target);
		UBoolProperty* objectProp = Cast<UBoolProperty>(Prop);

		OutValue = (bool)objectProp->GetPropertyValue(ValueAddr);
		return true;
	}

	return false;
}

ASplineTestGameModeBase::ASplineTestGameModeBase()
{
	static ConstructorHelpers::FObjectFinder<UBlueprint> BoxBP(TEXT("Blueprint'/Game/Blueprints/Box_BP.Box_BP'"));
	if (BoxBP.Object)
	{
		BoxBlueprint = (UClass*)BoxBP.Object->GeneratedClass;
	}
}

void ASplineTestGameModeBase::StartPlay()
{
	Super::StartPlay();

	LoadBluePrintTest();
	SpawnTimeSpline();
}

void ASplineTestGameModeBase::SpawnTimeSpline()
{
	TArray<AActor*> ActorList;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATimeSplinePawn::StaticClass(), ActorList);

	if (ActorList.Num() > 0)
		return;

	ATimeSplinePawn* TimeSplinePawn = GetWorld()->SpawnActor<ATimeSplinePawn>(ATimeSplinePawn::StaticClass());

	TArray<AActor*> BoxActorList;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), BoxBlueprint, BoxActorList);
	if (BoxActorList.Num() > 0)
	{
		bool OutBluePrintActiveValue = 0;
		GetBluePrintVariable(BoxActorList[0], TEXT("ActiveMove"), OutBluePrintActiveValue);
		if (OutBluePrintActiveValue)
			return;
		TimeSplinePawn->SetOtherActor(BoxActorList[0]);
	}
}

//블루프린트를 로딩해서 블루프린트의 USplineComponent 컴포넌트에 접근한다.
void ASplineTestGameModeBase::LoadBluePrintTest()
{
	TArray<AActor*> ActorList;
	FName Path = TEXT("Blueprint'/Game/Blueprints/Spline_BP.Spline_BP_C'"); 
	UClass* GeneratedBP = Cast<UClass>(StaticLoadObject(UClass::StaticClass(), NULL, *Path.ToString())); 

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), GeneratedBP, ActorList);
	for (int n = 0; n < ActorList.Num(); ++n)
	{
		AActor* Actor = ActorList[n];
		USplineComponent* SplineComponent = Actor->FindComponentByClass<USplineComponent>();
		if (SplineComponent)
		{
			int OutValue = 0;
			if(GetBluePrintVariable(Actor, TEXT("SplineID"), OutValue))
				UE_LOG(LogTemp, Log, TEXT("Have SplineComponent!!!"));
		}
	}
}

#pragma optimize( "", on )