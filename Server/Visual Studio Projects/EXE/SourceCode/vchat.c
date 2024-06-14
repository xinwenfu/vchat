#define _WIN32_WINNT 0x501
#pragma comment(lib, "WS2_32.lib") // This Includes the ws2_32 library, so we do not have to deal with the options.

/*
VulnServer - a deliberately vulnerable threaded TCP server application

This is vulnerable software, don't run it on an important system!  The author assumes no responsibility if
you run this software and your system gets compromised, because this software was designed to be exploited!

Visit the original author's blog for more details: http://www.thegreycorner.com
*/


/*
Copyright (c) 2010, Stephen Bradshaw
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

	* Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
	* Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
	* Neither the name of the organization nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "vchat.h"

/********************************************************
    Add an incoming client to queue so that we can send messages to it
/********************************************************/
void queue_add(client_t* cl) {
	DWORD dwWaitResult;

	/* Enter Critical Section */
	dwWaitResult = WaitForSingleObject(
		ghMutex, 	// handle to mutex
		INFINITE);	// no time-out interval

	/* Result can be Successful Wait or Aborted */
	switch (dwWaitResult) {
	case WAIT_OBJECT_0:
#ifdef DEBUG
		printf("queue_add: Obtained ghMutex\r\n");
		fflush(stdout);
#endif
		/* Add a client to the queue if there is space*/
		cli_count++;
		printf("queue_add: A new client just came. %d clients now\r\n", cli_count);
		fflush(stdout);
		for (int i = 0; i < MAX_CLIENTS; ++i) {
			if (!clients[i]) {
				clients[i] = cl;
				break;
			}
		}

		/* Release ownership of the mutex object */
		if (!ReleaseMutex(ghMutex)) {
			printf("queue_add: ReleaseMutex error\r\n");    // Handle error.
			fflush(stdout);
		}
#ifdef DEBUG
		else {
			printf("queue_add: ReleaseMutex succeded\r\n");
			fflush(stdout);
		}
#endif
		break;
	case WAIT_ABANDONED:
		/*
		The thread got ownership of an abandoned mutex
		 An indeterminate state
		*/
		printf("queue_add: WAIT_ABANDONED\r\n");
		fflush(stdout);
		return;
	}
#ifdef DEBUG
	printf("queue_add: Finsihed\r\n");
	fflush(stdout);
#endif
}

/************************************************
A client leaves delete the client from queue
************************************************/
void queue_delete(client_t* cl) {
	DWORD dwWaitResult;

	/* Enter Critical Section */
	dwWaitResult = WaitForSingleObject(
		ghMutex, 	// handle to mutex
		INFINITE);	// no time-out interval

	/* Result can be Successful Wait or Aborted */
	switch (dwWaitResult)
	{
	case WAIT_OBJECT_0:
#ifdef DEBUG
		printf("queue_delete: Obtained ghMutex\r\n");
		fflush(stdout);
#endif
		/* Remove client from queue */
		cli_count--;
		printf("queue_delete: A client just left. %d clients now\r\n", cli_count);
		fflush(stdout);
		for (int i = 0; i < MAX_CLIENTS; ++i) {
			if (clients[i]) {
				if (clients[i]->uid == cl->uid) {
					clients[i] = NULL;
					break;
				}
			}
		}
		free(cl);

		/* Release ownership of the mutex object */
		if (!ReleaseMutex(ghMutex)) {
			printf("queue_delete: ReleaseMutex error\r\n");    // Handle error.
			fflush(stdout);
		}
#ifdef DEBUG
		else {
			printf("queue_delete: ReleaseMutex succeded\r\n");
			fflush(stdout);
		}
#endif
		break;
	case WAIT_ABANDONED:
		/* 
		The thread got ownership of an abandoned mutex
		An indeterminate state
		*/
		printf("queue_delete: WAIT_ABANDONED\r\n");
		fflush(stdout);
		return;
	}
#ifdef DEBUG
	printf("queue_delete: Finished\r\n");
	fflush(stdout);
#endif
}

