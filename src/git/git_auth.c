// Code written by Fom477
// Copyright Fom477 2026

#include <build_check.h>
#include <git/auth.h>
#include <git/secret.h>

#if !BUILD_CHECK(GIT_AUTH)
#   error PreprocessError: Modules not found
#endif

GitUser CreateGitUser(const char *UserDisplayName, const char *UserMail, const char *UserName)
{
    const GitUser User = {.UserDisplayName = UserDisplayName, .UserMail = UserMail, .UserName = UserName};
    return User;
}

GitUserAuth CreateGitUserAuth(const GitUser User, const char *PwdToken)
{
    const GitUserAuth UserAuth = {.User = User, .UserToken = PwdToken};
    return UserAuth;
}

GitClientInfo GetGitClientInfo() // Suggestingly suggest to only initialize this once in your project IF you prefer neat stack space
{
    const char *DECCID = Decode(FORMIT_CLIENT_ID, FORMIT_CLIENT_ID_OFFSET_0, FORMIT_CLIENT_ID_OFFSET_1);
    const char *DECCSC = Decode(FORMIT_CLIENT_SECRET, FORMIT_CLIENT_SECRET_OFFSET_0, FORMIT_CLIENT_SECRET_OFFSET_1);

    const GitClientInfo ClientInfo = {.ClientID = DECCID, .ClientSecret = DECCSC};
    return ClientInfo;
}