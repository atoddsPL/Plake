// Fill out your copyright notice in the Description page of Project Settings.


#include "SpellProjectileBase.h"

#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
ASpellProjectileBase::ASpellProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collision"));
	RootComponent = SphereComponent;
	SphereComponent->SetSphereRadius(10.0f);

	ParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle"));
	ParticleSystemComponent->SetupAttachment(SphereComponent);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));

}

// Called when the game starts or when spawned
void ASpellProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASpellProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

