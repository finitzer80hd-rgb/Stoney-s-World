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
    void FireGrapple();
    void ReleaseGrapple();
    void ReelGrapple(float Value);
    void DeployGlide();
    void StopGlide();
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly) TObjectPtr<USpringArmComponent> CameraBoom;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly) TObjectPtr<UCameraComponent> FollowCamera;
    UPROPERTY(EditAnywhere, Category="Movement") float WalkSpeed = 600.f;
    UPROPERTY(EditAnywhere, Category="Movement") float SprintSpeed = 1050.f;
    UPROPERTY(EditAnywhere, Category="Movement") float SlideSpeed = 1250.f;
    UPROPERTY(EditAnywhere, Category="Grapple") float GrappleRange = 4500.f;
    UPROPERTY(EditAnywhere, Category="Grapple") float ReelSpeed = 1800.f;
private:
    bool bSprinting = false;
    bool bSliding = false;
    bool bGliding = false;
    bool bGrappling = false;
    FVector GrappleAnchor = FVector::ZeroVector;
    float GrappleLength = 0.f;
    float SlideTime = 0.f;
    void UpdateGrapple(float DeltaSeconds);
};
