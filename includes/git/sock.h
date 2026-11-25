// Code written by Fom477
// Copyright Fom477 2026
#ifndef FORMIT_GIT_SOCK
#define FORMIT_GIT_SOCK 1

#include <git.h>

GitUserAuth *ExchangeToken    (int Port, char *AuthCode, const char *ClientID, const char *ClientSecret);
GitUser     *GetUserProperty  (const char *AccessToken);
char        *SendRequest      (const char *Url, const char *Path, const char *RequestBody);
char        *StartLoopback    (int Port, const char *ClientID);
#endif