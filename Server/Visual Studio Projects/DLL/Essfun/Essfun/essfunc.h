// Only include the header file once
#pragma once

/*Visual Studio Defines to prevent errors for unsafe functions*/
#define _CRT_SECURE_NO_WARNINGS

#ifdef ESSFUN_EXPORTS
#define ESSFUN_API __declspec(dllexport)
#else
#define ESSFUN_API __declspec(dllimport)
#endif

ESSFUN_API void EssentialFunc1(void);
ESSFUN_API void EssentialFunc2(void);
ESSFUN_API void EssentialFunc3(void);
ESSFUN_API void EssentialFunc4(void);
ESSFUN_API void EssentialFunc5(void);
ESSFUN_API void EssentialFunc6(void);
ESSFUN_API void EssentialFunc7(void);
ESSFUN_API void EssentialFunc8(void);
ESSFUN_API void EssentialFunc9(void);
ESSFUN_API void EssentialFunc10(char*);
ESSFUN_API void EssentialFunc11(char*);
ESSFUN_API void EssentialFunc12(char*,char*);
ESSFUN_API void EssentialFunc13(char*);
ESSFUN_API void EssentialFunc14(char*);