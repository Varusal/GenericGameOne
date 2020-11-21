


#include "BPVideoSettingsLib.h"
#include "Engine.h"

UGameUserSettings* UBPVideoSettingsLib::GetGameUserSettings()
{
	if (GEngine != nullptr)
	{
		return GEngine->GameUserSettings;
	}

	return nullptr;
}

bool UBPVideoSettingsLib::GetSupportedScreenResolutions(TArray<FString>& Resolutions)
{
	FScreenResolutionArray ResolutionsArray;
	FString RefreshRate;
	if (RHIGetAvailableResolutions(ResolutionsArray, false))
	{
		for (const FScreenResolutionRHI& Resolution : ResolutionsArray)
		{
			if (Resolution.RefreshRate == 60 || Resolution.RefreshRate == 143)
			{
				FString StrW = FString::FromInt(Resolution.Width);
				FString StrH = FString::FromInt(Resolution.Height);
				if (Resolution.RefreshRate == 143)
				{
					RefreshRate = FString::FromInt(144);
				}
				else
				{
					RefreshRate = FString::FromInt(Resolution.RefreshRate);
				}
				Resolutions.AddUnique(StrW + " x " + StrH + " @" + RefreshRate + " Hz");
			}
		}
		return true;
	}
	return false;
}

bool UBPVideoSettingsLib::SetScreenResolution(const int32 Width, const int32 Height, FString WindowMode)
{
	UGameUserSettings* Settings = GetGameUserSettings();
	if (!Settings)
	{
		false;
	}

	Settings->SetScreenResolution(FIntPoint(Width, Height));
	if (WindowMode == "Windowed")
	{
		Settings->SetFullscreenMode(EWindowMode::Windowed);
		return true;
	}
	else if (WindowMode == "Fullscreen")
	{
		Settings->SetFullscreenMode(EWindowMode::Fullscreen);
		return true;
	}
	else if (WindowMode == "WindowedFullscreen")
	{
		Settings->SetFullscreenMode(EWindowMode::WindowedFullscreen);
		return true;
	}
	return false;
}

bool UBPVideoSettingsLib::ApplyScreenResolution(const int32 Width, const int32 Height, FString WindowMode)
{
	UGameUserSettings* Settings = GetGameUserSettings();
	if (!Settings)
	{
		return false;
	}

	EWindowMode::Type AppliedWindowMode;
	if (WindowMode == "Windowed")
	{
		AppliedWindowMode = EWindowMode::Windowed;
	}
	else if (WindowMode == "Fullscreen")
	{
		AppliedWindowMode = EWindowMode::Fullscreen;
	}
	else if (WindowMode == "WindowedFullscreen")
	{
		AppliedWindowMode = EWindowMode::WindowedFullscreen;
	}

	if (!AppliedWindowMode)
	{
		return false;
	}

	Settings->RequestResolutionChange(Width, Height, AppliedWindowMode, false);
	return true;
}
