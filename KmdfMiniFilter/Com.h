#pragma once

#include <fltKernel.h>

NTSTATUS ConnectNotifyCallback (
	__in PFLT_PORT ClientPort,
	__in PVOID ServerPortCookie,
	__in PVOID ConnectionContext,
	__in ULONG SizeOfContext,
	__out PVOID * ConnectionPortCookie
	);

VOID DisconnectNotifyCallback (
	__in PVOID ConnectionCookie
	);

NTSTATUS MessageNotifyCallback (
	__in PVOID PortCookie,
	__in_opt PVOID InputBuffer,
	__in ULONG InputBufferLength,
	__out PVOID OutputBuffer,
	__in ULONG OutputBufferLength,
	__out PULONG ReturnOutputBufferLength
	);