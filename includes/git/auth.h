// Code written by Fom477
// Copyright Fom477 2026
#ifndef FORMIT_GIT_AUTH
#define FORMIT_GIT_AUTH 1

#include <git.h>

#define FORMIT_CLIENT_ID_OFFSET_0 15
#define FORMIT_CLIENT_ID_OFFSET_1 31
#define FORMIT_CLIENT_SECRET_OFFSET_0 12
#define FORMIT_CLIENT_SECRET_OFFSET_1 25

static const char *UserMailId = "UserMail";
static const char *UserNameId = "UserName";
static const char *PwdTokenId = "PwdToken";

#ifdef __cplusplus
extern "C" GitUser       CreateGitUser    (const char *UserDisplayName, const char *UserMail, const char *UserName);
extern "C" GitUserAuth   CreateGitUserAuth(GitUser User, const char *PwdToken);
extern "C" GitClientInfo GetGitClientInfo ();
#else
GitUser       CreateGitUser    (const char *UserDisplayName, const char *UserMail, const char *UserName);
GitUserAuth   CreateGitUserAuth(GitUser User, const char *PwdToken);
GitClientInfo GetGitClientInfo ();
#endif
#endif