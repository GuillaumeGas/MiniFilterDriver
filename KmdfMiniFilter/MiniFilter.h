#pragma once

#include <fltKernel.h>

typedef struct MINIFILTER_DATA {
	PDRIVER_OBJECT Driver;
	PFLT_FILTER Filter;
} MINIFILTER_DATA;

FLT_PREOP_CALLBACK_STATUS MfPreOperationCallback(
	__inout PFLT_CALLBACK_DATA Data,
	__in PCFLT_RELATED_OBJECTS FltObjects,
	PVOID *CompletionContext
);

FLT_POSTOP_CALLBACK_STATUS MfPostOperationCallback(
	__inout PFLT_CALLBACK_DATA Data,
	__in PCFLT_RELATED_OBJECTS FltObjects,
	__in PVOID CompletionContext,
	__in FLT_POST_OPERATION_FLAGS Flags
);

NTSTATUS MfInstanceSetup(
	__in PCFLT_RELATED_OBJECTS FltObjects,
    __in FLT_INSTANCE_SETUP_FLAGS Flags,
    __in DEVICE_TYPE VolumeDeviceType,
    __in FLT_FILESYSTEM_TYPE VolumeFilesystemType
);

NTSTATUS MfInstanceQueryTeardown (
    __in PCFLT_RELATED_OBJECTS FltObjects,
    __in FLT_INSTANCE_QUERY_TEARDOWN_FLAGS Flags
);

VOID MfInstanceTeardownStart (
    __in PCFLT_RELATED_OBJECTS FltObjects,
    __in FLT_INSTANCE_TEARDOWN_FLAGS Flags
);

VOID MfInstanceTeardownComplete (
    __in PCFLT_RELATED_OBJECTS FltObjects,
    __in FLT_INSTANCE_TEARDOWN_FLAGS Flags
);

VOID MfDeleteTxfContext(
	__inout PFLT_CONTEXT Context,
	__in FLT_CONTEXT_TYPE ContextType
);

NTSTATUS MfUnload(
	__in FLT_FILTER_UNLOAD_FLAGS Flags
);

NTSTATUS MfQueryTearDown(
	__in PCFLT_RELATED_OBJECTS FltObjects,
	__in FLT_INSTANCE_QUERY_TEARDOWN_FLAGS Flags
);

NTSTATUS MfInstanceQueryTeardown(
	__in PCFLT_RELATED_OBJECTS FltObjects,
	__in FLT_INSTANCE_QUERY_TEARDOWN_FLAGS Flags
);