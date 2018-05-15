#include "MiniFilter.h"
#include "Sandbox.h"
#include "Utils.h"

FLT_PREOP_CALLBACK_STATUS MfPreOperationCallback(
	__in PFLT_CALLBACK_DATA Data,
	__in PCFLT_RELATED_OBJECTS FltObjects,
	PVOID *CompletionContext
)
{
	UNREFERENCED_PARAMETER(FltObjects);
	UNREFERENCED_PARAMETER(CompletionContext);

	if (gSandBox.state == ACTIVE)
	{
		//// We check if we have a file system miifilter callback operation
		////if (Data->Flags & FLTFL_CALLBACK_DATA_FS_FILTER_OPERATION)
		////{
		//	if (IsTarget(Data->Iopb->TargetFileObject->FileName))
		//	{
		//		__debugbreak();
		//		FilterPreOperation (Data->Iopb);
		//		FltSetCallbackDataDirty(Data); // We indicate that the content of the Data structure has been modified.
		//	}
		////}

		//NTSTATUS status;
		//PFLT_FILE_NAME_INFORMATION fileInfo;

		//status = FltGetFileNameInformation (Data, FLT_FILE_NAME_OPENED, &fileInfo);

		//if (status != STATUS_SUCCESS)
		//{
		//	if (status == STATUS_FLT_INVALID_NAME_REQUEST)
		//		return FLT_PREOP_SUCCESS_WITH_CALLBACK;
		//	if (status == STATUS_INSUFFICIENT_RESOURCES)
		//		return FLT_PREOP_SUCCESS_WITH_CALLBACK;
		//	if (status == STATUS_INVALID_PARAMETER)
		//		return FLT_PREOP_SUCCESS_WITH_CALLBACK;
		//	if (status == STATUS_FLT_NAME_CACHE_MISS)
		//		return FLT_PREOP_SUCCESS_WITH_CALLBACK;
		//	if (status == STATUS_ACCESS_DENIED)
		//		return FLT_PREOP_SUCCESS_WITH_CALLBACK;
		//}

		//if (IsTarget(fileInfo->Name))
		//{
		//	__debugbreak();
		//	FilterPreOperation (Data->Iopb);
		//	FltSetCallbackDataDirty(Data); // We indicate that the content of the Data structure has been modified.
		//}

		UNICODE_STRING callerFileName = {0};
		ULONG processId = 0	;

		// We retrieve the caller's process id
		processId = FltGetRequestorProcessId (Data);

		if (processId > 0)
		{
			if (GetProcessNameById (processId, &callerFileName))
			{
				GetExeNameFromFullPath (&callerFileName);

				if (IsTarget (callerFileName))
				{
					__debugbreak();
				}
			}
		}
	}

	return FLT_PREOP_SUCCESS_WITH_CALLBACK;
}

FLT_POSTOP_CALLBACK_STATUS MfPostOperationCallback(
	__in PFLT_CALLBACK_DATA Data,
	__in PCFLT_RELATED_OBJECTS FltObjects,
	__in PVOID CompletionContext,
	__in FLT_POST_OPERATION_FLAGS Flags
)
{
	UNREFERENCED_PARAMETER(Data);
	UNREFERENCED_PARAMETER(FltObjects);
	UNREFERENCED_PARAMETER(CompletionContext);
	UNREFERENCED_PARAMETER(Flags);

	KdPrint(("MINIFILTER PostOperationCallback\n"));

	return FLT_POSTOP_FINISHED_PROCESSING;
}



VOID MfDeleteTxfContext(
	__inout PFLT_CONTEXT Context,
	__in FLT_CONTEXT_TYPE ContextType
)
{
	UNREFERENCED_PARAMETER(Context);
	UNREFERENCED_PARAMETER(ContextType);

	KdPrint(("MINIFILTER DeleteTxfContext\n"));
}

NTSTATUS
MfInstanceSetup (
    __in PCFLT_RELATED_OBJECTS FltObjects,
    __in FLT_INSTANCE_SETUP_FLAGS Flags,
    __in DEVICE_TYPE VolumeDeviceType,
    __in FLT_FILESYSTEM_TYPE VolumeFilesystemType
    )
{
    UNREFERENCED_PARAMETER( FltObjects );
    UNREFERENCED_PARAMETER( Flags );
    UNREFERENCED_PARAMETER( VolumeDeviceType );
    UNREFERENCED_PARAMETER( VolumeFilesystemType );

    PAGED_CODE();

    KdPrint(("MiniFilter!PtInstanceSetup: Entered\n") );

    return STATUS_SUCCESS;
}


NTSTATUS
MfInstanceQueryTeardown (
    __in PCFLT_RELATED_OBJECTS FltObjects,
    __in FLT_INSTANCE_QUERY_TEARDOWN_FLAGS Flags
    )
{
    UNREFERENCED_PARAMETER( FltObjects );
    UNREFERENCED_PARAMETER( Flags );

    PAGED_CODE();

	KdPrint(("MiniFilter!PtInstanceQueryTeardown: Entered\n") );

    return STATUS_SUCCESS;
}


VOID
MfInstanceTeardownStart (
    __in PCFLT_RELATED_OBJECTS FltObjects,
    __in FLT_INSTANCE_TEARDOWN_FLAGS Flags
    )
{
    UNREFERENCED_PARAMETER( FltObjects );
    UNREFERENCED_PARAMETER( Flags );

    PAGED_CODE();

	KdPrint(("MiniFilter!PtInstanceTeardownStart: Entered\n") );
}


VOID
MfInstanceTeardownComplete (
    __in PCFLT_RELATED_OBJECTS FltObjects,
    __in FLT_INSTANCE_TEARDOWN_FLAGS Flags
    )
{
    UNREFERENCED_PARAMETER( FltObjects );
    UNREFERENCED_PARAMETER( Flags );

    PAGED_CODE();

    KdPrint(("MiniFilter!PtInstanceTeardownComplete: Entered\n") );
}