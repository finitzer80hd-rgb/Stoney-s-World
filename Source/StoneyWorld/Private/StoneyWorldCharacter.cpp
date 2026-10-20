#include "StoneyWorldCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

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
    Move->GetNavAgentPropertiesRef().bCanCrouch = true;

    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength = 360.f;
    CameraBoom->bUsePawnControlRotation = true;
    CameraBoom->SetRelativeLocation(FVector(0.f, 0.f, 72.f));

    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);

    AutoPossessPlayer = EAutoReceiveInput::Player0;
}

void AStoneyWorldCharacter::BeginPlay()
{
    Super::BeginPlay();
    UCharacterMovementComponent* Move = GetCharacterMovement();
    Move->MaxWalkSpeed = WalkSpeed;
    DefaultGroundFriction = Move->GroundFriction;
    DefaultAirControl = Move->AirControl;
}

void AStoneyWorldCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    UCharacterMovementComponent* Move = GetCharacterMovement();

    if (bSliding)
    {
        SlideTimeRemaining -= DeltaSeconds;

        // Preserve forward momentum without forcing the character to walk while airborne.
        if (Move->IsMovingOnGround())
        {
            AddMovementInput(GetActorForwardVector(), 0.65f);
        }

        if (SlideTimeRemaining <= 0.f || !Move->IsMovingOnGround())
        {
            EndSlide();
        }
    }

    if (bGrappling)
    {
        if (Move->IsMovingOnGround())
        {
            ReleaseGrapple();
        }
        else
        {
            UpdateGrapple(DeltaSeconds);
        }
    }

    if (bGliding)
    {
        if (Move->IsFalling())
        {
            FVector Velocity = Move->Velocity;
            Velocity.Z = FMath::Max(Velocity.Z, -GlideMaxFallSpeed);
            Move->Velocity = Velocity;
            Move->AirControl = GlideAirControl;
        }
        else
        {
            StopGlide();
        }
    }

    FinishMovementStatesOnLanding();
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

void AStoneyWorldCharacter::MoveForward(float Value)
{
    if (Controller && !FMath::IsNearlyZero(Value))
    {
        const FRotator Yaw(0.f, Controller->GetControlRotation().Yaw, 0.f);
        AddMovementInput(FRotationMatrix(Yaw).GetUnitAxis(EAxis::X), Value);
    }
}

void AStoneyWorldCharacter::MoveRight(float Value)
{
    if (Controller && !FMath::IsNearlyZero(Value))
    {
        const FRotator Yaw(0.f, Controller->GetControlRotation().Yaw, 0.f);
        AddMovementInput(FRotationMatrix(Yaw).GetUnitAxis(EAxis::Y), Value);
    }
}

void AStoneyWorldCharacter::Turn(float Value) { AddControllerYawInput(Value); }
void AStoneyWorldCharacter::LookUp(float Value) { AddControllerPitchInput(Value); }

void AStoneyWorldCharacter::StartSprint()
{
    bSprinting = true;
    if (!bSliding && !bIsCrouched)
    {
        GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
    }
}

void AStoneyWorldCharacter::StopSprint()
{
    bSprinting = false;
    if (!bSliding)
    {
        GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
    }
}

void AStoneyWorldCharacter::StartSlide()
{
    UCharacterMovementComponent* Move = GetCharacterMovement();
    if (bSliding || !Move->IsMovingOnGround() || GetVelocity().Size2D() < 300.f)
    {
        return;
    }

    bSliding = true;
    SlideTimeRemaining = SlideDuration;
    Move->GroundFriction = 0.5f;
    Move->BrakingDecelerationWalking = 900.f;

    // CharacterMovement uses MaxWalkSpeedCrouched while crouched, not MaxWalkSpeed.
    // Raise the crouched limit for the duration of the slide or the slide is capped at 360.
    Move->MaxWalkSpeedCrouched = SlideSpeed;
    Move->MaxWalkSpeed = SlideSpeed;
    Crouch();
}

