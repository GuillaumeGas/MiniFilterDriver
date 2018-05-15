#pragma once

#include <fltKernel.h>
#include <wdm.h>
#include <WinDef.h>

typedef NTSTATUS (*QUERY_INFO_PROCESS) (
	__in HANDLE ProcessHandle,
	__in PROCESSINFOCLASS ProcessInformationClass,
	__out_bcount(ProcessInformationLength) PVOID ProcessInformation,
	__in ULONG ProcessInformationLength,
	__out_opt PULONG ReturnLength
);

BOOL GetProcessNameById (__in ULONG processId, __out PUNICODE_STRING processName);
VOID GetExeNameFromFullPath(__inout PUNICODE_STRING processName);