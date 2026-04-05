#include "Component/FaceCameraWidgetComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

void UFaceCameraWidgetComponent::TickComponent(float DeltaTime, enum ELevelTick TickType,
                                              FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (APlayerCameraManager* CameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0))
	{
		SetWorldRotation(UKismetMathLibrary::FindLookAtRotation(GetComponentLocation(), CameraManager->GetCameraLocation()));
	}
}
