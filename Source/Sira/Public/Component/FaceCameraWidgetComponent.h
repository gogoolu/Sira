#pragma once

#include "Components/WidgetComponent.h"

#include "FaceCameraWidgetComponent.generated.h"


// widget component that always face to camera
UCLASS()
class UFaceCameraWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()
	
private:
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};

