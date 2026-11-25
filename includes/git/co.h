// Code written by Fom477
// Copyright Fom477 2026

#ifndef FORMIT_GIT_COLLABORATOR
#define FORMIT_GIT_COLLABORATOR 1

#include <git.h>
#include <git/repo.h>

typedef struct GitCollaborator     GitCollaborator;
typedef struct GitCollaboratorList GitCollaboratorList;

struct GitCollaborator
{
    const char *Name;

    int PermissionAdmin;
    int PermissionMaintain;
    int PermissionPull;
    int PermissionPush;
    int PermissionTriage;
};

struct GitCollaboratorList
{
    size_t          Size;
    GitCollaborator Users[];
};

#ifdef __cplusplus
extern "C" char                *AddCollaborator      (GitUserAuth User, const char *Repo, const char *UserName, const char *Permission);
extern "C" char                *AddCollaboratorSimple(GitUserAuth User, GitRepoProperty Repo, const char *UserName, const char *Permission);
extern "C" GitCollaborator      CreateCollaborator   (const char *Name, int PermissionAdmin, int PermissionMaintain, int PermissionPull, int PermissionPush, int PermissionTriage);
extern "C" GitCollaboratorList *ExtendCollaborator   (GitCollaboratorList *List, GitCollaborator Collaborator);
extern "C" GitCollaboratorList *GetCollaborators     (GitUserAuth User, const char *Repo);
extern "C" GitCollaboratorList *GetCollaboratorSimple(GitUserAuth User, GitRepoProperty Repo);
#else
char                *AddCollaborator      (GitUserAuth User, const char *Repo, const char *UserName, const char *Permission);
char                *AddCollaboratorSimple(GitUserAuth User, GitRepoProperty Repo, const char *UserName, const char *Permission);
GitCollaborator      CreateCollaborator   (const char *Name, int PermissionAdmin, int PermissionMaintain, int PermissionPull, int PermissionPush, int PermissionTriage);
GitCollaboratorList *ExtendCollaborator   (GitCollaboratorList *List, GitCollaborator Collaborator);
GitCollaboratorList *GetCollaborators     (GitUserAuth User, const char *Repo);
GitCollaboratorList *GetCollaboratorSimple(GitUserAuth User, GitRepoProperty Repo);
#endif
#endif