#include <ntifs.h>
#include <wdm.h>

#include "RegistrationData.h"
#include "MiniFilter.h"
#include "Com.h"

// Global data
MINIFILTER_DATA MfData;
DRIVER_INITIALIZE DriverEntry;

NTSTATUS DriverEntry(__in PDRIVER_OBJECT DriverObject, __in PUNICODE_STRING RegistryPath)
{
	NTSTATUS status;
	OBJECT_ATTRIBUTES attribs;
	PSECURITY_DESCRIPTOR sd;
	UNICODE_STRING portName = RTL_CONSTANT_STRING(L"\\SandboxTest");

	UNREFERENCED_PARAMETER(RegistryPath);

	MfData.Driver = DriverObject;

	// Registering the minifilter driver
	status = FltRegisterFilter(DriverObject, &FilterRegistration, &MfData.Filter);

	KdPrint(("MINIFILTER INIT\n"));

	if (!NT_SUCCESS(status)) 
	{
		KdPrint(("MINIFILTER ERROR : FltRegisterFilter failed. Code 0x%x\n", status));
		return STATUS_UNSUCCESSFUL;
	}

	status = FltBuildDefaultSecurityDescriptor(&sd, FLT_PORT_ALL_ACCESS);
	if (!NT_SUCCESS(status))
		return status;

	InitializeObjectAttributes(&attribs, &portName, OBJ_KERNEL_HANDLE, NULL, sd);
	status = FltCreateCommunicationPort(MfData.Filter, &MfData.Port, &attribs, NULL, ConnectNotifyCallback, DisconnectNotifyCallback, MessageNotifyCallback, 1);

	if (!NT_SUCCESS(status))
		return status;

	FltFreeSecurityDescriptor(sd);

	status = FltStartFiltering(MfData.Filter);

	if (!NT_SUCCESS(status)) 
	{
		KdPrint(("MINIFILTER ERROR : FltStartFiltering faild. Code 0x%x\n", status));
		FltUnregisterFilter(MfData.Filter);
		return STATUS_UNSUCCESSFUL;
	}

	KdPrint(("MINIFILTER is Filtering!\n"));

	return status;
}

NTSTATUS MfUnload(__in FLT_FILTER_UNLOAD_FLAGS Flags)
{
	UNREFERENCED_PARAMETER(Flags);

	PAGED_CODE();

	KdPrint(("MINIFILTER UNLOAD\n"));

	FltUnregisterFilter(MfData.Filter);

	return STATUS_SUCCESS;
}

NTSTATUS MfQueryTearDown(__in PCFLT_RELATED_OBJECTS FltObjects, __in FLT_INSTANCE_QUERY_TEARDOWN_FLAGS Flags)
{
	UNREFERENCED_PARAMETER(FltObjects);
	UNREFERENCED_PARAMETER(Flags);

	PAGED_CODE();

	return STATUS_SUCCESS;
}