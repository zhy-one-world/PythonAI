// Fill out your copyright notice in the Description page of Project Settings.


#include "FinishActor.h"
#include "../PythonAICharacter.h"
#include "Components/SphereComponent.h"

// Sets default values
AFinishActor::AFinishActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	if (SphereComponent)
	{
		SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AFinishActor::BeginOverlap);
		if (RootComponent)
		{
			RootComponent = SphereComponent;
		}
	}
}

// Called when the game starts or when spawned
void AFinishActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFinishActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFinishActor::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		APythonAICharacter* PythonAICharacter = Cast<APythonAICharacter>(OtherActor);
		if (PythonAICharacter)
		{
			FString MethodName = PythonAICharacter->GetPyhtonMethodName();
			FString Args = PythonAICharacter->GetPyhtonArgs();
			TArray<UObject*> objs = PythonAICharacter->FinishLevel.GetAllObjects();
			PythonAICharacter->FinishLevel.Broadcast(FVector(-1750.003052, -1357.0, 228.755005),MethodName,Args);

		}
	}
}

