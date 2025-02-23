#include "ServerManager.h"

/*
* This function should store, and or log the Error message into
* a file or message box. In the current implemntation this attempts
* to write the error message to a string in the class and this can
* be used to write the error message out to the user. This is not 
* currently saving legable strings.
* 
* @param lpszFunction: This a a pointer to a constant ASCII string. 
* 
* @return: None
* 
* Note: Based on https://learn.microsoft.com/en-us/windows/win32/debug/retrieving-the-last-error-code
*/
void server_manage::ServerManager::errMsg(PCTSTR  lpszFunction) {
	// Pull in error message 
	DWORD dw = GetLastError();
	/*
	std::cout << "\n" << dw << "\n";
	*/
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dw,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&(this->lpMsgBuf),
		0, NULL);
	// Signal Error Message Box for WinForm, 

	/* Temp Error Message*/
	/*
	std::wcout << lpszFunction << this->lpMsgBuf;
	*/
	return;
}

/*
* Initalize the Unnamed Pipes used for communcating with the child process
* 
* @param: None
* 
* @return: Error Coded Integer, -1 if failure to create any of the pipes
*		   Otherwise return 1.
* 
* Note: Implementation based on https://learn.microsoft.com/en-us/windows/win32/procthread/creating-a-child-process-with-redirected-input-and-output?redirectedfrom=MSDN
*/
int server_manage::ServerManager::initPipes() {
	// Security Attributes Note: https://learn.microsoft.com/en-us/dotnet/api/microsoft.visualstudio.ole.interop.security_attributes?view=visualstudiosdk-2022
	SECURITY_ATTRIBUTES saAttr;

	// Set Size in bytes of the structure
	saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
	// Child processes will inherit this handle when created. (non-zero Value)
	saAttr.bInheritHandle = TRUE;
	// We are not assigning it a security descriptor (https://learn.microsoft.com/en-us/windows/win32/api/winnt/ns-winnt-security_descriptor)
	saAttr.lpSecurityDescriptor = NULL;

	if (!CreatePipe(&(this->g_hChildStd_OUT_Rd), &(this->g_hChildStd_OUT_Wr), &saAttr, 0)) {
		errMsg(TEXT("initPipes: Failed To Create STD_OUT unnamed pipe"));
		return -1;
	}

	// Ensure the read handle to the pipe for STDOUT is not inherited.
	if (!SetHandleInformation(this->g_hChildStd_OUT_Rd, HANDLE_FLAG_INHERIT, 0)) {
		errMsg(TEXT("initPipes: Failed To Configure STD_OUT unnamed pipe"));
		return -1;
	}
		

	// Create a pipe for the child process's STDIN. 
	if (!CreatePipe(&(this->g_hChildStd_IN_Rd), &(this->g_hChildStd_IN_Wr), &saAttr, 0)) {
		errMsg(TEXT("initPipes: Failed To Create STD_IN unnamed pipe"));
		return -1;
	}
	
	// Ensure the write handle to the pipe for STDIN is not inherited. 
	if (!SetHandleInformation(this->g_hChildStd_IN_Wr, HANDLE_FLAG_INHERIT, 0)) {
		errMsg(TEXT("initPipes: Failed To Configure STD_IN unnamed pipe"));
		return -1;
	}

	return 1;
}

