

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BPVideoSettingsLib.generated.h"

/**
 * 
 */
UCLASS()
class UE4FPS_API UBPVideoSettingsLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/** Get supported screen resolutions.*/
	UFUNCTION(BlueprintPure, Category = "Video Settings")
	static bool GetSupportedScreenResolutions(TArray<FString>& Resolutions);

	/** Set screen resolution.*/
	UFUNCTION(BlueprintPure, Category = "Video Settings")
	static bool SetScreenResolution(const int32 Width, const int32 Height, FString WindowMode);

	/** Apply screen resolution.*/
	UFUNCTION(BlueprintPure, Category = "Video Settings")
	static bool ApplyScreenResolution(const int32 Width, const int32 Height, FString WindowMode);

private:
	/** Try to get the GameUserSettings object from the engine.*/
	static class UGameUserSettings* GetGameUserSettings();
};