/************************************************
   Broadcast message to current clients
************************************************/
int broadcast(char* KnocBuf, client_t* pCli) {
	DWORD dwWaitResult;
	int Result, SendResult = -1;

	/* 
	Enter Critical section 
	We need mutex since we want to make sure all the clients are alive
	*/
	dwWaitResult = WaitForSingleObject(
		ghMutex, 	// handle to mutex
		INFINITE);	// no time-out interval

	/* Result can be Successful Wait or Aborted */
	switch (dwWaitResult) {
	case WAIT_OBJECT_0:
#ifdef DEBUG
		printf("broadcast: Obtained ghMutex\r\n");
		fflush(stdout);
#endif

		Result = strlen(KnocBuf);

		/* Broadcast the message to all other clients */
		for (int i = 0; i < MAX_CLIENTS; ++i) {
			if (clients[i]) {
				if (clients[i]->uid != pCli->uid) {
					SendResult = send(clients[i]->connfd, KnocBuf, Result, 0);
				}
			}
		}

		/* Release ownership of the mutex object */
		if (!ReleaseMutex(ghMutex)) {
			printf("broadcast: ReleaseMutex error\r\n");    // Handle error.
			fflush(stdout);
		}
#ifdef DEBUG
		else {
			printf("broadcast: ReleaseMutex succeded\r\n");
			fflush(stdout);
		}
#endif
		break;
	case WAIT_ABANDONED:
		/*
		The thread got ownership of an abandoned mutex
		An indeterminate state
		*/
		printf("broadcast: WAIT_ABANDONED\r\n");
		fflush(stdout);
		break;
	}

#ifdef DEBUG
	printf("broadcast: Finished\r\n");
	fflush(stdout);
#endif

	return SendResult;
}

/************************************************
Check if there is vacancy in the client list
We intentionally allow a limited number of clients
************************************************/
bool checkVacancy() {
	DWORD dwWaitResult;
	bool hasVacancy = FALSE;

	// Need mutex to check if there is any slot on the client list
	dwWaitResult = WaitForSingleObject(
		ghMutex, 	// handle to mutex
		INFINITE);	// no time-out interval

	switch (dwWaitResult) {
	case WAIT_OBJECT_0:
#ifdef DEBUG
		printf("CheckVacancy: Obtained ghMutex\r\n");
		fflush(stdout);
#endif
		if (cli_count < MAX_CLIENTS)
			hasVacancy = TRUE;
		else
			hasVacancy = FALSE;

		// Release ownership of the mutex object
		if (!ReleaseMutex(ghMutex)) {
			printf("CheckVacancy: ReleaseMutex error\r\n");    // Handle error.
			fflush(stdout);
		}
		else {
#ifdef DEBUG
			printf("CheckVacnacy: ReleaseMutex success\r\n");
			fflush(stdout);
#endif
		}

		break;

		// The thread got ownership of an abandoned mutex
		// An indeterminate state
	case WAIT_ABANDONED:
		printf("CheckVacancy: WAIT_ABANDONED\r\n");
		fflush(stdout);
	}

#ifdef DEBUG
	printf("CheckVacancy: Finished\r\n");
	fflush(stdout);
#endif

	return hasVacancy;
}

