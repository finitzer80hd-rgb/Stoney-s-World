#pragma once
#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "GameFramework/Character.h"
#include "GameFramework/Pawn.h"
#include "StoneyWorldSystems.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UFloatingPawnMovement;
class UTextRenderComponent;

UCLASS()
class STONEYWORLD_API UStoneyWorldSaveGame : public USaveGame
{
    GENERATED_BODY()
public:
    UPROPERTY() FVector PlayerLocation = FVector::ZeroVector;
    UPROPERTY() FRotator PlayerRotation = FRotator::ZeroRotator;
    UPROPERTY() float Health = 100.f;
    UPROPERTY() int32 WantedLevel = 0;
    UPROPERTY() int32 MissionsCompleted = 0;
};

UCLASS()
class STONEYWORLD_API AStoneyWorldNPC : public ACharacter
{
    GENERATED_BODY()
public:
    AStoneyWorldNPC();
    virtual void Tick(float DeltaSeconds) override;
    virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="NPC") float Health = 100.f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="NPC") float DetectionRange = 1800.f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="NPC") float AttackRange = 180.f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="NPC") float AttackDamage = 8.f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="NPC") bool bHostile = true;
protected:
    virtual void BeginPlay() override;
private:
    float AttackCooldown = 0.f;
};

UCLASS()
class STONEYWORLD_API AStoneyWorldMission : public AActor
{
    GENERATED_BODY()
public:
    AStoneyWorldMission();
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly) TObjectPtr<USphereComponent> Trigger;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly) TObjectPtr<UTextRenderComponent> Label;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Mission") FText MissionTitle = FText::FromString(TEXT("First Steps"));
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Mission") FText MissionDescription = FText::FromString(TEXT("Reach this marker to complete the mission."));
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Mission") int32 Reward = 100;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Mission") bool bCompleted = false;
    UFUNCTION(BlueprintCallable, Category="Mission") bool CompleteMission(AActor* CompletingActor);
};

UCLASS()
class STONEYWORLD_API AStoneyWorldVehicle : public APawn
{
    GENERATED_BODY()
public:
    AStoneyWorldVehicle();
    virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
    UFUNCTION(BlueprintCallable, Category="Vehicle") void EnterVehicle(ACharacter* Character);
protected:
    virtual void BeginPlay() override;
private:
    UPROPERTY(VisibleAnywhere) TObjectPtr<UStaticMeshComponent> VehicleMesh;
    UPROPERTY(VisibleAnywhere) TObjectPtr<UFloatingPawnMovement> Movement;
    UPROPERTY(EditAnywhere, Category="Vehicle") float DriveSpeed = 1400.f;
    UPROPERTY(EditAnywhere, Category="Vehicle") float TurnRate = 90.f;
    UPROPERTY() TObjectPtr<ACharacter> PreviousDriver;
    void Throttle(float Value);
    void Steer(float Value);
    void ExitVehicle();
};
