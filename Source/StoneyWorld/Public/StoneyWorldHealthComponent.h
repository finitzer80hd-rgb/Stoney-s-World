#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StoneyWorldHealthComponent.generated.h"

UCLASS(ClassGroup=(StoneyWorld), meta=(BlueprintSpawnableComponent))
class STONEYWORLD_API UStoneyWorldHealthComponent : public UActorComponent
{
    GENERATED_BODY()
public:
    UStoneyWorldHealthComponent();
protected:
    virtual void BeginPlay() override;
    UFUNCTION() void HandleOwnerDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Health") float MaxHealth = 100.f;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Health") float Health = 100.f;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Wanted") int32 WantedLevel = 0;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Missions") int32 MissionsCompleted = 0;
    UFUNCTION(BlueprintCallable, Category="Health") void ApplyDamage(float Amount);
    UFUNCTION(BlueprintCallable, Category="Health") void RestoreHealth(float Amount);
    UFUNCTION(BlueprintCallable, Category="Wanted") void AddWantedLevel(int32 Amount = 1);
    UFUNCTION(BlueprintCallable, Category="Wanted") void ClearWantedLevel();
    UFUNCTION(BlueprintCallable, Category="Save") bool SaveProgress();
    UFUNCTION(BlueprintCallable, Category="Save") bool LoadProgress();
};
