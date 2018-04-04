#include "Sandbox.h"

BOOL ExtractParam (WCHAR * params, unsigned int beginIndex, unsigned int endIndex, WCHAR ** dest);

BOOL InitSandBox (WCHAR * buffer, ULONG bufferLength)
{
	unsigned int i = 0;
	unsigned int beginIndex = 0;
	unsigned int strLength = bufferLength / sizeof(WCHAR);

	FreeSandBox (gSandBox);

	// Retrieving the first param
	while (buffer[i] != ' ' && i < strLength)
		i++;

	if (ExtractParam (buffer, beginIndex, i - 1, &gSandBox.targetProcessName.Buffer) == FALSE)
		return FALSE;
	beginIndex = i + 1;

	if (beginIndex >= (strLength - 1))
		return FALSE;

	if (ExtractParam (buffer, beginIndex, strLength - 1, &gSandBox.sandDirectoryPath.Buffer) == FALSE)
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

static BOOL ExtractParam (WCHAR * params, unsigned int beginIndex, unsigned int endIndex, WCHAR ** dest)
{
	unsigned int size = (endIndex - beginIndex) + 2;
	unsigned int i = beginIndex;
	unsigned int index = 0;

	if (endIndex <= beginIndex)
		return FALSE;

	*dest = (WCHAR*)ExAllocatePoolWithTag (NonPagedPool, size * sizeof(WCHAR), SB_TAG);

	if (*dest == NULL)
		return FALSE;

	for (; i <= endIndex; i++)
	{
		(*dest)[index++] = params[i];
	}

	(*dest)[size - 1] = '\0';

	return TRUE;
}