// Code written by Fom477
// Copyright Fom477 2026

#include <stdio.h>

#include <build_check.h>
#include <git.h>
#include <git/auth.h>
#include <git/sock.h>

#if !BUILD_CHECK(GIT_AUTH) || !BUILD_CHECK(GIT_SOCK)
#   error PreprocessError: Modules not found
#endif

#define BODY_BUFFER_SIZE 8192
#define TEMP_BUFFER_SIZE 256

GitUserAuth Auth(const GitClientInfo ClientInfo)
{
    printf("Authorizing github...\n");

    char *AuthCode = StartLoopback(PORT, ClientInfo.ClientID);
    printf("Authorization process finished\n");
    GitUserAuth *Auth = ExchangeToken(PORT, AuthCode, ClientInfo.ClientID, ClientInfo.ClientSecret);
    if (!Auth)
    {
        fprintf(stderr, "CurlError: Unable to get result\n");
        return CreateGitUserAuth(CreateGitUser("(NULL)", "(NULL@example.com)", "(NULL)"), "gho_???");
    }

    return *Auth;
}