// Code written by Fom477
// Copyright Fom477 2026

#ifndef FORMIT_GIT_REPO
#define FORMIT_GIT_REPO 1

typedef struct GitRepoProperty     GitRepoProperty;
typedef struct GitRepoPropertyList GitRepoPropertyList;

struct GitRepoProperty
{
    const char *RepoName;
    const char *RepoDesc;
    int         IsPublic; // Defaulted to true
};

struct GitRepoPropertyList
{
    size_t          Size;
    GitRepoProperty RepoProperties[];
};

#ifdef __cplusplus
extern "C" GitRepoProperty CreateGitRepoProperty       (const char *RepoName, const char *RepoDesc, int isPublic);
extern "C" GitRepoProperty CreateGitRepoPropertyDefault(const char *RepoName, const char *RepoDesc);

extern "C" char           *CreateGitRepo               (GitUserAuth UserAuth, GitRepoProperty RepoProperty);
// extern "C" GitRepoPropertyList *ExtendList(GitRepoPropertyList *List, GitRepoProperty Repo);
// extern "C" GitRepoPropertyList *GetGitRepos(GitUserAuth User);
#else
GitRepoProperty CreateGitRepoProperty       (const char *RepoName, const char *RepoDesc, int IsPublic);
GitRepoProperty CreateGitRepoPropertyDefault(const char *RepoName, const char *RepoDesc);

char           *CreateGitRepo               (GitUserAuth UserAuth, GitRepoProperty RepoProperty);
// GitRepoPropertyList *ExtendList(GitRepoPropertyList *List, GitRepoProperty Repo);
// GitRepoPropertyList *GetGitRepos(GitUserAuth User);
// TODO: Add repo modification & deletion stuff
#endif
#endif