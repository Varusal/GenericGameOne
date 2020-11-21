

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BPCustomSettingsLibrary.generated.h"

/**
 * 
 */
UCLASS()
class UE4FPS_API UBPCustomSettingsLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	

public:
	/** Get Framerate Setting.*/
	UFUNCTION(BlueprintPure, Category = "Framerate")
	static int32 GetSavedFrameRate();

	/** Set Framerate Setting.*/
	UFUNCTION(BlueprintPure, Category = "Framerate")
	static bool SetSavedFrameRate(int32 Framerate);

	/** Get mouse sensitivity.*/
	UFUNCTION(BlueprintPure, Category = "Controls")
	static float GetSavedMouseSensitivity();

	/** Set mouse sensitivity.*/
	UFUNCTION(BlueprintPure, Category = "Controls")
	static bool SetSavedMouseSensitivity(float Sensitivity);

	/** Get sound volume.*/
	UFUNCTION(BlueprintPure, Category = "Volume")
	static float GetSavedVolume();

	/** Set sound volume.*/
	UFUNCTION(BlueprintPure, Category = "Volume")
	static bool SetSavedVolume(float Volume);

	/** Get resolution Width.*/
	UFUNCTION(BlueprintPure, Category = "Resolution")
	static int GetSavedResolutionWidth();

	/** Get resolution Height.*/
	UFUNCTION(BlueprintPure, Category = "Resolution")
	static int GetSavedResolutionHeight();

	/** Set resolution.*/
	UFUNCTION(BlueprintPure, Category = "Resolution")
	static bool SetSavedResolution(int Width, int Height);

	/** Flush Config.*/
	UFUNCTION(BlueprintPure, Category = "Config")
	static bool FlushConfig();
};
