#include "StoneyWorldSystems.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/TextRenderComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/StaticMesh.h"
#include "UObject/ConstructorHelpers.h"

AStoneyWorldNPC::AStoneyWorldNPC()
{
    PrimaryActorTick.bCanEverTick = true;
    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}
void AStoneyWorldNPC::BeginPlay() { Super::BeginPlay(); }
void AStoneyWorldNPC::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    if (!bHostile || Health <= 0.f) return;
    AttackCooldown -= DeltaSeconds;
    APawn* Player = UGameplayStatics::GetPlayerPawn(this, 0);
    if (!Player) return;
    const float Distance = FVector::Dist2D(GetActorLocation(), Player->GetActorLocation());
    if (Distance > DetectionRange) return;
    if (Distance > AttackRange)
    {
        AddMovementInput((Player->GetActorLocation() - GetActorLocation()).GetSafeNormal2D(), 0.7f);
        SetActorRotation(FMath::RInterpTo(GetActorRotation(), (Player->GetActorLocation() - GetActorLocation()).Rotation(), DeltaSeconds, 4.f));
    }
    else if (AttackCooldown <= 0.f)
    {
        UGameplayStatics::ApplyDamage(Player, AttackDamage, GetController(), this, nullptr);
        AttackCooldown = 1.2f;
    }
}
float AStoneyWorldNPC::TakeDamage(float Amount, FDamageEvent const& Event, AController* InstigatorController, AActor* Causer)
{
    const float Applied = Super::TakeDamage(Amount, Event, InstigatorController, Causer);
    Health = FMath::Max(0.f, Health - Amount);
    if (Health <= 0.f) { bHostile = false; SetActorEnableCollision(false); SetLifeSpan(0.2f); }
    return Applied;
}

AStoneyWorldMission::AStoneyWorldMission()
{
    PrimaryActorTick.bCanEverTick = false;
    Trigger = CreateDefaultSubobject<USphereComponent>(TEXT("MissionTrigger"));
    SetRootComponent(Trigger);
    Trigger->InitSphereRadius(220.f);
    Trigger->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    Trigger->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
    Label = CreateDefaultSubobject<UTextRenderComponent>(TEXT("MissionLabel"));
    Label->SetupAttachment(Trigger);
    Label->SetRelativeLocation(FVector(0,0,100));
    Label->SetHorizontalAlignment(EHTA_Center);
    Label->SetText(FText::FromString(TEXT("MISSION")));
}
bool AStoneyWorldMission::CompleteMission(AActor* CompletingActor)
{
    if (bCompleted || !IsValid(CompletingActor)) return false;
    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
    if (CompletingActor != PlayerPawn) return false;
    bCompleted = true;
    Label->SetText(FText::FromString(TEXT("MISSION COMPLETE")));
    Label->SetTextRenderColor(FColor::Green);
    return true;
}

AStoneyWorldVehicle::AStoneyWorldVehicle()
{
    PrimaryActorTick.bCanEverTick = false;
    AutoPossessPlayer = EAutoReceiveInput::Disabled;
    VehicleMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VehicleMesh"));
    SetRootComponent(VehicleMesh);
    VehicleMesh->SetSimulatePhysics(false);
    VehicleMesh->SetCollisionProfileName(TEXT("Pawn"));
    static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("/Engine/BasicShapes/Cube.Cube"));
    if (CubeMesh.Succeeded()) VehicleMesh->SetStaticMesh(CubeMesh.Object);
    VehicleMesh->SetRelativeScale3D(FVector(2.2f, 1.2f, 0.65f));
    Movement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("VehicleMovement"));
    Movement->MaxSpeed = DriveSpeed;
    Movement->Acceleration = 2200.f;
    Movement->Deceleration = 1800.f;
}
void AStoneyWorldVehicle::BeginPlay() { Super::BeginPlay(); }
void AStoneyWorldVehicle::SetupPlayerInputComponent(UInputComponent* Input)
{
    Super::SetupPlayerInputComponent(Input);
    Input->BindAxis(TEXT("MoveForward"), this, &AStoneyWorldVehicle::Throttle);
    Input->BindAxis(TEXT("MoveRight"), this, &AStoneyWorldVehicle::Steer);
    Input->BindAction(TEXT("Interact"), IE_Pressed, this, &AStoneyWorldVehicle::ExitVehicle);
}
void AStoneyWorldVehicle::Throttle(float Value)
{
    if (!Controller) return;
    AddMovementInput(GetActorForwardVector(), Value);
}
void AStoneyWorldVehicle::Steer(float Value)
{
    if (FMath::IsNearlyZero(Value)) return;
    AddActorWorldRotation(FRotator(0.f, Value * TurnRate * GetWorld()->GetDeltaSeconds(), 0.f));
}
void AStoneyWorldVehicle::EnterVehicle(ACharacter* Character)
{
    if (!IsValid(Character)) return;
    APlayerController* PC = Cast<APlayerController>(Character->GetController());
    if (!PC) return;
    PreviousDriver = Character;
    Character->SetActorHiddenInGame(true);
    Character->SetActorEnableCollision(false);
    Character->GetCharacterMovement()->DisableMovement();
    PC->Possess(this);
}
void AStoneyWorldVehicle::ExitVehicle()
{
    APlayerController* PC = Cast<APlayerController>(Controller);
    if (!PC || !IsValid(PreviousDriver)) return;
    const FVector ExitLocation = GetActorLocation() + GetActorRightVector() * 220.f + FVector(0,0,80.f);
    PreviousDriver->SetActorLocation(ExitLocation, false, nullptr, ETeleportType::TeleportPhysics);
    PreviousDriver->SetActorHiddenInGame(false);
    PreviousDriver->SetActorEnableCollision(true);
    PreviousDriver->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
    PC->Possess(PreviousDriver);
    PreviousDriver = nullptr;
}
