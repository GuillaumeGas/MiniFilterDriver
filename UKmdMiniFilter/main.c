#include <Windows.h>
#include <Fltuser.h>
#include <stdio.h>
#include <Subauth.h>

void printUsage ();

#define DECLARE_CONST_UNICODE_STRING(_var, _string) \
const WCHAR _var ## _buffer[] = _string; \
const UNICODE_STRING _var = { sizeof(_string) - sizeof(WCHAR), sizeof(_string), (PWCH) _var ## _buffer } 

int __cdecl wmain(int argc, wchar_t ** argv)
{
	if (argc != 2)
	{
		printUsage ();
		return 0;
	}
	else
	{
		HRESULT result;
		HANDLE port;
		const wchar_t * portName = L"\\SandboxTest";
		/*const wchar_t * targetName = argv[1];*/
		DWORD bytesReturned;
		UNICODE_STRING targetName;

		targetName.Buffer = argv[1];
		targetName.Length = wcslen (argv[1]);
		targetName.MaximumLength = wcslen (argv[1]);

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

		result = FilterSendMessage (port, (LPVOID)&targetName, sizeof (targetName) + 1, NULL, 0, &bytesReturned);

		if (result != S_OK)
		{
			printf ("FilterSendMessage() failed.\n");
			CloseHandle (port);
			return -1;
		}
		else
		{
			printf ("> The target process name has been sent successfully.\n");
		}

		CloseHandle (port);

		return 0;
	}
}

void printUsage ()
{
	printf ("Usage : USandbox targetFilePath\n");
}