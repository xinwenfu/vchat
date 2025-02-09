/* Provides EssFunc functions and interface */
#include "essfunc.h"

#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>



/*
Define Constants used to control
default behavior and output.
*/
#define VERSION "2.20"
#define DEFAULT_BUFLEN 4096
#define DEFAULT_PORT "9999"
#define MAX_CLIENTS 100

/* 
Define Constants used to control HEAP 
overflow
*/
#define HELPER_STR_SIZE 1200
#define CHUNK_SIZE 0x190
#define ALLOC_COUNT 10
#define ALLOC_FREE 6
#define HEAPVULN

/*
Typedef for functionpointer
This is a reference for
indirect jumps to functions
with the signature void <name>(void);
*/
typedef void (*functionpointer)();

/* Structure used in CFG exploit */
typedef struct {
	char buff[800];
	functionpointer tgt_func;
} function_auth;

/* Client structure */
typedef struct {
	struct sockaddr_in addr; /* Client remote address */
	SOCKET connfd;              /* Connection file descriptor */
	int uid;                 /* Client unique identifier */
	char name[32];           /* Client name */
} client_t;

/* Array of connected clients */
static client_t* clients[MAX_CLIENTS];

/* Global UID value, incremented per connection */
int uid = 1;

/* Track number of user clients connected*/
unsigned int cli_count = 0;

/* If set, server will inject random data into user requests */
bool MITM = false;


HANDLE ghMutex; // Handle to Mutex to prevent race conditions on client operations

/* Handle to socket that we bind to and listen on */
SOCKET ListenSocket = INVALID_SOCKET;

/* 
Global Variable used to coordinate the exiting of threads 
only set to 0 by one thread when CTL+C is received. We
do not protect this with a mutex so there may be a few cycles
between this being set to 1 and the exit occuring (Not noticable 
to people) 
*/
static int end_flag = 0;	// Flag to indicate if CTRL+C is received

/*
Stores a set of Flags (integer bitmask) which will be used to 
restore the CLI interface to state it was in before VChat 
modified it to disable quick edit. 
*/
DWORD prev_mode;	// Console mode


HANDLE hInput;

/* 
Forward Declerations of functions used in normal operations
of the VChat Server.
*/
DWORD WINAPI ConnectionHandler(LPVOID CSocket);
DWORD WINAPI acceptHandler(LPVOID ptr);
void queue_delete(client_t* cl);
void queue_delete(client_t* cl);
int broadcast(char* KnocBuf, client_t* pCli);
bool checkVacancy();
BOOL WINAPI controlHandler(DWORD type);


/*
Vulnerable Functions where unsafe
operations occure
*/
void Function1(char* Input);
void Function2(char* Input);
void Function3(char* Input);
void Function4(char* Input);
void Function5(char* Input);
void Function6a(char* Input);
void Function6b(char* Input, char* str_trgt, void** allocs, functionpointer* trgt);
void Function7(char* Input, SOCKET client);
void Function8a(char* Rcv, SOCKET client);
void Function8b(char* Rcv, functionpointer ptr);

void EssentialFunc1();

/* Functions used for CFG overflow called by Function5 */
/* Declared and Defined here for clear seperation*/
void good_function(char* Input, SOCKET client) {
	/* Send some data so the client knows we were successful */
	send(client, "GOOD FUNCTION CALLED\n", 21, 0);
	send(client, "The Secret is _____ \n", 21, 0);
	return;
}

void bad_function(char* Input, SOCKET client) {
	/* Send some data so the client knows we were successful */
	send(client, "BAD FUNCTION CALLED\n", 20, 0);
	return;
}

/* Utility Function for Heap Overflow */
void fill_array(functionpointer* arr, functionpointer* ptr, unsigned size) {
	for (int i = 0; i < size; i++)
		arr[i] = ptr;
	return;
}