// Code written by Fom477
// Copyright Fom477 2026
#ifndef FORMIT_GIT
#define FORMIT_GIT 1

#define PORT 8080
#define GIT_AGENT "User-Agent: Fom477-GitAuth"

typedef struct GitClientInfo GitClientInfo;
typedef struct GitUser       GitUser;
typedef struct GitUserAuth   GitUserAuth;

struct GitClientInfo
{
    const char *ClientID;
    const char *ClientSecret;
};

struct GitUser
{
    const char *UserDisplayName;
    const char *UserMail;
    const char *UserName;
};

struct GitUserAuth
{
    GitUser        User;
    char          *UserToken;

    long long      StartTime;
    long long      EndTime;
    int           *ExpireStatus;
    int           *ExpireTimer;
};

#ifdef __cplusplus
extern "C" GitUserAuth Auth  (GitClientInfo ClientInfo);
extern "C" void        ReAuth(GitUserAuth *Auth, GitClientInfo ClientInfo);
#else
GitUserAuth Auth(GitClientInfo ClientInfo);
void        ReAuth(GitUserAuth *Auth, GitClientInfo ClientInfo);
#endif
#endif