void AStoneyWorldCharacter::EndSlide()
{
    if (!bSliding)
    {
        return;
    }

    bSliding = false;
    UnCrouch();

    UCharacterMovementComponent* Move = GetCharacterMovement();
    Move->GroundFriction = DefaultGroundFriction;
    Move->BrakingDecelerationWalking = 2400.f;
    Move->MaxWalkSpeedCrouched = 360.f;
    Move->MaxWalkSpeed = bSprinting ? SprintSpeed : WalkSpeed;
}

void AStoneyWorldCharacter::FireGrapple()
{
    if (!Controller || bGrappling)
    {
        return;
    }

    FVector ViewLocation;
    FRotator ViewRotation;
    Controller->GetPlayerViewPoint(ViewLocation, ViewRotation);

    FHitResult Hit;
    FCollisionQueryParams Params(SCENE_QUERY_STAT(StoneyGrapple), false, this);
    const FVector TraceEnd = ViewLocation + ViewRotation.Vector() * GrappleRange;

    if (GetWorld()->LineTraceSingleByChannel(Hit, ViewLocation, TraceEnd, ECC_Visibility, Params))
    {
        // Ignore hits that are effectively underfoot or too close for a useful swing.
        const float Distance = FVector::Distance(GetActorLocation(), Hit.ImpactPoint);
        if (Distance < 220.f)
        {
            return;
        }

        GrappleAnchor = Hit.ImpactPoint;
        GrappleLength = FMath::Clamp(Distance, 180.f, GrappleRange);
        bGrappling = true;
        bGliding = false;
        GetCharacterMovement()->SetMovementMode(MOVE_Falling);
    }
}

void AStoneyWorldCharacter::UpdateGrapple(float DeltaSeconds)
{
    UCharacterMovementComponent* Move = GetCharacterMovement();
    const FVector CharacterLocation = GetActorLocation();
    FVector ToAnchor = GrappleAnchor - CharacterLocation;
    const float Distance = ToAnchor.Size();

    DrawDebugLine(GetWorld(), CharacterLocation, GrappleAnchor, FColor::Yellow, false, 0.f, 0, 2.f);

    if (Distance < 150.f)
    {
        ReleaseGrapple();
        return;
    }

    const FVector DirectionToAnchor = ToAnchor / Distance;

    // Apply pull acceleration, then enforce a maximum rope length while retaining tangential velocity.
    Move->Velocity += DirectionToAnchor * GrapplePullAcceleration * DeltaSeconds;
    const float RadialVelocity = FVector::DotProduct(Move->Velocity, DirectionToAnchor);

    if (Distance > GrappleLength)
    {
        const float ExcessDistance = Distance - GrappleLength;
        const FVector CorrectedLocation = CharacterLocation + DirectionToAnchor * ExcessDistance;
        SetActorLocation(CorrectedLocation, true);

        // DirectionToAnchor points inward; negative radial velocity is moving away from the anchor.
        if (RadialVelocity < 0.f)
        {
            Move->Velocity -= DirectionToAnchor * RadialVelocity;
        }
    }

    Move->Velocity = Move->Velocity.GetClampedToMaxSize(2400.f);
}

void AStoneyWorldCharacter::ReleaseGrapple()
{
    bGrappling = false;
}

void AStoneyWorldCharacter::ReelGrapple(float Value)
{
    if (bGrappling && !FMath::IsNearlyZero(Value))
    {
        GrappleLength = FMath::Clamp(
            GrappleLength - Value * ReelSpeed * GetWorld()->GetDeltaSeconds(),
            180.f,
            GrappleRange);
    }
}

void AStoneyWorldCharacter::DeployGlide()
{
    if (GetCharacterMovement()->IsFalling())
    {
        bGliding = true;
        bGrappling = false;
        GetCharacterMovement()->AirControl = GlideAirControl;
    }
}

void AStoneyWorldCharacter::StopGlide()
{
    if (!bGliding)
    {
        return;
    }

    bGliding = false;
    GetCharacterMovement()->AirControl = DefaultAirControl;
}

void AStoneyWorldCharacter::FinishMovementStatesOnLanding()
{
    UCharacterMovementComponent* Move = GetCharacterMovement();

    if (!Move->IsFalling())
    {
        if (bGrappling)
        {
            ReleaseGrapple();
        }
        if (bGliding)
        {
            StopGlide();
        }
        if (bSliding)
        {
            EndSlide();
        }
    }
}
