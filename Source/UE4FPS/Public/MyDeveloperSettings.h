

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "MyDeveloperSettings.generated.h"

/**
 * 
 */
UCLASS(config = GameSettings, Blueprintable, BlueprintType, defaultconfig, meta = (DisplayName = "My Settings"))
class UE4FPS_API UMyDeveloperSettings : public UDeveloperSettings
{
	GENERATED_BODY()
	
public:
	/** Framerate setting. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, config, Category = "Custom")
	int32 Framerate;
};