/*
* Create the child process and attach to initalized pipes (If they are not 
* inialized they will output to stdin/out). The executable used for the 
* creation of a child process is defined by the string provided as an argument.
* 
* 
* @param cmd: This is a Pointer to an ASCII string we use as an argument to the 
*			  CreateProcessA procedure to spawn a child process.
* 
* @return: Error Coded Integer, -1 for a failure and 1 for a success.
* 
* Note: Based on https://learn.microsoft.com/en-us/windows/win32/procthread/creating-a-child-process-with-redirected-input-and-output?redirectedfrom=MSDN
*/
int server_manage::ServerManager::CreateServerProcess(std::string  cmd) {
	// Local Management Info
	STARTUPINFO siStartInfo;
	bool bSuccess = FALSE;
	wchar_t* convt_cmd = new wchar_t[strlen(cmd.c_str()) + 8];
	wchar_t* port_conv = new wchar_t[7];

	std::locale::global(std::locale(""));
	std::wcout.imbue(std::locale(""));  // Ensure std::wcout uses the correct locale
	// Setup convt_cmd so we send over a WideChar Str
	//convt_cmd = (wchar_t*)malloc(sizeof(wchar_t)* strlen(cmd)); // Attempting to free causes heap errors?
	/*
	std::wcout << "STR: " << this->server_port.c_str() << strlen(this->server_port.c_str()) << " " << "\n";
	*/

	// Convert LPSTR to wchar_t
	mbstowcs(convt_cmd, cmd.c_str(), strlen(cmd.c_str()) + 1); //Plus null
	mbstowcs(port_conv, this->server_port.c_str(), strlen(this->server_port.c_str()) + 1); //Plus null

	// Concatinate strings for passing to the CreateProcessA
	wcscat(convt_cmd, L" ");
	wcscat(convt_cmd, port_conv);

	/*
	std::wcout << convt_cmd << "\n";
	*/

	// Set up members of the PROCESS_INFORMATION structure. 
	ZeroMemory(&(this->server_proc), sizeof((this->server_proc)));

	// Set up members of the STARTUPINFO structure. 
	// This structure specifies the STDIN and STDOUT handles for redirection.
	ZeroMemory(&siStartInfo, sizeof(siStartInfo));
	siStartInfo.cb = sizeof(siStartInfo);
	siStartInfo.hStdError = this->g_hChildStd_OUT_Wr;
	siStartInfo.hStdOutput = this->g_hChildStd_OUT_Wr;
	siStartInfo.hStdInput = this->g_hChildStd_IN_Rd;
	siStartInfo.dwFlags |= STARTF_USESTDHANDLES;

	bSuccess = CreateProcess(NULL,
		convt_cmd,             // command line 
		NULL,                  // process security attributes 
		NULL,                  // primary thread security attributes 
		TRUE,                  // handles are inherited 
		0 | CREATE_NO_WINDOW,  // creation flags 
		NULL,                  // use parent's environment 
		NULL,                  // use parent's current directory 
		&siStartInfo,          // STARTUPINFO pointer 
		&(this->server_proc)); // receives PROCESS_INFORMATION 
	/*
	std::cout << this->server_proc.dwProcessId << "\n"; // Temp for Testing
	*/

	// If an error occurs, exit the application. 
	if (bSuccess == FALSE) {
		errMsg(TEXT("CreateServerProcess: Failed to create process"));
		/*
		std::cout << "\nFAILED\n";
		*/
		return -1;
	}

	// Close handles to the stdin and stdout pipes no longer needed by the child process.
	// If they are not explicitly closed, there is no way to recognize that the child process has ended.
	CloseHandle(this->g_hChildStd_OUT_Wr);
	g_hChildStd_OUT_Wr = NULL;
	CloseHandle(this->g_hChildStd_IN_Rd);
	g_hChildStd_IN_Rd = NULL;
	
	return 1;
}

/*
* This function will read from the pipe if it has been initalized, and write the string to a 
* provided char buffer.
* 
* @param buffObj: This is a pointer to a charicter buffer, this is the location the message
*                 from the child will be written to.
* 
* @param size: This specifis the size of the target buffObj we will be writing to.
* 
* @return: -1 if error, otherwise this will be the number of successfully read bytes.
* 
* Note: Based on https://learn.microsoft.com/en-us/windows/win32/procthread/creating-a-child-process-with-redirected-input-and-output?redirectedfrom=MSDN
*/
int server_manage::ServerManager::serverRead(char *buffObj, DWORD size) {
	DWORD dwRead;
	if (this->g_hChildStd_OUT_Rd == NULL) 
		return -1;

	memset(buffObj, 0, size);

	if (ReadFile(this->g_hChildStd_OUT_Rd, buffObj, size, &dwRead, NULL) == 0)
		return -1;

	return dwRead;
}

/*
* This function will kill the server process if it has been started
* 
* @param: None
* 
* @returnL result of killing process. Nonzeoro if success, 0 if failure. 
*
*/
int server_manage::ServerManager::killServer() {

	uint16_t ret;

	if (this->isStarted == 0)
		return 1;
	
	// Terminate the process, ensure it is not blocking the 
	// Listening Thread
	ret = TerminateProcess((this->server_proc).hProcess, 0);

	// Close handles that have not already been closed
	if (g_hChildStd_OUT_Rd != NULL)
		CloseHandle(this->g_hChildStd_OUT_Rd);
	if (g_hChildStd_IN_Wr != NULL)
		CloseHandle(this->g_hChildStd_IN_Wr);

	return ret;
}