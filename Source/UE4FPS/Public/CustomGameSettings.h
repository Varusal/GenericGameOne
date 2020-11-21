

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CustomGameSettings.generated.h"

/**
 * 
 */
UCLASS(config = Game, defaultconfig)
class UE4FPS_API UCustomGameSettings : public UObject
{
	GENERATED_BODY()
	
public:
	UCustomGameSettings(const FObjectInitializer& ObjectInitializer);

	/** Framerate setting. */
	UPROPERTY(EditAnywhere, config, Category = "Custom")
	int Framerate;
};
