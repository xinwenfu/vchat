#pragma once
#include <iostream>
#include <windows.h> 

namespace server_manage {

    class ServerManager {
	private:
		bool isStarted; // Is the server started
		std::string server_port;
		PROCESS_INFORMATION server_proc; // Pointer to unmanaged type since we need to get around restrinctions, need to handle deallocation in destructor. 

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
		void errMsg(PCTSTR  lpszFunction);

	public:
		HANDLE g_hChildStd_IN_Rd = NULL; // Input and Output Pipes
		HANDLE g_hChildStd_IN_Wr = NULL;
		HANDLE g_hChildStd_OUT_Rd = NULL;
		HANDLE g_hChildStd_OUT_Wr = NULL;
		LPTSTR  lpMsgBuf;
	public:
		// Constructor
		ServerManager(LPSTR port): isStarted(0), server_port(port){
		}

		/*
		* Return server port string that is currently set
		* 
		* @param: None
		* 
		* @return: LPSTR containing port string.
		*/
		std::string get_serverPort() { return this->server_port;  }

		/*
		* Set server_port variable
		*
		* @param: nport: Should be a string consisting of an integer within 
		*                the valid port range
		*
		* @return: None
		*/
		void set_port(std::string nport) { this->server_port = nport; }

		/*
		* Set isStarted variable
		* 
		* @param: val: Boolean value of 1 or 0
		* 
		* @return: None
		*/
		void set_isStarted(bool val) { isStarted = val; }

		/*
		* This function will return the current server status. Is it started,
		* (True) or is it not started (False)
		*
		* @param: none
		*
		* @return: 1 (True) if the server is running, or 0 (False) if it is not.
		*/
		bool get_isStarted() { return this->isStarted; }

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
		int initPipes();

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
		int CreateServerProcess(std::string cmd);

		/*
		* This function will kill the server process if it has been started
		*
		* @param: None
		*
		* @returnL result of killing process. Nonzeoro if success, 0 if failure.
		*
		*/
		int killServer();
		
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
		int serverRead(char *buffObj, DWORD size);


		/* Destructor for server */
		~ServerManager() {
			if (isStarted == 0)
				return;
			
			this->killServer();
			//if (this->lpMsgBuf != NULL)
				//free(this->lpMsgBuf);
		}
	};
}