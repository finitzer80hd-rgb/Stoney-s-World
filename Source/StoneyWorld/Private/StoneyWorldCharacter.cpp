#include "StoneyWorldCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "DrawDebugHelpers.h"

AStoneyWorldCharacter::AStoneyWorldCharacter()
{
    PrimaryActorTick.bCanEverTick = true;
    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;
    UCharacterMovementComponent* Move = GetCharacterMovement();
    Move->MaxWalkSpeed = WalkSpeed;
    Move->MaxWalkSpeedCrouched = 360.f;
    Move->MaxAcceleration = 4096.f;
    Move->BrakingDecelerationWalking = 2400.f;
    Move->AirControl = 0.8f;
    Move->JumpZVelocity = 720.f;
    Move->GravityScale = 1.15f;
    Move->RotationRate = FRotator(0.f, 720.f, 0.f);
    Move->bOrientRotationToMovement = true;

    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength = 360.f;
    CameraBoom->bUsePawnControlRotation = true;
    CameraBoom->SetRelativeLocation(FVector(0,0,72));
    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    AutoPossessPlayer = EAutoReceiveInput::Player0;
}
void AStoneyWorldCharacter::BeginPlay()
{
    Super::BeginPlay();
    GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}
void AStoneyWorldCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    if (bSliding)
    {
        SlideTime -= DeltaSeconds;
        AddMovementInput(GetActorForwardVector(), 1.f);
        if (SlideTime <= 0.f || GetCharacterMovement()->IsMovingOnGround() == false)
        {
            bSliding = false;
            GetCharacterMovement()->MaxWalkSpeed = bSprinting ? SprintSpeed : WalkSpeed;
            GetCharacterMovement()->SetMovementMode(MOVE_Walking);
            UnCrouch();
            GetCharacterMovement()->GroundFriction = 8.f;
        }
    }
    if (bGliding && GetCharacterMovement()->IsFalling())
    {
        FVector V = GetVelocity();
        V.Z = FMath::Max(V.Z, -350.f);
        GetCharacterMovement()->Velocity = V;
        GetCharacterMovement()->AirControl = 1.f;
    }
    else if (bGrappling) UpdateGrapple(DeltaSeconds);
}
void AStoneyWorldCharacter::SetupPlayerInputComponent(UInputComponent* Input)
{
    Super::SetupPlayerInputComponent(Input);
    Input->BindAxis("MoveForward", this, &AStoneyWorldCharacter::MoveForward);
    Input->BindAxis("MoveRight", this, &AStoneyWorldCharacter::MoveRight);
    Input->BindAxis("Turn", this, &AStoneyWorldCharacter::Turn);
    Input->BindAxis("LookUp", this, &AStoneyWorldCharacter::LookUp);
    Input->BindAxis("GrappleReel", this, &AStoneyWorldCharacter::ReelGrapple);
    Input->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
    Input->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
    Input->BindAction("Sprint", IE_Pressed, this, &AStoneyWorldCharacter::StartSprint);
    Input->BindAction("Sprint", IE_Released, this, &AStoneyWorldCharacter::StopSprint);
    Input->BindAction("Slide", IE_Pressed, this, &AStoneyWorldCharacter::StartSlide);
    Input->BindAction("Grapple", IE_Pressed, this, &AStoneyWorldCharacter::FireGrapple);
    Input->BindAction("Grapple", IE_Released, this, &AStoneyWorldCharacter::ReleaseGrapple);
    Input->BindAction("Glide", IE_Pressed, this, &AStoneyWorldCharacter::DeployGlide);
    Input->BindAction("Glide", IE_Released, this, &AStoneyWorldCharacter::StopGlide);
}
void AStoneyWorldCharacter::MoveForward(float V)
{
    if (Controller && !FMath::IsNearlyZero(V))
    {
        const FRotator Yaw(0, Controller->GetControlRotation().Yaw, 0);
        AddMovementInput(FRotationMatrix(Yaw).GetUnitAxis(EAxis::X), V);
    }
}
void AStoneyWorldCharacter::MoveRight(float V)
{
    if (Controller && !FMath::IsNearlyZero(V))
    {
        const FRotator Yaw(0, Controller->GetControlRotation().Yaw, 0);
        AddMovementInput(FRotationMatrix(Yaw).GetUnitAxis(EAxis::Y), V);
    }
}
void AStoneyWorldCharacter::Turn(float V) { AddControllerYawInput(V); }
void AStoneyWorldCharacter::LookUp(float V) { AddControllerPitchInput(V); }
void AStoneyWorldCharacter::StartSprint()
{
    bSprinting = true;
    if (!bSliding) GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
}
void AStoneyWorldCharacter::StopSprint()
{
    bSprinting = false;
    if (!bSliding) GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}
