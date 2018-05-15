#pragma once

#include <fltKernel.h>
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

//#ifdef __SANDBOX__
//struct SandBox gSandBox;
//#else
extern struct SandBox gSandBox;
//#endif

BOOL InitSandBox (WCHAR * buffer, ULONG bufferLength);
VOID FreeSandBox (struct SandBox sandbox);

BOOL IsTarget (UNICODE_STRING targetFileName);
VOID FilterPreOperation (PFLT_IO_PARAMETER_BLOCK ioParameter);