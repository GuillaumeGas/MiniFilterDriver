#include "Com.h"

NTSTATUS ConnectNotifyCallback (
	__in PFLT_PORT ClientPort,
	__in PVOID ServerPortCookie,
	__in PVOID ConnectionContext,
	__in ULONG SizeOfContext,
	__out PVOID * ConnectionPortCookie
	)
{
	UNREFERENCED_PARAMETER(ClientPort);
	UNREFERENCED_PARAMETER(ServerPortCookie);
	UNREFERENCED_PARAMETER(ConnectionContext);
	UNREFERENCED_PARAMETER(SizeOfContext);
	UNREFERENCED_PARAMETER(ConnectionPortCookie);

	return STATUS_SUCCESS;
}

VOID DisconnectNotifyCallback (
	__in PVOID ConnectionCookie
	)
{
	UNREFERENCED_PARAMETER(ConnectionCookie);
}

NTSTATUS MessageNotifyCallback (
	__in PVOID PortCookie,
	__in_opt PVOID InputBuffer,
	__in ULONG InputBufferLength,
	__out PVOID OutputBuffer,
	__in ULONG OutputBufferLength,
	__out PULONG ReturnOutputBufferLength
	)
{
	UNREFERENCED_PARAMETER(PortCookie);
	/*UNREFERENCED_PARAMETER(InputBuffer);
	UNREFERENCED_PARAMETER(InputBufferLength);*/
	UNREFERENCED_PARAMETER(OutputBuffer);
	UNREFERENCED_PARAMETER(OutputBufferLength);
	UNREFERENCED_PARAMETER(ReturnOutputBufferLength);

	if (InputBuffer != NULL && InputBufferLength > 0)
	{
		ULONG size = InputBufferLength;
	}

	return STATUS_SUCCESS;
}