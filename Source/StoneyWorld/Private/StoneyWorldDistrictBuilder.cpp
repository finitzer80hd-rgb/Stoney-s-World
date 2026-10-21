#include "StoneyWorldDistrictBuilder.h"

#include "StoneyWorldSystems.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Engine/StaticMeshActor.h"
#include "Engine/World.h"
#include "GameFramework/PlayerStart.h"
#include "UObject/ConstructorHelpers.h"

AStoneyWorldDistrictBuilder::AStoneyWorldDistrictBuilder()
{
    PrimaryActorTick.bCanEverTick = false;
    USceneComponent* SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    SetRootComponent(SceneRoot);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> Cube(TEXT("/Engine/BasicShapes/Cube.Cube"));
    if (Cube.Succeeded())
    {
        CubeMesh = Cube.Object;
    }
}

void AStoneyWorldDistrictBuilder::BeginPlay()
{
    Super::BeginPlay();
    if (!bBuildAtRuntime || !GetWorld() || !CubeMesh) return;

    BuildRoadsAndSidewalks();
    BuildBuildings();
    if (bSpawnGameplayActors) SpawnPrototypeActors();
}

void AStoneyWorldDistrictBuilder::SpawnBlock(const FVector& RelativeLocation, const FVector& Scale, const FRotator& Rotation)
{
    if (!GetWorld() || !CubeMesh) return;

    const FTransform Transform(Rotation, GetActorTransform().TransformPosition(RelativeLocation), GetActorScale3D() * Scale);
    AStaticMeshActor* Block = GetWorld()->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), Transform);
    if (!Block) return;

    UStaticMeshComponent* Mesh = Block->GetStaticMeshComponent();
    if (Mesh)
    {
        Mesh->SetStaticMesh(CubeMesh);
        Mesh->SetMobility(EComponentMobility::Movable);
        Mesh->SetCollisionProfileName(TEXT("BlockAll"));
        Mesh->SetGenerateOverlapEvents(false);
    }
    Block->SetActorLabel(TEXT("District - Prototype Block"));
}

void AStoneyWorldDistrictBuilder::BuildRoadsAndSidewalks()
{
    // Engine basic cube is 100 cm per side. The builder creates a 10 km x 10 km
    // foundation, two crossing roads, and raised sidewalks from collision-enabled cubes.
    SpawnBlock(FVector(0.f, 0.f, -80.f), FVector(100.f, 100.f, 1.f));

    // Cross streets. The broad road deck remains flat and easy to drive on.
    SpawnBlock(FVector(0.f, 0.f, -25.f), FVector(100.f, 14.f, 0.35f));
    SpawnBlock(FVector(0.f, 0.f, -24.f), FVector(14.f, 100.f, 0.35f));

    // Sidewalks bordering the central roads.
    for (float Side : { -1.f, 1.f })
    {
        SpawnBlock(FVector(0.f, Side * 760.f, 5.f), FVector(100.f, 1.2f, 0.22f));
        SpawnBlock(FVector(Side * 760.f, 0.f, 5.f), FVector(1.2f, 100.f, 0.22f));
    }

    // Simple dashed lane markers; each is a small, low-contrast cube.
    for (int32 I = -8; I <= 8; ++I)
    {
        const float Offset = I * 520.f;
        SpawnBlock(FVector(Offset, 0.f, -3.f), FVector(2.2f, 0.12f, 0.025f));
        SpawnBlock(FVector(0.f, Offset, -2.f), FVector(0.12f, 2.2f, 0.025f));
    }

    // Safehouse pad and mission forecourt.
    SpawnBlock(FVector(-1250.f, -1350.f, 8.f), FVector(12.f, 8.f, 0.18f));
}

void AStoneyWorldDistrictBuilder::BuildBuildings()
{
    const int32 Rows = FMath::Clamp(BuildingRowsPerSide, 1, 6);
    for (int32 XSide : { -1, 1 })
    {
        for (int32 YSide : { -1, 1 })
        {
            for (int32 Row = 0; Row < Rows; ++Row)
            {
                const float X = XSide * (1450.f + Row * BlockSpacing);
                const float Y = YSide * (1450.f + Row * BlockSpacing);
                const float Height = 450.f + ((Row * 173 + (XSide + 2) * 97 + (YSide + 2) * 59) % 7) * 145.f;
                const FVector BuildingScale(5.5f + (Row % 2) * 1.8f, 5.0f + ((Row + 1) % 2) * 1.5f, Height / 50.f);
                SpawnBlock(FVector(X, Y, Height), BuildingScale);
                // Low annex blocks create stepped rooflines and make the placeholder skyline less uniform.
                SpawnBlock(FVector(X + XSide * 360.f, Y - YSide * 260.f, Height * 0.38f), FVector(2.4f, 2.8f, Height * 0.38f / 50.f));
            }
        }
    }
}

void AStoneyWorldDistrictBuilder::SpawnPrototypeActors()
{
    const FVector Origin = GetActorLocation();
    const FRotator Facing = GetActorRotation();

    auto SpawnAt = [this, &Origin, &Facing](UClass* Class, const FVector& Offset)
    {
        if (!Class || !GetWorld()) return;
        const FVector Location = Origin + Facing.RotateVector(Offset);
        GetWorld()->SpawnActor<AActor>(Class, Location, Facing);
    };

    SpawnAt(AStoneyWorldMission::StaticClass(), FVector(900.f, 250.f, 120.f));
    SpawnAt(AStoneyWorldNPC::StaticClass(), FVector(1450.f, 450.f, 100.f));
    SpawnAt(AStoneyWorldNPC::StaticClass(), FVector(1800.f, -550.f, 100.f));
    SpawnAt(AStoneyWorldVehicle::StaticClass(), FVector(-800.f, 250.f, 80.f));

    // Spawn a PlayerStart for convenience when the level has none. For reliable startup,
    // manually place a PlayerStart in the editor before pressing Play (recommended).
    bool bHasPlayerStart = false;
    for (TActorIterator<APlayerStart> It(GetWorld()); It; ++It)
    {
        bHasPlayerStart = true;
        break;
    }
    if (!bHasPlayerStart)
    {
        GetWorld()->SpawnActor<APlayerStart>(APlayerStart::StaticClass(), Origin + Facing.RotateVector(FVector(-350.f, -350.f, 130.f)), Facing);
    }
}
