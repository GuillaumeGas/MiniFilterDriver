#include <Windows.h>
#include <Fltuser.h>
#include <stdio.h>
#include <Subauth.h>
#include <Shlwapi.h>

#define TEST_CMD "test"
#define TEST_FILE_PATH "C:\\tmp\\SandBoxTest.txt"

void printUsage ();

BOOL startTest ();

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
		DWORD bytesReturned;
		UNICODE_STRING targetName;

		if (wcscmp (argv[1], L"test") == 0)
			return startTest ();

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

		printf ("> The communication port with SandboxTest is open.\n");

		result = FilterSendMessage (port, (LPVOID)targetName.Buffer, targetName.Length * sizeof(wchar_t), NULL, 0, &bytesReturned);

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

BOOL startTest ()
{
	HANDLE fileHdl = INVALID_HANDLE_VALUE;

	if (PathFileExists (TEST_FILE_PATH) == TRUE)
	{
		if (DeleteFile (TEST_FILE_PATH) == FALSE)
		{
			printf ("DeleteFile() failed, error code : %d\n", GetLastError ());
			return FALSE;
		}
	}

	fileHdl = CreateFile(TEST_FILE_PATH, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (fileHdl == INVALID_HANDLE_VALUE)
	{
		printf ("CreateFile() failed, error code : %d\n", GetLastError ());
		return FALSE;
	}

	CloseHandle (fileHdl);

	return TRUE;
}