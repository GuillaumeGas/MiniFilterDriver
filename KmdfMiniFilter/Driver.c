#include <ntifs.h>
#include <wdf.h>

#include "RegistrationData.h"
#include "MiniFilter.h"

// Global data
MINIFILTER_DATA MfData;

DRIVER_INITIALIZE DriverEntry;

NTSTATUS DriverEntry(_In_ PDRIVER_OBJECT DriverObject, _In_ PUNICODE_STRING RegistryPath)
{
	UNREFERENCED_PARAMETER(RegistryPath);
	//DbgSetDebugFilterState(DPFLTR_IHVDRIVER_ID, DPFLTR_INFO_LEVEL, TRUE);

	NTSTATUS status;

	MfData.Driver = DriverObject;

	// Registering the minifilter driver
	status = FltRegisterFilter(DriverObject, &FilterRegistration, &MfData.Filter);

	KdPrint(("MINIFILTER INIT\n"));

	if (!NT_SUCCESS(status)) {
		KdPrint(("MINIFILTER ERROR : FltRegisterFilter failed. Code 0x%x\n", status));
		return STATUS_UNSUCCESSFUL;
	}

	status = FltStartFiltering(MfData.Filter);

	if (!NT_SUCCESS(status)) {
		KdPrint(("MINIFILTER ERROR : FltStartFiltering faild. Code 0x%x\n", status));
		FltUnregisterFilter(MfData.Filter);
		return STATUS_UNSUCCESSFUL;
	}

	KdPrint(("MINIFILTER is Filtering!\n"));

	return status;
}

NTSTATUS MfUnload(_In_ FLT_FILTER_UNLOAD_FLAGS Flags)
{
	UNREFERENCED_PARAMETER(Flags);

	PAGED_CODE();

	KdPrint(("MINIFILTER UNLOAD\n"));

	FltUnregisterFilter(MfData.Filter);

	return STATUS_SUCCESS;
}

NTSTATUS MfQueryTearDown(_In_ PCFLT_RELATED_OBJECTS FltObjects, _In_ FLT_INSTANCE_QUERY_TEARDOWN_FLAGS Flags)
{
	UNREFERENCED_PARAMETER(FltObjects);
	UNREFERENCED_PARAMETER(Flags);

	PAGED_CODE();

	return STATUS_SUCCESS;
}