// Fill out your copyright notice in the Description page of Project Settings.


#include "TimeSplinePawn.h"

#include "UObject/ConstructorHelpers.h"
#include "Components/TimelineComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Components/SplineComponent.h"
//#include "UObject/UObjectGlobals.h"

// Sets default values
ATimeSplinePawn::ATimeSplinePawn() 
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Duration = 3.0f;
	StartOffset = 0.0f;
	OtherActor = nullptr;

	CreateTimeLine();
}

// Called when the game starts or when spawned
void ATimeSplinePawn::BeginPlay()
{
	Super::BeginPlay();

	LoadCurve();
	LoadSpline();
	AddTimeLineEvent();
	StartMovieTimeline();
}

void ATimeSplinePawn::SetOtherActor(AActor* Actor)
{
	OtherActor = Actor;
}

void ATimeSplinePawn::StartMovieTimeline()
{
	MoveTimeline->SetLooping(true);
	MoveTimeline->SetPlayRate(1.0f / Duration);
	MoveTimeline->SetNewTime(StartOffset);

	MoveTimeline->PlayFromStart();
}

void ATimeSplinePawn::AddTimeLineEvent()
{
	FOnTimelineFloat TimelineInterpFunc;
	TimelineInterpFunc.BindUFunction(this, TEXT("MoveTimelineStep"));
	MoveTimeline->AddInterpFloat(TimelineCurve, TimelineInterpFunc);

	FOnTimelineEvent TimelineFinishedFunc;
	TimelineFinishedFunc.BindUFunction(this, TEXT("MoveTimelineFinished"));
	MoveTimeline->SetTimelineFinishedFunc(TimelineFinishedFunc);

	FOnTimelineEvent TimelinePostUpdateFunc;
	TimelinePostUpdateFunc.BindUFunction(this, TEXT("MoveTimelinePostUpdate"));
	MoveTimeline->SetTimelinePostUpdateFunc(TimelinePostUpdateFunc);
}

void ATimeSplinePawn::MoveTimelineStep(float Value)
{
	float Distance = GetSplineDistanceByAlpha(Value);
	FVector NewLocation = SplineComponent->GetLocationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World);
	FRotator NewRotator = SplineComponent->GetRotationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World);
	this->SetActorLocationAndRotation(NewLocation, NewRotator);
	if (OtherActor)
	{
		OtherActor->SetActorLocationAndRotation(NewLocation, NewRotator);
	}
}

void ATimeSplinePawn::MoveTimelineFinished()
{

}

void ATimeSplinePawn::MoveTimelinePostUpdate()
{

}

// Called every frame
void ATimeSplinePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATimeSplinePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ATimeSplinePawn::CreateTimeLine()
{
	MoveTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("Timeline"));

	/*
	UCurveFloat* Curve = nullptr;
	static ConstructorHelpers::FObjectFinder<UCurveFloat> DataCurve(TEXT("/Game/Curve/Curve1.Curve1"));
	if (DataCurve.Object) {
		Curve = DataCurve.Object;
	}
	*/
}

void ATimeSplinePawn::LoadSpline()
{
	TArray<AActor*> ActorList;
	FName Path = TEXT("Blueprint'/Game/Blueprints/Spline_BP.Spline_BP_C'");
	UClass* GeneratedBP = Cast<UClass>(StaticLoadObject(UClass::StaticClass(), NULL, *Path.ToString()));

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), GeneratedBP, ActorList);
	for (int n = 0; n < ActorList.Num(); ++n)
	{
		AActor* Actor = ActorList[n];
		USplineComponent* Spline = Actor->FindComponentByClass<USplineComponent>();
		float MinDist = FLT_MAX;
		if (Spline)
		{
			float Dist = FVector::Dist2D(this->GetActorLocation(), Spline->GetComponentLocation());
			if (Dist < MinDist)
			{
				SplineComponent = Spline;
				MinDist = Dist;
			}
		}
	}
}

void ATimeSplinePawn::LoadCurve()
{
	/*
	이건 nullprt 넘어옴
	FName Path = TEXT("/Game/Curve/Curve1.Curve1_C");
	UClass* GeneratedBP = Cast<UClass>(StaticLoadObject(UCurveFloat::StaticClass(), NULL, *Path.ToString()));
	TimelineCurve = Cast<UCurveFloat>(GeneratedBP);
	*/
	UCurveFloat* Curve = LoadObject<UCurveFloat>(NULL, TEXT("/Game/Curve/Curve1.Curve1"), NULL, LOAD_None, NULL);
	TimelineCurve = Curve;
}

float ATimeSplinePawn::GetSplineDistanceByAlpha(float Alpha)
{
	float Length = SplineComponent->GetSplineLength();
	return FMath::Lerp(0.0f, Length, Alpha);
}