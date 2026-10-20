#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "StoneyWorldCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;

UCLASS()
class STONEYWORLD_API AStoneyWorldCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    AStoneyWorldCharacter();
    virtual void Tick(float DeltaSeconds) override;

protected:
    virtual void BeginPlay() override;
    virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

    void MoveForward(float Value);
    void MoveRight(float Value);
    void Turn(float Value);
    void LookUp(float Value);
    void StartSprint();
    void StopSprint();
    void StartSlide();
    void EndSlide();
    void FireGrapple();
    void ReleaseGrapple();
    void ReelGrapple(float Value);
    void DeployGlide();
    void StopGlide();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camera")
    TObjectPtr<USpringArmComponent> CameraBoom;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camera")
    TObjectPtr<UCameraComponent> FollowCamera;

    UPROPERTY(EditAnywhere, Category="Movement", meta=(ClampMin="100.0"))
    float WalkSpeed = 600.f;

    UPROPERTY(EditAnywhere, Category="Movement", meta=(ClampMin="100.0"))
    float SprintSpeed = 1050.f;

    UPROPERTY(EditAnywhere, Category="Movement", meta=(ClampMin="100.0"))
    float SlideSpeed = 1250.f;

    UPROPERTY(EditAnywhere, Category="Movement", meta=(ClampMin="0.1"))
    float SlideDuration = 0.85f;

    UPROPERTY(EditAnywhere, Category="Grapple", meta=(ClampMin="100.0"))
    float GrappleRange = 4500.f;

    UPROPERTY(EditAnywhere, Category="Grapple", meta=(ClampMin="100.0"))
    float ReelSpeed = 1800.f;

    UPROPERTY(EditAnywhere, Category="Grapple", meta=(ClampMin="0.0"))
    float GrapplePullAcceleration = 2200.f;

    UPROPERTY(EditAnywhere, Category="Glide", meta=(ClampMin="0.0"))
    float GlideMaxFallSpeed = 350.f;

    UPROPERTY(EditAnywhere, Category="Glide", meta=(ClampMin="0.0"))
    float GlideAirControl = 1.0f;

private:
    bool bSprinting = false;
    bool bSliding = false;
    bool bGliding = false;
    bool bGrappling = false;

    FVector GrappleAnchor = FVector::ZeroVector;
    float GrappleLength = 0.f;
    float SlideTimeRemaining = 0.f;
    float DefaultGroundFriction = 8.f;
    float DefaultAirControl = 0.8f;

    void UpdateGrapple(float DeltaSeconds);
    void FinishMovementStatesOnLanding();
};
