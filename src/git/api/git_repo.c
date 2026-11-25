// Code written by Fom477
// Copyright Fom477 2026

#include <stdlib.h>

#include <build_check.h>
#include <curl/curl.h>
#include <git.h>
#include <string.h>
#include <git/repo.h>
#include <git/sock.h>
#include <socket/socket.h>

#if !BUILD_CHECK(GIT_REPO)
#   error PreprocessError: Modules not found
#endif

GitRepoProperty CreateGitRepoProperty(const char *RepoName, const char *RepoDesc, const int IsPublic)
{
    const GitRepoProperty Property = {.RepoName = RepoName, .RepoDesc = RepoDesc, .IsPublic = IsPublic};
    return Property;
}

GitRepoProperty CreateGitRepoPropertyDefault(const char *RepoName, const char *RepoDesc)
{
    return CreateGitRepoProperty(RepoName, RepoDesc, 1); // True
}

char *CreateGitRepo(const GitUserAuth UserAuth, const GitRepoProperty RepoProperty)
{
    printf("Creating repo of name %s\n", RepoProperty.RepoName);
    char Body[512];
    char Header[512];

    snprintf(Body, sizeof(Body),
        "{\"name\": \"%s\", \"description\": \"%s\", \"private\": \"%s\"}",
        RepoProperty.RepoName,
        RepoProperty.RepoDesc,
        RepoProperty.IsPublic? "true": "false");

    snprintf(Header, sizeof(Header), "Authorization: Bearer %s", UserAuth.UserToken);

    struct curl_slist *Headers = NULL;
    Headers = curl_slist_append(Headers, Header);
    Headers = curl_slist_append(Headers, "Accept: application/vnd.github+json");
    Headers = curl_slist_append(Headers, "Content-Type: application/json");
    Headers = curl_slist_append(Headers, GIT_AGENT);

    char *Raw = PerformCurl("https://api.github.com/user/repos", Headers, Body, "GET");
    if (!Raw)
    {
        fprintf(stderr, "Unable to perform curl");
        return NULL;
    }

    char *Response = strdup(Raw);
    free(Raw);

    printf("Successfully created repo: %s", Response);
    return Response;
}