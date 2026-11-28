// Code written by Fom477
// Copyright Fom477 2026

#ifndef FORMIT_MAIN
#define FORMIT_MAIN 1

#if defined(__BYTE_ORDER__) && defined(__ORDER_LITTLE_ENDIAN__) && defined(__ORDER_BIG_ENDIAN__)
    #if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
        #define IS_LITTLE_ENDIAN 1
        #define IS_BIG_ENDIAN    0
    #elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
        #define IS_LITTLE_ENDIAN 0
        #define IS_BIG_ENDIAN    1
    #endif
#elif defined(_WIN32) || defined(__x86_64__) || defined(__i386__) || defined(__aarch64__) || defined(__ARMEL__)
    #define IS_LITTLE_ENDIAN 1
    #define IS_BIG_ENDIAN    0
#elif defined(__ARMEB__) || defined(__THUMBEB__) || defined(__AARCH64EB__) || defined(_MIBSEB) || defined(__MIPSEB)
    #define IS_LITTLE_ENDIAN 0
    #define IS_BIG_ENDIAN    1
#endif

#include <git.h>
#include <stage.h>

typedef struct FormitInstance FormitInstance;

struct FormitInstance
{
    const char     *InstanceName;

    GitClientInfo   ClientInfo;
    GitUserAuth    *GitUser;
    StagerInstance *Stager;
};

void RefreshToken(FormitInstance *Instance);

#endif