/************************************************
Accept user connections
Start a thread for each connection
************************************************/
DWORD WINAPI acceptHandler(LPVOID ptr) {
	
	char* PortNumber = (char*)ptr;
	WSADATA wsaData;
	SOCKET ListenSocket = INVALID_SOCKET, ClientSocket = INVALID_SOCKET;
	struct addrinfo* result = NULL, hints;
	int Result;
	struct sockaddr_in ClientAddress;
	int ClientAddressL = sizeof(ClientAddress);

	/* Initiate Winsock DLL*/
	Result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (Result != 0) {
		printf("WSAStartup failed with error: %d\r\n", Result);
		fflush(stdout);
		return 1;
	}

	/* Zero out hints addrinfor struct */
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;          // IPv4 
	hints.ai_socktype = SOCK_STREAM;    // TCP (Specify reliable 2-way)
	hints.ai_protocol = IPPROTO_TCP;    // TCP (default when Sockstream and AF_INET used)
	hints.ai_flags = AI_PASSIVE;        // Specify Socket will be used in a call to bind

	/* 
	Get socket information based on the desired port number and 
	the values set in hint (is TCP port XXX avalable)
	*/
	Result = getaddrinfo(NULL, PortNumber, &hints, &result);
	if (Result != 0) {
		printf("Getaddrinfo failed with error: %d\r\n", Result);
		fflush(stdout);
		WSACleanup();
		return 1;
	}

	/* Create Socket based on result info */
	ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (ListenSocket == INVALID_SOCKET) {
		printf("Socket failed with error: %ld\r\n", WSAGetLastError());
		fflush(stdout);
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}

	/* Bind sock to desired ListenSocket port */
	Result = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
	if (Result == SOCKET_ERROR) {
		printf("Bind failed with error: %d\r\n", WSAGetLastError());
		fflush(stdout);
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	/*
	We no longer need the addr info loaded into results
	getaddrinfo will dynamically allocate objects
	*/
	freeaddrinfo(result);

	/* Listen For Connections */
	Result = listen(ListenSocket, SOMAXCONN);
	if (Result == SOCKET_ERROR) {
		printf("Listen failed with error: %d\r\n", WSAGetLastError());
		fflush(stdout);
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	/* Infinate loop to accept and dispatch client connections */
	while (1) {
		printf("Waiting for client connections...\r\n");
		fflush(stdout);
		/* Accept connections from the listen socket allocated earlier */
		ClientSocket = accept(ListenSocket, (SOCKADDR*)&ClientAddress, &ClientAddressL);
		if (ClientSocket == INVALID_SOCKET) {
			if (!end_flag) {
				printf("Accept failed with error: %d\r\n", WSAGetLastError());
				fflush(stdout);
			}
			closesocket(ListenSocket);
			WSACleanup();
			return 1;
		}

		printf("Received a client connection from %s:%u\r\n", inet_ntoa(ClientAddress.sin_addr), htons(ClientAddress.sin_port));
		fflush(stdout);

		/* Check if max clients is reached */
		if (!checkVacancy()) {
			printf("<< max clients reached\r\n");
			fflush(stdout);
			printf("<< reject ");
			fflush(stdout);
			printf("\r\n");
			closesocket(ClientSocket);
			continue;
		}

		/* Client settings */
		client_t* cli = (client_t*)malloc(sizeof(client_t));
		cli->addr = ClientAddress;
		cli->connfd = ClientSocket;
		cli->uid = uid++;
		sprintf(cli->name, "%d", cli->uid);

#ifdef DEBUG
		printf("To CreateThread\r\n");
		fflush(stdout);
#endif
		/*
		Dispatch Thread to Connection Handler
		*/
		CreateThread(0, 0, ConnectionHandler, (LPVOID)cli, 0, 0);
	}
}

/************************************************
   Accept CTRL+C to exit the server, this is
   similar to a Signal Handler
************************************************/
BOOL WINAPI controlHandler(DWORD type) {
	if (type == CTRL_C_EVENT) {
		printf("CTRL+C Received. Terminating the server...\r\n");
		fflush(stdout);

		/* Set flag so child threads eventually exit */
		end_flag = 1;	// CTRL+C is received

		/* Close all sockets that are communciating with clients */
		for (int i = 0; i < MAX_CLIENTS; ++i) {
			if (clients[i]) {
				closesocket(clients[i]->connfd);
			}
		}

		/* Cleanup Listen Socket and Windows Socket Library intializations */
		closesocket(ListenSocket);	// Close the listening socket for accepting incoming connections
		WSACleanup();	            // Clean up socket
		
		
		/* Restore previous console mode */
		SetConsoleMode(hInput, prev_mode);	// Restore console mode to the previous mode

		/* Cleanup Mutex */
		CloseHandle(ghMutex);	// Close mutex
		return TRUE;
	}
	return FALSE;
}


void Function1(char* Input) {
	char Buffer2S[140];
	strcpy(Buffer2S, Input);
}

void Function2(char* Input) {
	char Buffer2S[60];
	strcpy(Buffer2S, Input);
}

void Function3(char* Input) {
	char Buffer2S[2000];
	strcpy(Buffer2S, Input);
}

void Function4(char* Input) {
	char Buffer2S[1000];
	strcpy(Buffer2S, Input);
}
void Function5(char* Input) {
	function_auth usr_auth;
	usr_auth.tgt_func = good_function;

	strcpy(usr_auth.buff, Input);

	/* Call function pointer */
	usr_auth.tgt_func();
	return;
}

/************************************************
   One thread of ConnectionHandler for each client that 
   connects to the VChat server 

   The chat server uses the KNOCK command which specifies
   that client messages are to be forwarded to all other 
   clients
************************************************/
DWORD WINAPI ConnectionHandler(LPVOID cli) {
	int RecvBufLen = DEFAULT_BUFLEN;
	char* RecvBuf = malloc(DEFAULT_BUFLEN);
	char BigEmpty[1000];
	char* GdogBuf = malloc(1024);
	int Result, SendResult, i, k;
	client_t* pCli = (client_t*)cli;

	/* Add client to connection list */
	queue_add(pCli);

	/* Initalize Buffers */
	memset(BigEmpty, 0, 1000);
	memset(RecvBuf, 0, DEFAULT_BUFLEN);
	SOCKET Client = pCli->connfd;

	/* Prompt user on connection */
	SendResult = send(Client, "Welcome to Vulnerable Server! Enter HELP for help.\n", 51, 0);
	sprintf(BigEmpty, "You are User %s.\n", pCli->name);
	SendResult = send(Client, BigEmpty, strlen(BigEmpty) + 1, 0);
	memset(BigEmpty, 0, 1000);

	/* Catch error if one occurs */
	if (SendResult == SOCKET_ERROR) {
		printf("Send failed with error: %d\r\n", WSAGetLastError());
		fflush(stdout);
		closesocket(Client);
		return 1;
	}

	/* While the client is connected, handle any messahes recieved */
	while (cli) {
		/* Recieve and store message */
		Result = recv(Client, RecvBuf, RecvBufLen, 0);
		
		/* If there are contents parse them */
		if (Result > 0) {
			if (strncmp(RecvBuf, "HELP ", 5) == 0) {
				const char NotImplemented[47] = "Command specific help has not been implemented\n";
				SendResult = send(Client, NotImplemented, sizeof(NotImplemented), 0);
			}
			else if (strncmp(RecvBuf, "HELP", 4) == 0) {
				const char ValidCommands[251] = "Valid Commands:\nHELP\nSTATS [stat_value]\nRTIME [rtime_value]\nLTIME [ltime_value]\nSRUN [srun_value]\nTRUN [trun_value]\nGMON [gmon_value]\nGDOG [gdog_value]\nKSTET [kstet_value]\nGTER [gter_value]\nHTER [hter_value]\nLTER [lter_value]\nKSTAN [lstan_value]\nEXIT\n";
				SendResult = send(Client, ValidCommands, sizeof(ValidCommands), 0);
			}
			else if (strncmp(RecvBuf, "STATS ", 6) == 0) {
				char* StatBuf = malloc(120);
				memset(StatBuf, 0, 120);
				strncpy(StatBuf, RecvBuf, 120);
				SendResult = send(Client, "STATS VALUE NORMAL\n", 19, 0);
			}
			else if (strncmp(RecvBuf, "RTIME ", 6) == 0) {
				char* RtimeBuf = malloc(120);
				memset(RtimeBuf, 0, 120);
				strncpy(RtimeBuf, RecvBuf, 120);
				SendResult = send(Client, "RTIME VALUE WITHIN LIMITS\n", 26, 0);
			}
			else if (strncmp(RecvBuf, "LTIME ", 6) == 0) {
				char* LtimeBuf = malloc(120);
				memset(LtimeBuf, 0, 120);
				strncpy(LtimeBuf, RecvBuf, 120);
				SendResult = send(Client, "LTIME VALUE HIGH, BUT OK\n", 25, 0);
			}
			else if (strncmp(RecvBuf, "SRUN ", 5) == 0) {
				char* SrunBuf = malloc(120);
				memset(SrunBuf, 0, 120);
				strncpy(SrunBuf, RecvBuf, 120);
				SendResult = send(Client, "SRUN COMPLETE\n", 14, 0);
			}
			else if (strncmp(RecvBuf, "TRUN ", 5) == 0) {
				char* TrunBuf = malloc(3000);
				memset(TrunBuf, 0, 3000);
				for (i = 5; i < RecvBufLen; i++) {
					if ((char)RecvBuf[i] == '.') {
						strncpy(TrunBuf, RecvBuf, 3000);
						Function3(TrunBuf);
						break;
					}
				}
				memset(TrunBuf, 0, 3000);
				SendResult = send(Client, "TRUN COMPLETE\n", 14, 0);
			}
			else if (strncmp(RecvBuf, "KNOCK ", 6) == 0) {
				/************************************************
				  Chat server handling is implemented here

				  May perform "MITM" if enabled
				************************************************/
				printf("Received a message\r\n");
				fflush(stdout);

				char* KnocBuf = malloc(3000);
				memset(KnocBuf, 0, 3000);
				for (i = 6; i < RecvBufLen; i++) {
					if ((char)RecvBuf[i] == '.') {
						strncpy(KnocBuf, RecvBuf, 3000);
						Function3(KnocBuf);
						break;
					}
				}
				memset(KnocBuf, 0, 3000);

				RecvBuf[Result] = '\0';
				sprintf(KnocBuf, "%s", RecvBuf + 6);

				// If MITM attack is enabled, add some junk to the message
				if (MITM) {
					char MITMStr[] = " hahaha...";
					sprintf(KnocBuf, "%s %s", RecvBuf + 6, MITMStr);
					printf("RecvBuf=%s\r\n", KnocBuf);
					fflush(stdout);
				}

				broadcast(KnocBuf, pCli);

				free(KnocBuf);
				/************************************************
				  End of KNOCK Command
				************************************************/

			}
			else if (strncmp(RecvBuf, "MITM ", 5) == 0) {
				/************************************************
				Admin to set MITM on server
				************************************************/
				if ((char)RecvBuf[5] == '1') {
					MITM = true;
				} if ((char)RecvBuf[5] == '0') {
					MITM = false;
				}
				/************************************************
				  End of MITM Command
				************************************************/
			}
			else if (strncmp(RecvBuf, "NAME ", 5) == 0) {
				/************************************************ 
				  Change name function 
				************************************************/
				memcpy(pCli->name, RecvBuf + 5, Result - 5);
				// printf("Name changed to %s, length=%d\r\n", pCli->name, strlen(pCli->name));
				pCli->name[Result - 5 - 2] = '\0';
				// printf("Name changed to %s, length=%d\r\n", pCli->name, strlen(pCli->name));
				
				/************************************************
				  End Change Name Function
				************************************************/
			}
			else if (strncmp(RecvBuf, "FUNCC", 5) == 0) {
				/************************************************
				  Begin CFG Exploit Function
				************************************************/
				char* FuncBuff = malloc(2048);
				memset(FuncBuff, 0, 2048);
				strncpy(FuncBuff, RecvBuf, 2048);
				memset(RecvBuf, 0, DEFAULT_BUFLEN);
				Function5(FuncBuff);
				/************************************************
				  End CFG Exploit Function
				************************************************/
			}
			else if (strncmp(RecvBuf, "GMON ", 5) == 0) {
				char GmonStatus[13] = "GMON STARTED\n";
				for (i = 5; i < RecvBufLen; i++) {
					if ((char)RecvBuf[i] == '/') {
						if (strlen(RecvBuf) > 3950) {
							Function3(RecvBuf);
						}
						break;
					}
				}
				SendResult = send(Client, GmonStatus, sizeof(GmonStatus), 0);
			}
			else if (strncmp(RecvBuf, "GDOG ", 5) == 0) {
				strncpy(GdogBuf, RecvBuf, 1024);
				SendResult = send(Client, "GDOG RUNNING\n", 13, 0);
			}
			else if (strncmp(RecvBuf, "KSTET ", 6) == 0) {
				char* KstetBuf = malloc(100);
				strncpy(KstetBuf, RecvBuf, 100);
				memset(RecvBuf, 0, DEFAULT_BUFLEN);
				Function2(KstetBuf);
				SendResult = send(Client, "KSTET SUCCESSFUL\n", 17, 0);
			}
			else if (strncmp(RecvBuf, "GTER ", 5) == 0) {
				char* GterBuf = malloc(180);
				memset(GdogBuf, 0, 1024);
				strncpy(GterBuf, RecvBuf, 180);
				memset(RecvBuf, 0, DEFAULT_BUFLEN);
				Function1(GterBuf);
				SendResult = send(Client, "GTER ON TRACK\n", 14, 0);
			}
			else if (strncmp(RecvBuf, "HTER ", 5) == 0) {
				char THBuf[3];
				memset(THBuf, 0, 3);
				char* HterBuf = malloc((DEFAULT_BUFLEN + 1) / 2);
				memset(HterBuf, 0, (DEFAULT_BUFLEN + 1) / 2);
				i = 6;
				k = 0;
				while ((RecvBuf[i]) && (RecvBuf[i + 1])) {
					memcpy(THBuf, (char*)RecvBuf + i, 2);
					unsigned long j = strtoul((char*)THBuf, NULL, 16);
					memset((char*)HterBuf + k, (byte)j, 1);
					i = i + 2;
					k++;
				}
				Function4(HterBuf);
				memset(HterBuf, 0, (DEFAULT_BUFLEN + 1) / 2);
				SendResult = send(Client, "HTER RUNNING FINE\n", 18, 0);
			}
			else if (strncmp(RecvBuf, "LTER ", 5) == 0) {
				char* LterBuf = malloc(DEFAULT_BUFLEN);
				memset(LterBuf, 0, DEFAULT_BUFLEN);
				i = 0;
				while (RecvBuf[i]) {
					if ((byte)RecvBuf[i] > 0x7f) {
						LterBuf[i] = (byte)RecvBuf[i] - 0x7f;
					}
					else {
						LterBuf[i] = RecvBuf[i];
					}
					i++;
				}
				for (i = 5; i < DEFAULT_BUFLEN; i++) {
					if (LterBuf != NULL && (char)LterBuf[i] == '.') {
						Function3(LterBuf);
						break;
					}
				}
				memset(LterBuf, 0, DEFAULT_BUFLEN);
				SendResult = send(Client, "LTER COMPLETE\n", 14, 0);
			}
			else if (strncmp(RecvBuf, "KSTAN ", 6) == 0) {
				SendResult = send(Client, "KSTAN UNDERWAY\n", 15, 0);
			}
			else if (strncmp(RecvBuf, "EXIT", 4) == 0) {
				SendResult = send(Client, "GOODBYE\n", 8, 0);
				printf("Connection closing...\r\n");
				fflush(stdout);
				break; // Connection exits
			}
			else {
				SendResult = send(Client, "UNKNOWN COMMAND\n", 16, 0);
			}

			if (SendResult == SOCKET_ERROR) {
				printf("Send failed with error: %d\r\n", WSAGetLastError());
				fflush(stdout);
				break;
			}
		}
		else if (Result == 0) {
			/* If the connection has been gracefully closed, the return value is zero. */
			printf("Connection closing...\r\n");
			fflush(stdout);
			break;
		}
		else {
			/* 
			Otherwise, a value of SOCKET_ERROR is returned,
			and a specific error code can be retrieved by calling WSAGetLastError.
			*/

			if (!end_flag) {
				printf("Recv failed with error: %d\r\n", WSAGetLastError());
				fflush(stdout);
			}
			break;
		}

	}
	// The connection is closing for whatever reason
	
	/* Clean up Client Object */
	queue_delete(pCli); // Only deleted from the queue; Socket is not closed

	/* Clean up socket and buffers */
	closesocket(Client);
	free(RecvBuf);
	free(GdogBuf);
	return 1;
}

/* Main Function */
int main(int argc, char* argv[]) {
	char PortNumber[6];	// Server port number
	const char Usage[128] = "Wrong arguments!\nUsage: %s [port_number]\n\nIf no port number is provided, the default port of %s will be used.\n";
	
	/* Configure based on CLI arguments */
	if (argc > 2) {
		// Too many command line arguments
		printf(Usage, argv[0], DEFAULT_PORT);
		return 1;
	}
	else if (argc == 2) {
		// User provides port number
		if ((atoi(argv[1]) > 0) && (atoi(argv[1]) < 65536) && (strlen(argv[1]) < 7)) {
			strncpy(PortNumber, argv[1], 6);
		}
		else {
			printf(Usage, argv[0], DEFAULT_PORT);
			fflush(stdout);
			return 1;
		}
	}
	else {
		/* Use default port if no arguments are provided */
		strncpy(PortNumber, DEFAULT_PORT, 6);
	}

	/************************************************
	   Disable quick-edit mode which may stalk (cause issues
	   with) the server if user selects console text
	/************************************************/
	hInput = GetStdHandle(STD_INPUT_HANDLE);
	GetConsoleMode(hInput, &prev_mode);
	SetConsoleMode(hInput, ENABLE_EXTENDED_FLAGS | (prev_mode & ~ENABLE_QUICK_EDIT_MODE));
	/************************************************
	   End Quick edit modifications
	************************************************/


	/* Print info and ensure external dll is loaded */
	printf("Starting vulnserver version %s\r\n", VERSION);
	fflush(stdout);
	EssentialFunc1(); // Call function from external dll
	printf("\nThis is vulnerable software! Do not run at production systems!\nDo NOT try Windows console CMD's Mark and Copy! It stalks server!\n\nCTRL+C to terminate server!\n\r\n");
	fflush(stdout);

	/************************************************
	   Accept CTRL+C from the console to exit the server
	   setup handler.
	/************************************************/
	if (!SetConsoleCtrlHandler(controlHandler, TRUE)) {
		fprintf(stderr, "Failed SetConsoleCtrlHandler");
		fflush(stderr);
		return -1;
	}
	/************************************************
	   End CTRL+C 
	************************************************/

	/************************************************
	   We use mutex to maintain the integrity of a connect list
	   A received message will be bradcast to other clients than the sender
	   mutex makes sure the client is still alove with its socket
	************************************************/
	ghMutex = CreateMutex(
		NULL,              // default security attributes
		FALSE,             // initially not owned
		NULL);             // unnamed mutex

	if (ghMutex == NULL) {
		printf("CreateMutex error: %d\r\n", GetLastError());
		fflush(stdout);
		return 1;
	}

	/************************************************
	   We use a thread to handle incoming connections so that we can terminate the server with CTRL+C
	   Otherwise, accept() is blocking and CTRL+C cannot be processed
	************************************************/
	HANDLE hAccept = CreateThread(0, 0, acceptHandler, PortNumber, 0, 0);

	/************************************************
	   main thread is an infinite look 
	   Sleep ensures we do not use much CPU
	   (This is a busy wait)
	************************************************/
	while (!end_flag) { Sleep(100); }

	Sleep(1000); // last chance to clean up threads

	return 0;
}