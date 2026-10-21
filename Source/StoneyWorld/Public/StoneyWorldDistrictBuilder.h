#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StoneyWorldDistrictBuilder.generated.h"

class UStaticMesh;

UCLASS()
class STONEYWORLD_API AStoneyWorldDistrictBuilder : public AActor
{
    GENERATED_BODY()

public:
    AStoneyWorldDistrictBuilder();

protected:
    virtual void BeginPlay() override;

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="District")
    bool bBuildAtRuntime = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="District", meta=(ClampMin="1"))
    int32 BuildingRowsPerSide = 3;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="District", meta=(ClampMin="500"))
    float BlockSpacing = 1500.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="District")
    bool bSpawnGameplayActors = true;

private:
    UStaticMesh* CubeMesh = nullptr;

    void SpawnBlock(const FVector& RelativeLocation, const FVector& Scale, const FRotator& Rotation = FRotator::ZeroRotator);
    void BuildRoadsAndSidewalks();
    void BuildBuildings();
    void SpawnPrototypeActors();
};
