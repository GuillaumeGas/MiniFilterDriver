#define __SANDBOX__
#include "Sandbox.h"

struct SandBox gSandBox;

BOOL ExtractParam (WCHAR * params, unsigned int beginIndex, unsigned int endIndex, UNICODE_STRING * dest);

BOOL InitSandBox (WCHAR * buffer, ULONG bufferLength)
{
	unsigned int i = 0;
	unsigned int beginIndex = 0;
	unsigned int strLength = bufferLength / sizeof(WCHAR);

	FreeSandBox (gSandBox);

	// Retrieving the first param
	while (buffer[i] != ' ' && i < strLength)
		i++;

	if (ExtractParam (buffer, beginIndex, i - 1, &gSandBox.targetProcessName) == FALSE)
		return FALSE;
	beginIndex = i + 1;

	if (beginIndex >= (strLength - 1))
		return FALSE;

	if (ExtractParam (buffer, beginIndex, strLength - 1, &gSandBox.sandDirectoryPath) == FALSE)
		return FALSE;

	gSandBox.state = ACTIVE;

	return TRUE;
}

VOID FreeSandBox (struct SandBox sandbox)
{
	if (sandbox.sandDirectoryPath.Buffer != NULL)
		ExFreePoolWithTag (sandbox.sandDirectoryPath.Buffer, SB_TAG);
	if (sandbox.targetProcessName.Buffer != NULL)
		ExFreePoolWithTag (sandbox.targetProcessName.Buffer, SB_TAG);
	sandbox.state = INACTIVE;
}

static BOOL ExtractParam (WCHAR * params, unsigned int beginIndex, unsigned int endIndex, UNICODE_STRING * dest)
{
	unsigned int size = (endIndex - beginIndex) + 2;
	unsigned int i = beginIndex;
	unsigned int index = 0;

	if (endIndex <= beginIndex)
		return FALSE;

	dest->Buffer = (WCHAR*)ExAllocatePoolWithTag (NonPagedPool, size * sizeof(WCHAR), SB_TAG);

	if (dest->Buffer == NULL)
		return FALSE;

	dest->Length = dest->MaximumLength = (size - 1) * sizeof (WCHAR);

	for (; i <= endIndex; i++)
	{
		dest->Buffer[index++] = params[i];
	}

	dest->Buffer[size - 1] = '\0';

	return TRUE;
}

BOOL IsTarget (UNICODE_STRING targetFileName)
{
	if (targetFileName.Length != gSandBox.targetProcessName.Length)
	{
		return FALSE;
	} else {
		unsigned int i = 0;
		for (; i < (targetFileName.Length / sizeof (WCHAR)); i++)
			if (targetFileName.Buffer[i] != gSandBox.targetProcessName.Buffer[i])
				return FALSE;
		return TRUE;
	}
}

VOID FilterPreOperation (PFLT_IO_PARAMETER_BLOCK ioParameter)
{
	// TODO
}