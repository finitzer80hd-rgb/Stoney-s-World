#include "StoneyWorldHealthComponent.h"
#include "StoneyWorldSystems.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

UStoneyWorldHealthComponent::UStoneyWorldHealthComponent() { PrimaryComponentTick.bCanEverTick = false; }
void UStoneyWorldHealthComponent::ApplyDamage(float Amount)
{
    Health = FMath::Clamp(Health - FMath::Max(0.f, Amount), 0.f, MaxHealth);
    if (Health <= 0.f)
    {
        if (ACharacter* Character = Cast<ACharacter>(GetOwner())) Character->DisableInput(nullptr);
    }
}
void UStoneyWorldHealthComponent::RestoreHealth(float Amount) { Health = FMath::Clamp(Health + FMath::Max(0.f, Amount), 0.f, MaxHealth); }
void UStoneyWorldHealthComponent::AddWantedLevel(int32 Amount) { WantedLevel = FMath::Clamp(WantedLevel + Amount, 0, 5); }
void UStoneyWorldHealthComponent::ClearWantedLevel() { WantedLevel = 0; }
bool UStoneyWorldHealthComponent::SaveProgress()
{
    UStoneyWorldSaveGame* Save = Cast<UStoneyWorldSaveGame>(UGameplayStatics::CreateSaveGameObject(UStoneyWorldSaveGame::StaticClass()));
    if (!Save || !GetOwner()) return false;
    Save->PlayerLocation = GetOwner()->GetActorLocation();
    Save->PlayerRotation = GetOwner()->GetActorRotation();
    Save->Health = Health; Save->WantedLevel = WantedLevel; Save->MissionsCompleted = MissionsCompleted;
    return UGameplayStatics::SaveGameToSlot(Save, TEXT("StoneyWorld_Save01"), 0);
}
bool UStoneyWorldHealthComponent::LoadProgress()
{
    if (!UGameplayStatics::DoesSaveGameExist(TEXT("StoneyWorld_Save01"), 0)) return false;
    UStoneyWorldSaveGame* Save = Cast<UStoneyWorldSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("StoneyWorld_Save01"), 0));
    if (!Save || !GetOwner()) return false;
    GetOwner()->SetActorLocationAndRotation(Save->PlayerLocation, Save->PlayerRotation, false, nullptr, ETeleportType::TeleportPhysics);
    Health = FMath::Clamp(Save->Health, 0.f, MaxHealth); WantedLevel = FMath::Clamp(Save->WantedLevel, 0, 5); MissionsCompleted = FMath::Max(0, Save->MissionsCompleted);
    return true;
}
