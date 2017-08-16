// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "Odin.h"
#if PLATFORM_WINDOWS
#include "WindowsHWrapper.h"
#include <ShlObj.h>
#endif

// Implementation of generated SkookumScript bindings
#include <SkUEProjectGeneratedBindings.generated.inl> 

DEFINE_LOG_CATEGORY(LogOdinPerf);
DEFINE_LOG_CATEGORY(LogOdinMultipliers);


class FOdinGameModule : public FDefaultGameModuleImpl
{
	virtual void StartupModule() override 
	{
		// Register the shell extensions on Windows
#if PLATFORM_WINDOWS && !WITH_EDITOR
		FString ModInstallerPath = FPaths::ConvertRelativePathToFull(FPaths::GameDir() / TEXT("Binaries/Win64/RoboRecallModInstaller.exe"));
		FPaths::MakePlatformFilename(ModInstallerPath);

		HKEY hKey;
		if(SUCCEEDED(::RegCreateKeyEx(HKEY_CURRENT_USER, TEXT("SOFTWARE\\Classes\\RoboRecallMod\\shell\\open\\command"), 0, nullptr, REG_OPTION_NON_VOLATILE, KEY_WRITE, nullptr, &hKey, nullptr)))
		{
			FString Command = FString::Printf(TEXT("\"%s\" \"%%1\""), *ModInstallerPath);
			::RegSetValueEx(hKey, NULL, 0, REG_SZ, (const BYTE*)*Command, (Command.Len() + 1) * sizeof(TCHAR));
			::RegCloseKey(hKey);
		}
		if(SUCCEEDED(::RegCreateKeyEx(HKEY_CURRENT_USER, TEXT("SOFTWARE\\Classes\\.robo"), 0, nullptr, REG_OPTION_NON_VOLATILE, KEY_WRITE, nullptr, &hKey, nullptr)))
		{
			FString Value = TEXT("RoboRecallMod");
			::RegSetValueEx(hKey, NULL, 0, REG_SZ, (const BYTE*)*Value, (Value.Len() + 1) * sizeof(TCHAR));
			::RegCloseKey(hKey);
		}
		if(SUCCEEDED(::RegCreateKeyEx(HKEY_CURRENT_USER, TEXT("SOFTWARE\\Classes\\RoboRecallMod\\DefaultIcon"), 0, nullptr, REG_OPTION_NON_VOLATILE, KEY_WRITE, nullptr, &hKey, nullptr)))
		{
			FString CurrentValue;

			// Capture the current value
			::DWORD Size = 0;
			if(SUCCEEDED(::RegQueryValueEx(hKey, NULL, NULL, NULL, NULL, &Size)) && Size > 0)
			{
				char *Buffer = new char[Size];
				if(SUCCEEDED(RegQueryValueEx(hKey, NULL, NULL, NULL, (LPBYTE)Buffer, &Size )))
				{
					CurrentValue = FString(Size - 1, (TCHAR*)Buffer);
					CurrentValue.TrimToNullTerminator();
				}
				delete[] Buffer;
			}

			// Write the new value and update the icon cache if it's different
			FString Value = FString::Printf(TEXT("\"%s\""), *ModInstallerPath);
			if (Value != CurrentValue)
			{
				::RegSetValueEx(hKey, NULL, 0, REG_SZ, (const BYTE*)*Value, (Value.Len() + 1) * sizeof(TCHAR));
				SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_IDLIST, NULL, NULL);
			}
			::RegCloseKey(hKey);
		}
#endif
	}
};

IMPLEMENT_PRIMARY_GAME_MODULE( FOdinGameModule, Odin, "Odin" );
