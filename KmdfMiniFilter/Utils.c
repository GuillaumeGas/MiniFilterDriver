#include "Utils.h"
#include <ntifs.h>

#define SU_TAG 'SU__'

static NTSTATUS GetProcessImageName (HANDLE, UNICODE_STRING *);

QUERY_INFO_PROCESS ZwQueryInformationProcess = NULL;

/*
	1) We retrieve a process handle thanks to its pid (PsLookupProcessByProcessId())
	2) We open the object represented by this hande (ObOpenObjectByPointer())
	3) Mem alloc for tmp processName var (ExAllocatePoolWithTag())
	4) Retrieve the image name (GetProcessImageName ())
	5) Success : copy into the out argument (unicode string, RtlCopyUnicodeString())
*/
BOOL GetProcessNameById (__in ULONG processId, __out PUNICODE_STRING processName)
{
	HANDLE processHandle = NULL;
	NTSTATUS status = STATUS_ACCESS_DENIED;
	PEPROCESS eProcess = NULL;
	UNICODE_STRING tmpProcName = {0};

	if (!processName)
		return FALSE;

	status = PsLookupProcessByProcessId ((HANDLE)processId, &eProcess);
	
	if (!NT_SUCCESS(status) || !eProcess)
		return FALSE;

	status = ObOpenObjectByPointer (eProcess, 0, NULL, 0, 0, KernelMode, &processHandle);
	
	if (!NT_SUCCESS(status) || !processHandle)
	{
		ObDereferenceObject (eProcess);
		return FALSE;
	}

	tmpProcName.Length = 0;
	tmpProcName.MaximumLength = 1024;
	tmpProcName.Buffer = (WCHAR*)ExAllocatePoolWithTag (NonPagedPool, tmpProcName.MaximumLength, SU_TAG);

	if (tmpProcName.Buffer == NULL)
	{
		ZwClose (processHandle);
		ObDereferenceObject (eProcess);
		return FALSE;
	}

	RtlZeroMemory (tmpProcName.Buffer, tmpProcName.MaximumLength);

	status = GetProcessImageName (processHandle, &tmpProcName);

	if (!NT_SUCCESS(status))
	{
		ExFreePoolWithTag (tmpProcName.Buffer, SU_TAG);
		ZwClose (processHandle);
		ObDereferenceObject (eProcess);
		return FALSE;
	}

	processName->Length = tmpProcName.Length;
	processName->MaximumLength = 255;

	if (processName->Buffer == NULL)
	{
		processName->Buffer = (WCHAR*)ExAllocatePoolWithTag (NonPagedPool, processName->MaximumLength * sizeof (WCHAR), SU_TAG);
	}

	RtlCopyUnicodeString (processName, &tmpProcName);

	ExFreePoolWithTag (tmpProcName.Buffer, SU_TAG);
	ZwClose (processHandle);
	ObDereferenceObject (eProcess);

	return TRUE;
}

static NTSTATUS GetProcessImageName (HANDLE processHandle, UNICODE_STRING * processImageName)
{
	NTSTATUS status = STATUS_ACCESS_DENIED;
	UNICODE_STRING * imageName = NULL;
	ULONG returnedLength = 0;
	ULONG bufferLength = 0;
	PVOID buffer = NULL;

	// on récupère un pointeur sur la fonction dont nous avons besoin pour récupérer le nom du process
	if (ZwQueryInformationProcess == NULL)
	{
		UNICODE_STRING routineName;

		RtlInitUnicodeString (&routineName, L"ZwQueryInformationProcess");
		ZwQueryInformationProcess = (QUERY_INFO_PROCESS) MmGetSystemRoutineAddress (&routineName);

		if (ZwQueryInformationProcess == NULL)
			return STATUS_INSUFFICIENT_RESOURCES;
	}

	// on récupère la taille de la chaine
	status = ZwQueryInformationProcess (processHandle, ProcessImageFileName, NULL, 0, &returnedLength);

	if (status != STATUS_INFO_LENGTH_MISMATCH)
		return status;

	buffer = ExAllocatePoolWithTag (PagedPool, returnedLength, SU_TAG);
	if (buffer == NULL)
		return STATUS_INSUFFICIENT_RESOURCES;
	
	status = ZwQueryInformationProcess (processHandle, ProcessImageFileName, buffer, returnedLength, &returnedLength);

	if (NT_SUCCESS (status))
	{
		imageName = (UNICODE_STRING*) buffer;
		RtlCopyUnicodeString (processImageName, imageName);
	}

	ExFreePool (buffer);
	return status;
}

VOID GetExeNameFromFullPath(__inout PUNICODE_STRING processName)
{
	int length = processName->Length / sizeof (WCHAR);
	int index = length;
	WCHAR * str = processName->Buffer;

	for (; index >= 0; index--)
	{
		if (str[index] == '\\')
		{
			index++;
			break;
		}
	}

	if (index >= length)
		return;

	if (str[index] == '\0')
		return;

	{
		int i = 0;
		while (index < length)
			str[i++] = str[index++];
		str[i] = '\0';
		processName->Length = i * sizeof (WCHAR);
	}
}