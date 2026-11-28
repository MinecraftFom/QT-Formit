// Code written by Fom477
// Copyright Fom477 2026

#include <curl/curl.h>
#include <json_object.h>
#include <json_tokener.h>
#include <stdlib.h>
#include <string.h>

#include <build_check.h>
#include <git.h>
#include <git/co.h>
#include <socket/socket.h>

#if !BUILD_CHECK(GIT_COLLABORATOR)
#error PreprocessorError: Module not found
#endif

char *AddCollaborator(const GitUserAuth User, const char *Repo, const char *UserName, const char *Permission)
{
    printf("Adding collaborator %s with permission %s\n", UserName, Permission);

    char Body[512];
    char Dest[512];
    char Header[512];

    snprintf(Body, sizeof(Body), "{\"permission\":\"%s\"}", Permission);
    snprintf(Dest, sizeof(Dest), "https://api.github.com/repos/%s/%s/collaborators/%s", User.User.UserName, Repo, UserName);
    snprintf(Header, sizeof(Header), "Authorization: Bearer %s", User.UserToken);

    struct curl_slist *Headers = NULL;
    Headers = curl_slist_append(Headers, Header);
    Headers = curl_slist_append(Headers, "Accept: application/vnd.github+json");
    Headers = curl_slist_append(Headers, "Content-Type: application/json");
    Headers = curl_slist_append(Headers, GIT_AGENT);

    char *Raw = PerformCurl(Dest, Headers, Body, "PUT");
    if (!Raw)
    {
        fprintf(stderr, "Unable to perform curl\n");
        return NULL;
    }

    char *Response = strdup(Raw);
    free(Raw);
    return Response;
}

char *AddCollaboratorSimple(const GitUserAuth User, const GitRepoProperty Repo, const char *UserName, const char *Permission)
{
    return AddCollaborator(User, Repo.RepoName, UserName, Permission);
}

GitCollaborator CreateCollaborator(const char *Name, const int PermissionAdmin, const int PermissionMaintain, const int PermissionPull, const int PermissionPush, const int PermissionTriage)
{
    const GitCollaborator Collaborator = {.Name = Name, .PermissionAdmin = PermissionAdmin, .PermissionMaintain = PermissionMaintain, .PermissionPull = PermissionPull, .PermissionPush = PermissionPush, .PermissionTriage = PermissionTriage};
    return Collaborator;
}

GitCollaboratorList *ExtendCollaborator(GitCollaboratorList *List, const GitCollaborator Collaborator)
{
    if (!List)
    {
        GitCollaboratorList *NewList = malloc(sizeof(GitCollaboratorList) + sizeof(GitCollaborator));
        NewList->Users[NewList->Size] = Collaborator;
        NewList->Size = 1;
        return NewList;
    }

    GitCollaboratorList *NewList = realloc(List, sizeof(GitCollaborator) * (List->Size + 1));
    NewList->Users[NewList->Size] = Collaborator;
    NewList->Size++;
    return NewList;
}

GitCollaboratorList *GetCollaborators(const GitUserAuth UserAuth, const char *Repo)
{
    printf("Getting repo collaborators of repo %s from user %s\n", Repo, UserAuth.User.UserName);
    char FullUrl[256];
    char Header[512];

    snprintf(FullUrl, sizeof(FullUrl), "https://api.github.com/repos/%s/%s/collaborators", UserAuth.User.UserName, Repo);
    snprintf(Header, sizeof(Header), "Authorization: Bearer %s", UserAuth.UserToken);
    struct curl_slist *Headers = NULL;
    Headers = curl_slist_append(Headers, Header);
    Headers = curl_slist_append(Headers, "Accept: application/vnd.github+json");
    Headers = curl_slist_append(Headers, "Content-Type: application/json");
    Headers = curl_slist_append(Headers, GIT_AGENT);

    char *Raw = PerformHttp(FullUrl, Headers);
    if (!Raw)
    {
        fprintf(stderr, "CurlError: Unable to perform curl\n");
        return NULL;
    }

    char *Response = strdup(Raw);
    free(Raw);
    printf("Successfully got response: %s\n", Response);

    GitCollaboratorList *List = malloc(sizeof(GitCollaboratorList));
    List->Size = 0;

    const json_object *JsonResponse = json_tokener_parse(Response);
    const size_t Length = json_object_array_length(JsonResponse);

    for (int i = 0; i < Length; i++)
    {
        const json_object *CurrentItem = json_object_array_get_idx(JsonResponse, i);
        const json_object *Permission  = json_object_object_get(CurrentItem, "permissions");

        json_object *Name              = json_object_object_get(CurrentItem, "login");
        const json_object *P_Admin     = json_object_object_get(Permission, "admin");
        const json_object *P_Maintain  = json_object_object_get(Permission, "maintain");
        const json_object *P_Pull      = json_object_object_get(Permission, "pull");
        const json_object *P_Push      = json_object_object_get(Permission, "push");
        const json_object *P_Triage    = json_object_object_get(Permission, "triage");

        if (!Name)
            continue;

        const char *N         = json_object_get_string(Name);
        const int   PAdmin    = json_object_get_boolean(P_Admin);
        const int   PMaintain = json_object_get_boolean(P_Maintain);
        const int   PPull     = json_object_get_boolean(P_Pull);
        const int   PPush     = json_object_get_boolean(P_Push);
        const int   PTriage   = json_object_get_boolean(P_Triage);

        const GitCollaborator CurrentCollaborator = CreateCollaborator(N, PAdmin, PMaintain, PPull, PPush, PTriage);
        List = ExtendCollaborator(List, CurrentCollaborator);
    }

    return List;
}

GitCollaboratorList *GetCollaboratorSimple(const GitUserAuth UserAuth, const GitRepoProperty Repo)
{
    return GetCollaborators(UserAuth, Repo.RepoName);
}