#include <Windows.h>
#include <Fltuser.h>
#include <stdio.h>

int __cdecl main(int argc, char ** argv)
{
	HRESULT result;
	HANDLE port;
	const wchar_t * portName = L"\\SandboxTest";

	result = FilterConnectCommunicationPort (portName, 0, NULL, 0, NULL, &port);

	if (result != S_OK)
	{
		printf ("FilterConnectCommunicationPort() failed.");
		return -1;
	}

	if (port == INVALID_HANDLE_VALUE)
	{
		printf ("Invalid port value, FilterConnectCommunicationPort() failed.");
		return -1;
	}

	printf ("> The communication port with SandboxTest is open.");

	CloseHandle (port);

	return 0;
}