void AStoneyWorldCharacter::StartSlide()
{
    if (!GetCharacterMovement()->IsMovingOnGround() || GetVelocity().Size2D() < 250.f) return;
    bSliding = true; SlideTime = 0.85f;
    GetCharacterMovement()->MaxWalkSpeed = SlideSpeed;
    GetCharacterMovement()->GroundFriction = 0.5f;
    Crouch();
}
void AStoneyWorldCharacter::FireGrapple()
{
    if (!Controller) return;
    FVector Start; FRotator Rot;
    Controller->GetPlayerViewPoint(Start, Rot);
    FHitResult Hit;
    FCollisionQueryParams Params(SCENE_QUERY_STAT(StoneyGrapple), false, this);
    if (GetWorld()->LineTraceSingleByChannel(Hit, Start, Start + Rot.Vector() * GrappleRange, ECC_Visibility, Params))
    {
        GrappleAnchor = Hit.ImpactPoint;
        GrappleLength = FVector::Distance(GetActorLocation(), GrappleAnchor);
        bGrappling = true;
        bGliding = false;
        GetCharacterMovement()->SetMovementMode(MOVE_Falling);
    }
}
void AStoneyWorldCharacter::UpdateGrapple(float DeltaSeconds)
{
    DrawDebugLine(GetWorld(), GetActorLocation(), GrappleAnchor, FColor::Yellow, false, 0.f, 0, 2.f);
    FVector ToAnchor = GrappleAnchor - GetActorLocation();
    const float Distance = ToAnchor.Size();
    if (Distance < 160.f) { ReleaseGrapple(); return; }
    const FVector Direction = ToAnchor.GetSafeNormal();
    FVector V = GetCharacterMovement()->Velocity;
    // Pull inward while retaining tangential momentum for a simple swing feel.
    const float Inward = FVector::DotProduct(V, Direction);
    V += Direction * FMath::Max(0.f, ReelSpeed * DeltaSeconds - Inward * 0.15f);
    const FVector Radial = Direction * FVector::DotProduct(V, Direction);
    V -= Radial * 0.12f;
    GetCharacterMovement()->Velocity = V.GetClampedToMaxSize(2400.f);
    GrappleLength = FMath::Max(150.f, GrappleLength - 80.f * DeltaSeconds);
}
void AStoneyWorldCharacter::ReleaseGrapple() { bGrappling = false; }
void AStoneyWorldCharacter::ReelGrapple(float V)
{
    if (bGrappling && !FMath::IsNearlyZero(V))
        GrappleLength = FMath::Clamp(GrappleLength - V * ReelSpeed * GetWorld()->GetDeltaSeconds(), 150.f, GrappleRange);
}
void AStoneyWorldCharacter::DeployGlide()
{
    if (GetCharacterMovement()->IsFalling()) { bGliding = true; bGrappling = false; }
}
void AStoneyWorldCharacter::StopGlide()
{
    bGliding = false;
    GetCharacterMovement()->AirControl = 0.8f;

}
