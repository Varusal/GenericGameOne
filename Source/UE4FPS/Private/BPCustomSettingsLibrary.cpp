


#include "BPCustomSettingsLibrary.h"
#include "Runtime/Core/Public/Misc/ConfigCacheIni.h"

int32 UBPCustomSettingsLibrary::GetSavedFrameRate()
{
	FString CustomSettingsSection = "CustomSettings.VideoSettings";
	int Framerate = 0;
	GConfig->GetInt(*CustomSettingsSection, TEXT("Framerate"), Framerate, GGameIni);
	return Framerate;
}

bool UBPCustomSettingsLibrary::SetSavedFrameRate(int32 Framerate)
{
	FString CustomSettingsSection = "CustomSettings.VideoSettings";
	GConfig->SetInt(*CustomSettingsSection, TEXT("Framerate"), Framerate, GGameIni);

	FlushConfig();

	int ConfigFrameRate = GetSavedFrameRate();
	if (ConfigFrameRate == Framerate)
	{
		return true;
	}
	return false;
}

float UBPCustomSettingsLibrary::GetSavedMouseSensitivity()
{
	FString CustomSettingsSection = "CustomSettings.Controls";
	float Sensitivity = 0.0f;
	GConfig->GetFloat(*CustomSettingsSection, TEXT("MouseSensitivity"), Sensitivity, GGameIni);
	return Sensitivity;
}

bool UBPCustomSettingsLibrary::SetSavedMouseSensitivity(float Sensitivity)
{
	FString CustomSettingsSection = "CustomSettings.Controls";
	GConfig->SetFloat(*CustomSettingsSection, TEXT("MouseSensitivity"), Sensitivity, GGameIni);

	FlushConfig();

	float ConfigSensitivity = GetSavedMouseSensitivity();
	if (ConfigSensitivity == Sensitivity)
	{
		return true;
	}
	return false;
}

float UBPCustomSettingsLibrary::GetSavedVolume()
{
	FString CustomSettingsSection = "CustomSettings.Sound";
	float Volume = 0.0f;
	GConfig->GetFloat(*CustomSettingsSection, TEXT("SoundVolume"), Volume, GGameIni);
	return Volume;
}

bool UBPCustomSettingsLibrary::SetSavedVolume(float Volume)
{
	FString CustomSettingsSection = "CustomSettings.Sound";
	GConfig->SetFloat(*CustomSettingsSection, TEXT("SoundVolume"), Volume, GGameIni);

	FlushConfig();

	float ConfigVolume = GetSavedVolume();
	if (ConfigVolume == Volume)
	{
		return true;
	}
	return false;
}

int UBPCustomSettingsLibrary::GetSavedResolutionWidth()
{
	FString CustomSettingsSection = "CustomSettings.Resolution";
	int Width = 0;
	GConfig->GetInt(*CustomSettingsSection, TEXT("ResolutionWidth"), Width, GGameIni);
	return Width;
}

int UBPCustomSettingsLibrary::GetSavedResolutionHeight()
{
	FString CustomSettingsSection = "CustomSettings.Resolution";
	int Height = 0;
	GConfig->GetInt(*CustomSettingsSection, TEXT("ResolutionHeight"), Height, GGameIni);
	return Height;
}

bool UBPCustomSettingsLibrary::SetSavedResolution(int Width, int Height)
{
	FString CustomSettingsSection = "CustomSettings.Resolution";
	GConfig->SetInt(*CustomSettingsSection, TEXT("ResolutionWidth"), Width, GGameIni);
	GConfig->SetInt(*CustomSettingsSection, TEXT("ResolutionHeight"), Height, GGameIni);

	FlushConfig();
	return true;
}

bool UBPCustomSettingsLibrary::FlushConfig()
{
	GConfig->Flush(false, GGameIni);
	return true;
}
