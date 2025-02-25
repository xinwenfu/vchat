#include "essfunc.h"


// essfunc.dll for Vulnserver
// Visit my blog for more details: http://www.thegreycorner.com/

/*
Copyright (c) 2010, Stephen Bradshaw
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

	* Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
	* Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
	* Neither the name of the organization nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <stdio.h>
#include <string.h>

#define VERSION "2.12"

void EssentialFunc1() {
	printf("Called essential function dll version %s\n", VERSION);
}

void EssentialFunc2() {
	__asm  {
		jmp esp
		jmp eax
		pop eax
		pop eax
		ret
	}
}

void EssentialFunc3() {
	__asm {
		jmp esp
		jmp ecx
		pop ebx
		pop ebx
		ret
	}
}

void EssentialFunc4() {
	__asm  {
		jmp esp
		jmp ebx
		pop ebp
		pop ebp
		ret
	}
}

void EssentialFunc5() {
	__asm  {
		jmp esp
		jmp edi
		pop ebx
		pop ebx
		ret
	}
}

void EssentialFunc6() {
	__asm  {
		jmp esp
		jmp edx
		pop ecx
		pop edx
		ret
	};
}

void EssentialFunc7() {
	__asm  {
		jmp esp
		jmp esi
		pop ecx
		pop eax
		ret
	}
}


void EssentialFunc8() {
	__asm  {
		jmp esp
		jmp ebp
		pop eax
		pop edx
		ret
	}
}


void EssentialFunc9() {
	__asm  {
		jmp esp
		jmp esp
		jmp [esp-12]
		pop ecx
		pop ecx
		ret
	}
}


void EssentialFunc10(char* Input) {
	char Buffer2S[140];
	strcpy(Buffer2S, Input);
}

void EssentialFunc11(char* Input) {
	char Buffer2S[60];
	strcpy(Buffer2S, Input);
}


void EssentialFunc12(char* Status, char* Input) {
	char Buffer2S[2000];
	strcpy(Buffer2S, Input);
	printf("%s", Status);
}

void EssentialFunc13(char* Input) {
	char Buffer2S[2000];
	strcpy(Buffer2S, Input);
}

void EssentialFunc14(char* Input) {
	char Buffer2S[1000];
	strcpy(Buffer2S, Input);
}
