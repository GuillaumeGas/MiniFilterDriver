#pragma once

#include <wdm.h>
#include <WinDef.h>

#define SB_TAG 'SB__'

enum SandState
{
	INACTIVE = 0,
	ACTIVE = 1
};

struct SandBox
{
	enum SandState state;
	UNICODE_STRING targetProcessName;
	UNICODE_STRING sandDirectoryPath;
};

struct SandBox gSandBox;

BOOL InitSandBox (WCHAR * buffer, ULONG bufferLength);
VOID FreeSandBox (struct SandBox sandbox);